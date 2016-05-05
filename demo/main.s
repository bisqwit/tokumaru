.segment "ZEROPAGE"
.exportzp SourcePtr
SourcePtr:   .res 2
NMIcounter:  .res 1
WhichSystem: .res 1
DummyByte:   .res 1

Byte1:       .res 1
Byte2:       .res 1

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
	jsr IdentifySystem
	; Populate nametable and attribute table
	lda #<$2000
	ldx #>$2000
	jsr SetPPUaddr
	lda #<Initial_NameTable
	ldx #>Initial_NameTable
	jsr SetSource
	ldx #8
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
	lda #<$1000
	ldx #>$1000
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

.include "macros.inc"
;.include "delay.inc"

.macro delay_n n
    .if (n)=2
	nop
    .elseif (n)=3
	cmp DummyByte
    .elseif (n)=5
	inc DummyByte
    .elseif (n)=7
	php
	plp
    .elseif (n)=9
	nop
	delay_n 7
    .elseif (n)=12
	jsr rts12
    .elseif (n)=14
	jsr rts14
    .elseif (n)=15
	jsr rts15
    .elseif (n)<34
	delay_n (n)/2
	delay_n (n) - (n)/2
    .elseif ((n) <= 289)
	; overhead: 7+2 = 9
	; Size: 7 bytes
	pha
	 lda #((n)-(25+9))
	 jsr delay_a_25_clocks
	pla
    .else
	; overhead: 7+7+4+4 = 22
	; Size: 13 bytes
	pha
	txa
	pha
	 lda #<((n)-(30+22))
	 ldx #>((n)-(30+22))
	 jsr delay_256x_a_30_clocks
	pla
	tax
	pla
    .endif
.endmacro


.segment "NMI"
NMI:	php	;3
	pha	;3
	txa	;2
	pha	;3
	tya	;2
	pha	;3
	 inc NMIcounter	;5
	 lda #0		;2
	 sta $2003	;4
	 lda #2		;2
	 sta $4014	;4
	 bit $2002	;4

	 lda WhichSystem	;3
	 bpl @cont		;3
	 jmp @quick_nmi
	@cont:

	 lda NMIcounter		;3
	 ;and #$40
	 ;lsr ;$20
	 lsr ;$10		;2
	 and #$10		;2
	 ora #$80		;2
	 sta Byte1		;3
	 
	 lda #$80		;2
	 sta $2000		;4

	 lda #$90		;2
	 sta Byte2		;4

	 jsr StarField		;4301

	 jsr delay_that_amount1

	 lda Byte1
	 ldx #$90

	 @ppu_cycles .set 32*8+3
	 @cpu_cycles .set 0

	 .repeat 64

	 @want       .set ((@ppu_cycles + (10*8))/3 - @cpu_cycles)
	 delay_n    @want
	 @cpu_cycles .set @cpu_cycles + @want + 4

	 ; This config for the rest of scanline
	 stx $2000

	 @want       .set ((@ppu_cycles + 341)/3 - @cpu_cycles)
	 delay_n    @want
	 
	 @cpu_cycles .set @cpu_cycles + @want + 4
	 @ppu_cycles .set @ppu_cycles + 341

	 sta $2000
	 .endrepeat

	 ;jsr delay_that_amount2
	 ;lda #$80
	 ;sta $2000
@quick_nmi:
	pla
	tay
	pla
	tax
	pla
	plp
	rti

.segment "STARFIELD"
StarField:
	; 4289 cycles + JSR+RTS
	ldx #0
:	lda $201,x ;attr			;4
	and #3					;2
	sec					;2
	sbc $203,x ;X				;4
	eor #$FF				;2
	sta $203,x				;5
	; Hide if Y >= $48 && Y <= $D8
	;      && X >= $48 && X <= $D8
	lda $300,x				;4
	sec					;2
	sbc #$48				;2
	cmp #($D0-$48)				;2
	Jcs @nohide1				;2
	lda $203,x	;4			;4
	; carry is clear
	sbc #$0B-1	;2			;2
	cmp #($E3-$0B)	;2			;2
	Jcs @nohide2	;2			;2
	;
@hide:	lda #$F8	;2			;2
	sta $200,x	;5			;5
	Jne @next	;3			;3
@nohide1:
	delay_n 10
@nohide2:
	lda $300,x	;4
	delay_n 5       ;5
@next:	sta $200,x				;5
	inx					;2
	inx					;2
	inx					;2
	inx					;2
	Jne :-					;3
	rts					;-1

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


.segment "SYSTEMDEP"
IdentifySystem:
	; Enable NMI temporarily
	lda #$80
	sta $2000
	sta WhichSystem
	ldx #0
	ldy #0
	lda NMIcounter
:	cmp NMIcounter
	beq :-
	lda NMIcounter
@wait:
	; Each iteration takes 11 cycles.
	; NTSC NES: 29780 cycles or 2707 = $A93 iterations
	; PAL NES:  33247 cycles or 3022 = $BCE iterations
	; Dendy:    35464 cycles or 3224 = $C98 iterations
	; so we can divide by $100 (rounding down), subtract ten,
	; and end up with 0=ntsc, 1=pal, 2=dendy, 3=unknown
	inx
	Jne :+
	iny
