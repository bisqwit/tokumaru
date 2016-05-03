EXTENDED_FORMAT=1
; ^ Set to 1 if you compressed with -e. This results in a smaller decompressor.

.segment "ZEROPAGE"
; Global temporaries
Count:           .byte 0,0,0,0 ; 4 bytes, each is a 2-bit integer
Next:            .byte 0,0,0,0 ; 4 bytes, each encodes three 2-bit integers
BitBuffer:	 .byte 0       ; 1 byte, used by ReadBit

; Temporaries used during color extraction
ColorN:          .byte 0           ; current color
ColorA:          .byte 0           ; pivot color
NextWork         = Next+0          ; workbench for Next,X

; Temporaries used during tile decoding
; Reuses color extraction temporaries,
; as they're not needed at the same time
RowsRemaining:   .byte 0
Plane0           = ColorN      ; 8 bits (1 byte)
Plane1           = ColorA      ; 8 bits (1 byte)

; Inputs:
.importzp SourcePtr

.segment "DECOMPRESS"

.export DecompressTokumaru
DecompressTokumaru:
	; Input: SourcePtr = pointer to input data
	; Data will be written to $2007.
	; Clobbers: A,X,Y,C,ZN,V
	lda #$80
	sta BitBuffer

	ldy #0
	lda (SourcePtr),y
	pha ; TilesRemaining is written at $101,s
	inc SourcePtr+0
	bne :+
	inc SourcePtr+1
:
@BeginBlock:
	ldx #3 ; Color counter
@nextcolor:
	jsr Read2Bits
	and #3
	sta Count,x
	beq @colorcounter
	; Choose pivot color
	;     Bit sequence Value of x Value of A
	;     0,10,11      0          1,2,3
	;     0,10,11      1          0,2,3
	;     0,10,11      2          0,1,3
	;     0,10,11      3          0,1,2
	;   A = ReadBit() ? 2 + ReadBit() : 1
	;   A -= !(x < A)
	stx ColorN
	lda #1
	jsr ReadBit
	bcc :+
	jsr ReadBit
	adc #1
:	sta ColorA
	cpx ColorA
	bcc :+
	; carry is set
	sbc #1
:	; Save A in Next,x
	ldy #255
	ldx #3
	sta ColorA
	bne @store
	;   p = 0; while(p==A || p==x) ++p;  B = p;
:	 iny
	 cpy ColorA
	 beq :-
	 cpy ColorN
	 beq :-
	 ; Store in Next,x
	 tya
@store:	 lsr a
	 ror NextWork
	 lsr a
	 ror NextWork
	;   ++p;   while(p==A || p==c) ++p;  C = p;
	dex
	bne :-

	ldx ColorN
	ldy Count,x
	lda NextWork

	dey
:	lsr a
	lsr a
	; Next,x now contains: --221100
	; If Count[x]=2, remove 00 from list
	dey
	beq :-
	sta Next,x

@colorcounter:
	txa
	cmp #2
	rol Next,x
	lsr a
	rol Next,x

	dex
	bpl @nextcolor

	; Clear the "previous" tile row
	inx ;ldx #0
	stx Plane0
	stx Plane1
@BeginTile:
	; Read 8 rows
	lda #8
	sta RowsRemaining
@nextrow:
	; if(ReadBit()) goto RowIsDone;
	jsr ReadBit
	bcs @row_completed
	; Read 8 pixels
	; Read the color of the first pixel, and draw it
	jsr Read2Bits
	ldx #1
	stx Plane1 ; When ROL puts this into carry, 8 pixels have been completed
	bne @putpixel

@process_next_pixel:
	; Process the next pixel.
	; X = color of previous pixel
	; A = next pixel
	; if(Count==0 || ReadBit()==1) choose X
	lda Next,x   ; Put X in low 2 bits
	ldy Count,x
	beq @putpixel

.if EXTENDED_FORMAT=1

:	jsr ReadBit
	bcc @putpixel
	lsr a      ; put next0 in low 2 bits
	lsr a
	dey
	bne :-
.else

	jsr ReadBit
	bcs @putpixel
:	lsr a      ; put next0 in low 2 bits
	lsr a
	dey
	beq @putpixel
	jsr ReadBit
	bcs :-

.endif

@putpixel:
	and #3 ; Required only because X is used as an index
	tax    ; Convert the color into index before changing A
	lsr a
	rol Plane0
	lsr a
	rol Plane1
	bcc @process_next_pixel
@row_completed:
	; Arrived here with C=set
	; Send the current row.
	; The first plane goes straight into PPU,
	; but the second plane must be buffered.
	lda Plane0
	sta $2007
	lda Plane1
	pha
	dec RowsRemaining
	bne @nextrow
	; Once the whole tile has been extracted,
	; send also the second plane.
	; To save RAM, the second plane was put into stack, in reverse order.
	; We can't just use PLA+STA in a loop, or we'll reverse the data.
	tsx
	; OldStackPointer     = X
	; CorrectStackPointer = X+8
	; for(Y=CorrectStackPointer; Y!=OldStackPointer; Y--) Send($100[Y]);
	@old = RowsRemaining ; reuse as temp
	txa
	sta @old
	adc #(8-1) ; C was set. Clears C.
	tax
:	ldy $100,x
	sty $2007
	dex
	cpx @old
	bne :-
	; Restore stack pointer _after_ reading $100, so that
	; a possible interrupt happening in the middle of the
	; loop will not clobber our stack data.
	tax
	txs
	; If there are more tiles to decode, decode
	dec $101,x ;TilesRemaining (accomplishes the same as PLA--SEC--SBC#1--PHA)
	beq EndCompress
	jsr ReadBit
	bcc @BeginTile
	;bcs @BeginBlock
	jmp @BeginBlock


; ReadBit must be in same segment, because we use relative branch to EndCompress.
.export ReadBit
ReadBit:
	; Read 1 bit from input.
	; Output: C = bit
	; Clobbers: ZN
	; Cost:
	;    1792 out of 2048 times: 4 cycles   (7 of 8)
	;     255 out of 2048 times: 37 cycles  (1 of 8, 255 of 256)
	;       1 out of 2048 times: 46 cycles  (1 of 8, 1 of 256)
	; Average: 8.75 cycles
	; Total average cost with JSR+RTS: 20.75 cycles
	asl BitBuffer
	beq :+
	rts
:	pha
	 tya
	 pha
	  ldy #0
	  lda (SourcePtr),y
	  inc SourcePtr+0
	  beq rbwrap
:	  rol a
	  sta BitBuffer
	 pla
	 tay
EndCompress:
	pla
	rts
rbwrap:	  inc SourcePtr+1
	  bne :- ; Assumed to be unconditional.

Read2Bits:
	; Read 2 bits from input
	; Output: A = value (0-3); 6 high bits are undefined
	; Clobbers: C, ZN
	; Average cost: 57.5 cycles including JSR+RTS
	jsr ReadBit
	rol a
	jsr ReadBit
	rol a
	rts

;Fail1:	.byte 0,1 ; brk 1
;	@x:jmp @x

	@e=DecompressTokumaru+$F0
	.assert * < @e, warning, .sprintf("Decompressor is %d bytes larger than Tokumaru's code",*-@e)
