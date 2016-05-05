; USER-ADJUSTABLE SETTINGS

EXTENDED_FORMAT=1
; ^ Set to 1 if you compressed with -e.
;   Makes decompressor 5 bytes shorter. No calculable speed difference.
;   If you compressed with -e3, set to 3. If with -e2, set to 2.
;   +2 makes decompresser 1 byte smaller, but consumes 2 more bytes of RAM.

FASTER=0
; ^ Set to 1 if you want faster code.
;   It will cause ReadBit to be inlined, and some other assorted changes.
;   Makes decompressor 37 bytes longer, but about 10 cycles faster per bit.

RAMBUFFER=1
; ^ Set to 1 if you can spare 7 more bytes of zero-page RAM.
;   Makes decompressor 6-7 bytes shorter, and 8-10 cycles faster per tile.
;   If FASTER=1, makes 5-6 bytes shorter, and 28-30 cycles faster per tile.

RAMTEMP=1
; ^ Set to 1 if you can spare 3 more bytes of zero-page RAM.
;   Makes decompressor 1-2 bytes longer, but 1.5 cycles faster per bit.

; SUMMARY -- SIZE PROFILE FOR DECOMPRESSOR:
;
;  RAMBUFFER RAMTEMP  CODE SIZE    RAM USE    MAXIMUM STACK USE
;  0         0        $DA (218)    12 bytes   13 bytes
;  0         1        $DC (220)    15 bytes   10 bytes
;  1         0        $D4 (212)    19 bytes   4 bytes
;  1         1        $D5 (213)    22 bytes   2 bytes
;
; IF EXTENDED_FORMAT=0,  ADD 5 BYTES
; IF EXTENDED_FORMAT>=2, SUBTRACT 1 BYTE, but add 2 bytes of RAM use
; IF FASTER=1,          ADD 37 BYTES.
; IF RAMBUFFER=1 & FASTER=1, ADD 1 EXTRA BYTE.
;
; END OF USER-ADJUSTABLE SETTINGS


.segment "ZEROPAGE"
; Global temporaries.
Count:           .res 4 ; 4 bytes, each is a 2-bit integer
Next:            .res 4 ; 4 bytes, each encodes three 2-bit integers
BitBuffer:	 .res 1 ; 1 byte, used by ReadBit

; Temporaries used during tile decoding.
RowsRemaining:   .res 1
Plane0:          .res 1 ; 8 bits (1 byte)
Plane1:          .res 1 ; 8 bits (1 byte)

.if RAMBUFFER=1
                 .res 7 ; 8 bytes
Part2 = *-8             ; overlap 1 byte with Plane1
.endif

; Temporaries used during color extraction.
; Reuses tile decoding temporaries,
; as they're not needed at the same time.
.if EXTENDED_FORMAT&2
ColorN:          .res 1
ColorA:          .res 1
.else
ColorN         = Plane0 ; current color
ColorA         = Plane1 ; pivot color
.endif
NextWork       = Next+0 ; workbench for Next,X

.if RAMTEMP=1
TilesRemaining:  .res 1
ReadBit_Atemp:   .res 1
ReadBit_Ytemp:   .res 1
.endif


; Inputs:
.importzp SourcePtr


.segment "DECOMPRESS"

.export DecompressTokumaru
DecompressTokumaru:
	; Input:  SourcePtr = pointer to input data
	; Output: SourcePtr = pointer to first byte after input data

	; Data will be written to $2007.
	; Clobbers: A,X,Y,C,ZN,V
	lda #$80
	sta BitBuffer

	ldy #0
.if EXTENDED_FORMAT&2
	sty Plane0
	sty Plane1
.endif
	lda (SourcePtr),y
.if RAMTEMP=1
	sta TilesRemaining
.else
	pha ; TilesRemaining is written at $101,s
.endif
	inc SourcePtr+0
	bne :+
	inc SourcePtr+1
:

.macro Read1Bit
    ; When RAMTEMP=0 and FASTER=0: Average: 20.75, minimum: 16, maximum: 58
    ; When RAMTEMP=0 and FASTER=1: Average: 10.50, minimum: 5, maximum: 58
    ; When RAMTEMP=1 and FASTER=0: Average: 19.38, minimum: 16, maximum: 52
    ; When RAMTEMP=1 and FASTER=1: Average:  9.75, minimum: 5, maximum: 52
    .if FASTER=1
	asl BitBuffer
	bne *+5
	jsr ReadBit_Reload
    .else
	jsr ReadBit
    .endif
.endmacro