:	cmp NMIcounter
	Jeq @wait
	tya
	sec
	sbc #10
	cmp #3
	bcc :+
	lda #3
:	sta WhichSystem
	; Disable NMI again
	lda #0
	sta $2000
	rts

.segment "DELAY1"
delay_that_amount1:
	@num          .set $91
	@already_done .set (16+21+4331 + 12+25)
	;16 and 21 are first parts of NMI routine
	;4331 is the third part including StarField
	;12 is the JSR+RTS to this routine, 25 is this routine

	ldx WhichSystem	;3
	lda @ptrhi,x	;5
	pha		;3
	lda @ptrlo,x	;5
	pha		;3
	rts		;6
@ptrlo: .byte <(@sys0-1), <(@sys1-1), <(@sys2-1), <(@sys3-1)
@ptrhi: .byte >(@sys0-1), >(@sys1-1), >(@sys2-1), >(@sys3-1)
@sys0:
@sys2:
@sys3:
	@vblank .set 20
	delay_n	((341*(@vblank + @num))/3    - @already_done)
	rts
@sys1:
	@vblank .set 70
	delay_n	((341*(@vblank + @num))*5/16 - @already_done)
	rts

.segment "DELAY2"
;;;;;;;;;;;;;;;;;;;;;;;;
; Delays A clocks + overhead
; Clobbers A. Preserves X,Y.
; Time: A+25 clocks (including JSR)
;;;;;;;;;;;;;;;;;;;;;;;;
                  ;       Cycles              Accumulator         Carry flag
                  ; 0  1  2  3  4  5  6          (hex)           0 1 2 3 4 5 6
                  ;
                  ; 6  6  6  6  6  6  6   00 01 02 03 04 05 06
:      sbc #7     ; carry set by CMP
delay_a_25_clocks:
       cmp #7     ; 2  2  2  2  2  2  2   00 01 02 03 04 05 06   0 0 0 0 0 0 0
       Jcs :-     ; 2  2  2  2  2  2  2   00 01 02 03 04 05 06   0 0 0 0 0 0 0
       lsr        ; 2  2  2  2  2  2  2   00 00 01 01 02 02 03   0 1 0 1 0 1 0
       Jcs *+2    ; 2  3  2  3  2  3  2   00 00 01 01 02 02 03   0 1 0 1 0 1 0
       Jeq :+     ; 3  3  2  2  2  2  2   00 00 01 01 02 02 03   0 1 0 1 0 1 0
       lsr        ;       2  2  2  2  2         00 00 01 01 01       1 1 0 0 1
       Jeq @rts   ;       3  3  2  2  2         00 00 01 01 01       1 1 0 0 1
       Jcc @rts   ;             3  3  2               01 01 01           0 0 1
:      Jne @rts   ; 2  2              3   00 00             01   0 1         0
@rts:  rts        ; 6  6  6  6  6  6  6   00 00 00 00 01 01 01   0 1 1 1 0 0 1
; Total cycles:    25 26 27 28 29 30 31

;;;;;;;;;;;;;;;;;;;;;;;;
; Delays X*256+A clocks + overhead
; Clobbers A,X. Preserves Y.
; Depends on delay_a_25_clocks within short branch distance
; Time: X*256+A+30 clocks (including JSR)
;;;;;;;;;;;;;;;;;;;;;;;;
delay_256x_a_30_clocks:
:       cpx #0			;2
        Jeq delay_a_25_clocks	;3
        ;4 cycles done. Must consume 256 cycles; 252 cycles remain.
        pha				;3
         lda #(256-4-(3+2+4+2+3))-25	;2
         jsr delay_a_25_clocks		;238
        pla				;4
        dex				;2
        jmp :-				;3

.segment "CODE"
rts15:	jmp rts12
rts14:	nop
rts12:	rts


.segment "TILEDATA"
TilePage0: .incbin "test_page0.bin"
TilePage1: .incbin "test_page1.bin"

.segment "NAMETABLE"
.align $100
Initial_NameTable:
    .repeat 2,page
	; Name table
	;      0123456789ABCDEF0123456789ABCDEF
	.byte "                                "
	.byte "                                "
	.byte "                                "
	.byte "   Tokumaru Compression Test    "
	.byte "                                "
	.byte "   Test successful              "
	.byte "                                "
	.byte "   CHR-RAM contents:            "
	.byte "                                "
	.byte "  044444444444444444444444441   "
	;
	.repeat 2,pgnum
	.repeat 8,row
	  .repeat 2
	    .byte 0
	  .endrepeat
	  .byte $35
	  .repeat 24,column
	    .if column=16
	      .byte 0
	    .endif
	    .if pgnum=0
	      .if column<16
	       .byte 16*(row) + column
	      .else
	       .byte 16*(8+row) + column-16
	      .endif
	    .else
	      .if column<8
	       .byte 16*(row) + column+8
	      .elseif column<16
	       .byte 16*(8+row) + column-8
	      .else
	       .byte 16*(8+row) + column-16+8
	      .endif
	    .endif
	  .endrepeat
	  .byte $35
	  .repeat 3
	    .byte 0
	  .endrepeat
	.endrepeat
	.endrepeat
	.byte "  244444444444444444444444443  "

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
    .endrepeat

.segment "PALETTE"
Initial_Palette:
	;BG
	.byte $0F,$17,$3D,$12
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
