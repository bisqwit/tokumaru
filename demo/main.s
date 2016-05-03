.segment "ZEROPAGE"
.exportzp SourcePtr
SourcePtr:	.byte 0,0

.segment "DECOMPRESS"
.import DecompressTokumaru

.segment "CODE"
Reset:
	sei
	cld
	ldx #$FF
	txs
	; Disable rendering, NMI and stuff
	lda #0
	sta $2000
	sta $2001
	; Wait two vblanks
	bit $2002
	ldx #2
:	bit $2002
	bpl :-
	dex
	bne :-
	; Populate nametable and attribute table
	lda #<$2000
	ldx #>$2000
	jsr SetPPUaddr
	lda #<Initial_NameTable
	ldx #>Initial_NameTable
	jsr SetSource
	ldx #4
	ldy #0
:	lda (SourcePtr),y
	sta $2007
	iny
	bne :-
	inc SourcePtr+1
	dex
	bne :-
	; Populate palette
	lda #<$3F00
	ldx #>$3F00
	jsr SetPPUaddr
	lda #<Initial_Palette
	ldx #>Initial_Palette
	jsr SetSource
	ldy #0
:	lda (SourcePtr),y
	sta $2007
	iny
	cpy #$20
	bne :-
	; Populate CHR-RAM
	lda #<TilePage0
	ldx #>TilePage0
	jsr SetSource
	lda #<$0000
	ldx #>$0000
	jsr Decompress
	; Copy the CHR-RAM contents into PRAM for debugging
	jsr CopyPRAM
	jsr StarFieldInit
	; Wait 1 vblank, enable rendering and halt
:	bit $2002
	bmi :-
:	bit $2002
	bpl :-
	lda #0
	sta $2003
	sta $2006
	sta $2006
	sta $2005
	sta $2005
	lda #$80
	sta $2000
	lda #$1E
	sta $2001
@jam:	jmp @jam

Decompress:	
	jsr SetPPUaddr
	jmp DecompressTokumaru

SetPPUaddr:	
	bit $2002
	stx $2006
	sta $2006
	rts

SetSource:
	sta SourcePtr+0
	stx SourcePtr+1
	rts

CopyPRAM:
	; Copies $1000 bytes from PPU $0000 into RAM $6000-$6FFF
	lda #<$0000
	ldx #>$0000
	jsr SetPPUaddr
	lda #<$6000
	ldx #>$6000
	jsr SetSource
	ldx #$10
	ldy #0
:	lda $2007
	sta (SourcePtr),y
	iny
	bne :-
	inc SourcePtr+1
	dex
	bne :-
	rts

.segment "IRQ"
IRQ:	rti

.segment "NMI"
NMI:	php
	 pha
	  txa
	  pha
	   lda #0
	   sta $2003
	   lda #2
	   sta $4014
	   bit $2002
	   jsr StarField
	  pla
	  tax
	 pla
	plp
	rti

StarField:
	ldx #0
:	lda $201,x ;attr
	and #3
	sec
	sbc $203,x ;X
	eor #$FF
	sta $203,x
	; Hide if Y >= $48 && Y <= $D8
	;      && X >= $48 && X <= $D8
	lda $300,x
	cmp #$48
	bcc @nohide
	cmp #$D0
	bcs @nohide
	lda $203,x
	cmp #$33
	bcc @nohide
	cmp #$C3
	bcs @nohide
@hide:	lda #$F8
	sta $200,x
	bne @next
@nohide:lda $300,x
@next:	sta $200,x
	inx
	inx
	inx
	inx
	bne :-
	rts

StarFieldInit:
	ldx #0
:	; Sufficiently random data
	; Format: Y, Index, Attr, X
	lda TilePage0,x
	sta $200,x
	sta $300,x ; backup Y coordinate
	inx
	lda TilePage0,x
	and #3
	ora #$24
	sta $200,x ; index
	inx
	lda TilePage0,x
	and #$C3
	ora #$20   ; behind bg
	sta $200,x ; attr done
	inx
	lda TilePage0,x
	sta $200,x ; x done
	inx
	bne :-
	rts

.segment "TILEDATA"
TilePage0: .incbin "test_page0.bin"

.segment "NAMETABLE"
.align $100
Initial_NameTable:
	; Name table
	;      0123456789ABCDEF0123456789ABCDEF
	.byte "                                "
	.byte "                                "
	.byte "                                "
	.byte "   Tokumaru Compression Test    "
	.byte "                                "
	.byte "   Test successful              "
	.byte "                                "
	.byte "   Nametable contents:          "
	.byte "                                "
	.byte "       044444444444444441       "
	;
	.repeat 16,row
	  .repeat 7
	    .byte 0
	  .endrepeat
	  .byte $35
	  .repeat 16,column
	    .byte 16*row + column
	  .endrepeat
	  .byte $35
	  .repeat 7
	    .byte 0
	  .endrepeat
	.endrepeat
	.byte "       244444444444444443       "
	.repeat 32*(30 - 16 - 11)
	.byte 0
	.endrepeat
	; Attribute table
	.repeat 8
	 .byte $FF
	.endrepeat
	.repeat 8
	 .byte $5A
	.endrepeat
	.repeat 64-(8*32/4/4)
	.byte 0
	.endrepeat

.segment "PALETTE"
Initial_Palette:
	;BG
	.byte $0F,$14,$20,$12
	.byte $0F,$11,$21,$31
	.byte $0F,$06,$16,$26
	.byte $0F,$1A,$2A,$20
	;SP
	.byte $0F,$00,$10,$20
	.byte $0F,$02,$21,$20
	.byte $0F,$03,$00,$20
	.byte $0F,$0A,$01,$20

.segment "VECTORS"
.word $FFFF,$FFFF,$FFFF, NMI, Reset, IRQ

.segment "INES_HEADER"
.byte $4E,$45,$53,$1A ; "NES" EOF
.byte 1,0             ; 16k PRG, CHR RAM
.byte 0*$10+1         ; Mapper 0 (NROM), vertical mirroring