.macro Read2Bits
;	; Read 2 bits from input
;	; Output: A = value (0-3); 6 high bits are undefined
;	; Clobbers: C, ZN
	; If FASTER=1
;	; Cost:
;	;    1536 out of 2048 times: 14 cycles
;	;     510 out of 2048 times: 57 cycles (51 if RAMTEMP=1)
;	;       2 out of 2048 times: 66 cycles (60 if RAMTEMP=1)
;	; Average: 24.76 cycles (23.26 if RAMTEMP=1)
;	; Minimum: 14 cycles
;	; Maximum: 66 cycles (60 if RAMTEMP=1)
	;
;	; If FASTER=0
;	; Cost:
;	;    1536 out of 2048 times: 36 cycles (36 if RAMTEMP=1)
;	;     510 out of 2048 times: 69 cycles (63 if RAMTEMP=1)
;	;       2 out of 2048 times: 78 cycles (72 if RAMTEMP=1)
;	; Average: 44.26 cycles (42.76 if RAMTEMP=1)
;	; Minimum: 36 cycles
;	; Maximum: 78 cycles    (72 if RAMTEMP=1)
      .repeat 2
	Read1Bit
	rol a
      .endrepeat
.endmacro


@BeginBlock:
	ldx #3 ; Color counter
@nextcolor:
	Read2Bits
	and #3
	sta Count,x
	beq @colorcounter
	; Choose pivot color
	;     Bit sequence x Goal   BitValue0 BitValue1 BitValue2 x>BitValue0 x<BitValue1
	;     0,10,11      0 1,2,3  0,1,2     1,2,3     2,1,0     0,0,0       1,1,1      
	;     0,10,11      1 0,2,3  0,1,2     1,2,3     2,1,0     1,0,0       0,1,1      
	;     0,10,11      2 0,1,3  0,1,2     1,2,3     2,1,0     1,1,0       0,0,1      
	;     0,10,11      3 0,1,2  0,1,2     1,2,3     2,1,0     1,1,1       0,0,0      
	;
	;   BitValue0 = ReadBit() ? 1 + ReadBit() : 0
	;   BitValue1 = ReadBit() ? 1 + ReadBit() : 0
	;   A = BitValue0 + (x < BitValue1) = BitValue0 + !(BitValue0 < x)
	;   A = BitValue1 - (BitValue0 < x) = BitValue1 - !(x < BitValue1)
.if 0
	stx NextWork
	ldy #$FF
:	iny
	cpy #2
	beq :+
	Read1Bit
	bcs :-
:	cpy NextWork
	bcc :+
	iny
:	;tya
.else
	lda #0
	Read1Bit
	bcc :+
	Read1Bit
	adc #1
:	stx ColorN
	cmp ColorN
	adc #0
	;tay
.endif

.if 0	; This bitmask code is so nice, but unfortunately the 4-byte table
	; it requires makes it 3 bytes larger than the alternative.
	lda BitMask,x ; X
	ora BitMask,y ; A
	sta ColorA ; bitmask
	tya
	; Save A in NextWork
	ldy #$FF
@store:	lsr a
	ror NextWork
	lsr a
	ror NextWork
:	asl ColorA
	beq @done
	iny
	bcs :-
@storey:; Store in NextWork
	tya
	bpl @store
@done:
.else
	sta ColorA
	ldy #255
	.byte $C9 ;cmp #
@storey:; Store in NextWork
	tya
@store:	lsr a
	ror NextWork
	lsr a
	ror NextWork
	;   ++p;   while(p==A || p==c) ++p;  C = p;
	;   p = 0; while(p==A || p==x) ++p;  B = p;
:	iny
	cpy ColorA
	beq :-
	cpy ColorN
	beq :-
	cpy #4
	bne @storey
.endif

	lda NextWork
	ldy Count,x
	cpy #2
	bne :+
	; If Count[x]=2, remove 00 from list
	lsr a
	lsr a
:	and #$FC ; Keep two low bits as 0
@colorcounter:
	sta Next,x
	dex
	bpl @nextcolor

.if (EXTENDED_FORMAT&2)=0
	; X = $FF
	; Clear the "previous" tile row
	inx ;ldx #0
	stx Plane0
	stx Plane1
.endif

	; First: X=0,       A=unknown, Y=unknown, Z=true, N=false, C=unknown
	; Other: X=unknown, A=unknown, Y=unknown, ZN=unknown, C=false
@BeginTile:
	; Read 8 rows
.if RAMBUFFER=1
	;2 cycles
    .if FASTER=1
	ldx #7
	stx RowsRemaining
