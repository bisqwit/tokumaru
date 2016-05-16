; USER-ADJUSTABLE SETTINGS

.ifndef EXTENDED_FORMAT
EXTENDED_FORMAT=0
.endif
; ^ Set to 1 if you compressed with -e.
;   Makes decompressor 5 bytes shorter, but 0-0.5 cycles slower per bit.
;   If you compressed with -e3, set to 3. If with -e2, set to 2.
;   +2 makes decompresser 1 byte smaller, but consumes 2 more bytes of RAM.

.ifndef FASTER
FASTER=0
.endif
; ^ FASTER can take values between 0 and 7.
;   +1 means bitmask code will be used in color initialization.
;   +2 means Read2Bits will use inlined Read1Bit rather than calling ReadBit.
;   +4 means Read1Bit will use inlined Read1Bit rather than calling ReadBit.

.ifndef RAMBUFFER
RAMBUFFER=1
.endif
; ^ Set to 1 if you can spare 7 more bytes of zero-page RAM.
;   Makes decompressor shorter and faster.

.ifndef RAMTEMP
RAMTEMP=0
.endif
; ^ Set to 1 if you can spare 3 more bytes of zero-page RAM.
;   Makes decompressor longer but faster.

; SUMMARY -- SIZE PROFILE FOR DECOMPRESSOR:
;
;  E F R T  CODE SIZE   RAM USE   STACK USE  CYCLES   PER BIT  PER TILE
;  1 0 1 0  $0D4 (212)  19 bytes   5 bytes   1161959  53.20    4538.90
;  1 0 1 1  $0D5 (213)  22 bytes   2 bytes   1144551  52.41    4470.90
;  0 0 1 0  $0D9 (217)  19 bytes   5 bytes   1159744  53.10    4530.25
;  1 0 0 0  $0DA (218)  12 bytes  12 bytes   1162215  53.21    4539.90
;  0 0 1 1  $0DA (218)  22 bytes   2 bytes   1142336  52.30    4462.25
;  1 0 0 1  $0DC (220)  15 bytes   9 bytes   1145319  52.44    4473.90
;  1 1 1 0  $0DC (220)  19 bytes   5 bytes   1162523  53.23    4541.11
;  1 1 1 1  $0DD (221)  22 bytes   2 bytes   1145115  52.43    4473.11
;  0 0 0 0  $0DF (223)  12 bytes  12 bytes   1161792  53.20    4538.25
;  1 1 0 0  $0E1 (225)  12 bytes  12 bytes   1163803  53.29    4546.11
;  0 0 0 1  $0E1 (225)  15 bytes   9 bytes   1144896  52.42    4472.25
;  0 1 1 0  $0E1 (225)  19 bytes   5 bytes   1160308  53.13    4532.45
;  0 1 1 1  $0E2 (226)  22 bytes   2 bytes   1142900  52.33    4464.45
;  1 1 0 1  $0E3 (227)  15 bytes   9 bytes   1146907  52.51    4480.11
;  1 2 1 0  $0E5 (229)  19 bytes   5 bytes   1138551  52.13    4447.46
;  0 1 0 0  $0E6 (230)  12 bytes  12 bytes   1161588  53.19    4537.45
;  1 2 1 1  $0E6 (230)  22 bytes   2 bytes   1121143  51.33    4379.46
;  0 1 0 1  $0E8 (232)  15 bytes   9 bytes   1144692  52.41    4471.45
;  1 4 1 0  $0E9 (233)  19 bytes   5 bytes    996983  45.65    3894.46
;  1 2 0 0  $0EA (234)  12 bytes  12 bytes   1139831  52.19    4452.46
;  0 2 1 0  $0EA (234)  19 bytes   5 bytes   1136336  52.03    4438.81
;  1 4 1 1  $0EA (234)  22 bytes   2 bytes    979575  44.85    3826.46
;  0 2 1 1  $0EB (235)  22 bytes   2 bytes   1118928  51.23    4370.81
;  1 2 0 1  $0EC (236)  15 bytes   9 bytes   1122935  51.42    4386.46
;  1 3 1 0  $0EC (236)  19 bytes   5 bytes   1138347  52.12    4446.67
;  1 3 1 1  $0ED (237)  22 bytes   2 bytes   1120939  51.33    4378.67
;  1 4 0 0  $0EE (238)  12 bytes  12 bytes    998263  45.71    3899.46
;  0 2 0 0  $0EF (239)  12 bytes  12 bytes   1137616  52.09    4443.81
;  1 4 0 1  $0F0 (240)  15 bytes   9 bytes    981367  44.93    3833.46
;  1 5 1 0  $0F0 (240)  19 bytes   5 bytes    996779  45.64    3893.67
;  1 3 0 0  $0F1 (241)  12 bytes  12 bytes   1139627  52.18    4451.67
;  0 2 0 1  $0F1 (241)  15 bytes   9 bytes   1120720  51.32    4377.81
;  0 3 1 0  $0F1 (241)  19 bytes   5 bytes   1136132  52.02    4438.02
;  1 5 1 1  $0F1 (241)  22 bytes   2 bytes    979371  44.84    3825.67
;  0 4 1 0  $0F2 (242)  19 bytes   5 bytes   1007008  46.11    3933.62
;  0 3 1 1  $0F2 (242)  22 bytes   2 bytes   1130964  51.78    4417.83
;  1 3 0 1  $0F3 (243)  15 bytes   9 bytes   1122731  51.41    4385.67
;  0 4 1 1  $0F3 (243)  22 bytes   2 bytes    989600  45.31    3865.62
;  1 6 1 0  $0F4 (244)  19 bytes   5 bytes    981134  44.92    3832.55
;  1 5 0 0  $0F5 (245)  12 bytes  12 bytes    998059  45.70    3898.67
;  1 6 1 1  $0F5 (245)  22 bytes   2 bytes    963726  44.13    3764.55
;  0 3 0 0  $0F6 (246)  12 bytes  12 bytes   1137412  52.08    4443.02
;  0 4 0 0  $0F7 (247)  12 bytes  12 bytes    996048  45.61    3890.81
;  1 5 0 1  $0F7 (247)  15 bytes   9 bytes    981163  44.93    3832.67
;  0 3 0 1  $0F8 (248)  15 bytes   9 bytes   1120516  51.31    4377.02
;  1 6 0 0  $0F9 (249)  12 bytes  12 bytes    968820  44.36    3784.45
;  0 4 0 1  $0F9 (249)  15 bytes   9 bytes    991392  45.39    3872.62
;  0 5 1 0  $0F9 (249)  19 bytes   5 bytes   1010814  46.28    3948.49
;  0 5 1 1  $0FA (250)  22 bytes   2 bytes    993406  45.49    3880.49
;  1 6 0 1  $0FB (251)  15 bytes   9 bytes    965518  44.21    3771.55
;  1 7 1 0  $0FB (251)  19 bytes   5 bytes    967336  44.29    3778.66
;  1 7 1 1  $0FC (252)  22 bytes   2 bytes    949928  43.49    3710.66
;  0 6 1 0  $0FD (253)  19 bytes   5 bytes    970325  44.43    3790.33
;  0 5 0 0  $0FE (254)  12 bytes  12 bytes   1012094  46.34    3953.49
;  0 6 1 1  $0FE (254)  22 bytes   2 bytes    952917  43.63    3722.33
;  1 7 0 0  $100 (256)  12 bytes  12 bytes    982210  44.97    3836.76
;  0 5 0 1  $100 (256)  15 bytes   9 bytes    995198  45.57    3887.49
;  0 6 0 0  $102 (258)  12 bytes  12 bytes    962021  44.05    3757.89
;  1 7 0 1  $102 (258)  15 bytes   9 bytes    951720  43.58    3717.66
;  0 6 0 1  $104 (260)  15 bytes   9 bytes    954709  43.71    3729.33
;  0 7 1 0  $104 (260)  19 bytes   5 bytes    960537  43.98    3752.10
;  0 7 1 1  $105 (261)  22 bytes   2 bytes    943129  43.18    3684.10
;  0 7 0 0  $109 (265)  12 bytes  12 bytes    971401  44.48    3794.54
;  0 7 0 1  $10B (267)  15 bytes   9 bytes    944921  43.27    3691.10
;
; E = EXTENDED_FORMAT
; F = FASTER
; R = RAMBUFFER
; T = RAMTEMP
EOF
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
.if !(FASTER&1)
ColorA:          .res 1
.endif
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
    ; When RAMTEMP=0 and !(FASTER&4): Average: 20.75, minimum: 16, maximum: 58
    ; When RAMTEMP=0 and !(FASTER&4): Average: 10.50, minimum: 5, maximum: 58
    ; When RAMTEMP=1 and !(FASTER&4): Average: 19.38, minimum: 16, maximum: 52
    ; When RAMTEMP=1 and !(FASTER&4): Average:  9.75, minimum: 5, maximum: 52
    .if (FASTER&4)
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
	; If FASTER&2
;	; Cost:
;	;    1536 out of 2048 times: 14 cycles
;	;     510 out of 2048 times: 57 cycles (51 if RAMTEMP=1)
;	;       2 out of 2048 times: 66 cycles (60 if RAMTEMP=1)
;	; Average: 24.76 cycles (23.26 if RAMTEMP=1)
;	; Minimum: 14 cycles
;	; Maximum: 66 cycles (60 if RAMTEMP=1)
	;
;	; If !(FASTER&2)
;	; Cost:
;	;    1536 out of 2048 times: 36 cycles (36 if RAMTEMP=1)
;	;     510 out of 2048 times: 69 cycles (63 if RAMTEMP=1)
;	;       2 out of 2048 times: 78 cycles (72 if RAMTEMP=1)
;	; Average: 44.26 cycles (42.76 if RAMTEMP=1)
;	; Minimum: 36 cycles
;	; Maximum: 78 cycles    (72 if RAMTEMP=1)
    .repeat 2
      .if (FASTER&2)
	asl BitBuffer
	bne *+5
	jsr ReadBit_Reload
      .else
	jsr ReadBit
      .endif
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
	lda #0
	Read1Bit
	bcc :+
	Read1Bit
	adc #1
:	stx ColorN
	cmp ColorN
	adc #0

.if (FASTER&1)
	; This bitmask code is so nice, but unfortunately the 4-byte table
	; it requires makes it larger than the alternative.
	tay
	; it requires makes it 3 bytes larger than the alternative.
	lda BitMask,x ; X
	ora BitMask,y ; A
	sta ColorN ; bitmask
	tya
	; Save A in NextWork
	ldy #$FF
@store:	lsr a
	ror NextWork
	lsr a
	ror NextWork
:	asl ColorN
	beq @done
	iny
	bcs :-
@storey:; Store in NextWork
	tya
	bpl @store
@done:

.else ; Non-bitmask code

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
    .if FASTER>0
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
    .if FASTER>0
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
.if (FASTER&6) < 6
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
.endif
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

.if (FASTER&1)
BitMask: .byte $88,$48,$28,$18
.endif

	@e=DecompressTokumaru+$F0
	.assert * < @e, warning, .sprintf("Decompressor is %d bytes larger than Tokumaru's code",*-@e)