@nextrow:
    .else
	ldx #8
@nextrow:
	stx RowsRemaining
    .endif
.else
	;7 cycles
	sec
	ror RowsRemaining ; When LSR clears this bit, 8 rows have been completed
@nextrow:
.endif
	; if(ReadBit()) goto RowIsDone;
	Read1Bit
	bcs @row_completed
	; Read 8 pixels
	; Read the color of the first pixel, and draw it
	Read2Bits
	ldx #1
	stx Plane1 ; When ROL puts this into carry, 8 pixels have been completed
	bne @putpixel

@process_next_pixel:
	; Process the next pixel.
	; X = color of previous pixel
	; A = next pixel
	; if(Count==0 || ReadBit()==1) choose X
	txa
	ora Next,x   ; Keep X in low 2 bits
	ldy Count,x
	beq @putpixel

.if EXTENDED_FORMAT&1

:	Read1Bit
	bcc @putpixel
	lsr a      ; put next0 in low 2 bits
	lsr a
	dey
	bne :-
.else

	Read1Bit
	bcs @putpixel
:	lsr a      ; put next0 in low 2 bits
	lsr a
	dey
	beq @putpixel
	Read1Bit
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
.if RAMBUFFER=1
	; 16 cycles per row
	ldx RowsRemaining
    .if FASTER=1
	dec RowsRemaining
	sta Part2,x
	bpl @nextrow
	; 6+8*13 + 2+8*(4+2+3)-1 = 183 cycles per tile (+2 if RAMTEMP=0)
    .else
	dex
	sta Part2,x
	bne @nextrow
	; 2+8*16 + 2+8*(4+2+3)-1 = 203 cycles per tile (+2 if RAMTEMP=0)
    .endif
	ldx #7
:	lda Part2,x
	sta $2007
	dex
	bpl :-
    .if RAMTEMP=0
	tsx
    .endif
.else
	; 8 cycles per row
	pha
	lsr RowsRemaining
	bne @nextrow
	; When RowsRemaining=0, carry is set by LSR
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
	; 7+8*8 + 2+2+3+2+2+8*(4+4+2+3+3)-1+2+2 = 213 cycles per tile
.endif
	; If there are more tiles to decode, decode
.if RAMTEMP=1
	dec TilesRemaining
.else
	dec $101,x ;TilesRemaining (accomplishes the same as PLA--SEC--SBC#1--PHA)
.endif
	beq EndCompress
	Read1Bit
	bcc @BeginTile
	jmp @BeginBlock

; ReadBit must be in same segment, because we use relative branch to EndCompress.
.export ReadBit
ReadBit:
	; Read 1 bit from input.
	; Output: C = bit
	; Clobbers: ZN
	;
	; When RAMTEMP=0:
	; Cost:
	;    1792 out of 2048 times: 4 cycles   (7 of 8)
	;     255 out of 2048 times: 37 cycles  (1 of 8, 255 of 256)
	;       1 out of 2048 times: 46 cycles  (1 of 8, 1 of 256)
	; Average: 8.75 cycles
	; Total average cost with JSR+RTS: 20.75 cycles
	; Minimum: 16 cycles
	; Maximum: 58 cycles
	;
	; When RAMTEMP=1:
	; Cost:
	;    1792 out of 2048 times: 4 cycles   (7 of 8)
	;     255 out of 2048 times: 31 cycles  (1 of 8, 255 of 256)
	;       1 out of 2048 times: 40 cycles  (1 of 8, 1 of 256)
	; Average: 7.38 cycles
	; Total average cost with JSR+RTS: 19.38 cycles
	; Minimum: 16 cycles
	; Maximum: 52 cycles
	asl BitBuffer
	beq ReadBit_Reload
	rts
ReadBit_Reload:
.if RAMTEMP
	sta ReadBit_Atemp
	sty ReadBit_Ytemp
.else
	pha
	 tya
	 pha
.endif
	  ldy #0
	  lda (SourcePtr),y
	  inc SourcePtr+0
	  beq rbwrap
:	  rol a
	  sta BitBuffer
.if RAMTEMP
	lda ReadBit_Atemp
	ldy ReadBit_Ytemp
EndCompress:
	rts
.else
	 pla
	 tay
EndCompress:
	pla
	rts
.endif
rbwrap:	  inc SourcePtr+1
	  bne :- ; Assumed to be unconditional.

;BitMask:	.byte $88,$48,$28,$18

	@e=DecompressTokumaru+$F0
	.assert * < @e, warning, .sprintf("Decompressor is %d bytes larger than Tokumaru's code",*-@e)

