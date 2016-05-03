; delay_n, delays n cycles with small code
; Will clobber Z+N, C, D, and V
; Will preserve A, X, Y, S, and I (interrupt-disable flag)
;
; Requires that a label @nearby_rts is defined,
; that points _somewhere_ that contains a $60 byte (RTS).
;
; Requires that a label @nearby_rts_14cyc is defined,
; that points _somewhere_ that contains a dummy 2-cycle
; opcode, such as NOP or LDA #1, followed by a $60 byte (RTS).
;
; Requires that a label @nearby_rts_15cyc is defined,
; that points _somewhere_ that contains a JMP to a location that does RTS.

; Requires a disposable zeropage variable @zptemp.
;
; Generated code is:
; - 1 bytes or less when n <= 2
; - 2 bytes or less when n <= 5
; - 3 bytes or less when n <= 7
; - 4 bytes or less when n <= 12
; - 5 bytes or less when n <= 22
; - 6 bytes or less when n <= 24
; - 7 bytes or less when n <= 33
; - 8 bytes or less when n <= 38
; - 9 bytes or less when n <= 53
; -10 bytes or less when n <= 150
; -11 bytes or less when n <= 700
; Detailed summary:
; - When 2 <= n <= 10: min=1, avg=2.6, max=4
; - When 11 <= n <= 20: min=3, avg=4.1, max=5
; - When 21 <= n <= 30: min=5, avg=5.9, max=7
; - When 31 <= n <= 50: min=6, avg=7.7, max=9
; - When 51 <= n <= 75: min=7, avg=8.6, max=10
; - When 76 <= n <= 100: min=8, avg=9.2, max=10
; - When 101 <= n <= 200: min=7, avg=9.3, max=11
; - When 201 <= n <= 300: min=8, avg=9.6, max=11
; - When 301 <= n <= 500: min=8, avg=9.7, max=11
; - When 501 <= n <= 700: min=8, avg=9.8, max=11
; Written by Joel Yliluoma


.macro delay_n n, rest
.assert (n = 0 || n >= 2), error, "delay_n: delay must be 0 or >= 2"
.if(n = 0)
.elseif(n < 0)
	.error .sprintf("delay_n: n cannot be < 0, you gave %d", n)
.elseif(n > 700)
	delay_n (n/2), { delay_n (n-(n/2)) }
.else
.if((n >= 1) && (n < 351))
.if((n >= 1) && (n < 176))
.if((n >= 1) && (n < 88))
.if((n >= 1) && (n < 44))
.if((n >= 1) && (n < 22))
; 1 to 21
.if(n = 1)
	.error "delay_n: 1-cycle delay cannot be implemented"
.elseif(n = 2)
	; 1 bytes
	nop
.elseif(n = 3)
	; 2 bytes
	cmp $C5
.elseif(n = 4 || n = 6 || (n > 15 && n < 19))
	; 2..5 bytes
	delay_n (n-2)
	nop
.elseif(n = 5)
	; 2 bytes
	inc @zptemp
.elseif(n = 7)
	; 2 bytes
	php
	plp
.elseif(n = 8)
	; 4 bytes
	.repeat 4
	 nop
	.endrepeat
.elseif(n = 9 || n = 11 || n = 13 || n = 19 || n = 21)
	; 3..5 bytes
	delay_n (n-7)
	delay_n 7
.elseif(n = 10)
	; 4 bytes
	php
	cmp $C5
	plp
.elseif(n = 12)
	; 3 bytes
	jsr @nearby_rts
.elseif(n = 14)
	; 3 bytes
	jsr @nearby_rts_14cyc
.elseif(n = 15)
	; 3 bytes
	jsr @nearby_rts_15cyc
.elseif(n = 20)
	; 5 bytes
	delay_n (n-15)
	jsr @nearby_rts_15cyc
.endif
.else
; 22 to 43
.if(n = 22 || n = 23 || n = 37)
	; 5..8 bytes
	delay_n (n-7)
	delay_n 7
.elseif(n = 24)
	; 6 bytes
	delay_n (n-12)
	jsr @nearby_rts
.elseif(n = 25 || n = 27 || n = 29 || n = 30)
	; 6..7 bytes
	delay_n (n-15)
	jsr @nearby_rts_15cyc
.elseif(n = 26 || n = 28)
	; 6 bytes
	delay_n (n-14)
	jsr @nearby_rts_14cyc
.elseif(n = 31)
	; 6 bytes
	pha
	lda #$0A ;hides 'asl a'
	Jpl *-1
	pla
.elseif(n = 32)
	; 7 bytes
	delay_n (n-2)
	nop
.elseif(n = 33)
	; 7 bytes
	pha
	clc
	lda #$0A ;hides 'asl a'
	Jpl *-1
	pla
.elseif(n = 34)
	; 8 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 35)
	; 8 bytes
	pha
	clc
	clc
	lda #$0A ;hides 'asl a'
	Jpl *-1
	pla
.elseif(n = 36)
	; 8 bytes
	pha
	clc
	lda #$2A ;hides 'rol a'
	nop
	Jcc *-2
	pla
.elseif(n = 38)
	; 7 bytes
	pha
	clc
	lda #$0A ;hides 'asl a'
	Jcc *-1
	pla
.elseif(n = 39)
	; 9 bytes
	pha
	lda #$2A ;hides 'rol a'
	delay_n 6
	Jpl *-4
	pla
.elseif(n = 40)
	; 8 bytes
	pha
	clc
	clc
	lda #$0A ;hides 'asl a'
	Jcc *-1
	pla
.elseif(n = 41)
	; 7 bytes
	pha
	lda #$0A ;hides 'asl a'
	sec
	Jpl *-2
	pla
.elseif(n = 42)
	; 8 bytes
	pha
	lda #$2A ;hides 'rol a'
	delay_n 7
	Jpl *-3
	pla
.elseif(n = 43)
	; 7 bytes
	pha
	lda #$E9 ;hides 'sbc #$2A'
	rol a ;first loop only
	Jcs *-2
	pla
.endif
.endif
.else
.if((n >= 44) && (n < 66))
; 44 to 65
.if(n = 44 || n = 49)
	; 8..9 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 45)
	; 8 bytes
	pha
	sec
	lda #$69 ;hides 'adc #$EA'
	nop ;first loop only
	Jpl *-2
	pla
.elseif(n = 46)
	; 6 bytes
	pha
	lda #$4A ;hides 'lsr a'
	Jne *-1
	pla
.elseif(n = 47 || n = 52)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 48)
	; 7 bytes
	pha
	sec
	lda #$0A ;hides 'asl a'
	Jne *-1
	pla
.elseif(n = 50)
	; 8 bytes
	pha
	sec
	sec
	lda #$4A ;hides 'lsr a'
	Jne *-1
	pla
.elseif(n = 51)
	; 8 bytes
	pha
	lda #$0A ;hides 'asl a'
	delay_n 4
	Jpl *-3
	pla
.elseif(n = 53 || n = 65)
	; 8..9 bytes
	php
	delay_n (n-7)
	plp
.elseif(n = 54)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 55)
	; 8 bytes
	sty @zptemp
	ldy #136 ;hides 'dey'
	Jmi *-1
	ldy @zptemp
.elseif(n = 56)
	; 9 bytes
	pha
	clc
	lda #$2A ;hides 'rol a'
	delay_n 7
	Jcc *-3
	pla
.elseif(n = 57)
	; 8 bytes
	pha
	lda #$E9 ;hides 'sbc #$2A'
	rol a ;first loop only
	nop
	Jcs *-3
	pla
.elseif(n = 58)
	; 7 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	Jmi *-2
	pla
.elseif(n = 59)
	; 9 bytes
	pha
	sec
	lda #$69 ;hides 'adc #$EA'
	nop ;first loop only
	nop
	Jpl *-3
	pla
.elseif(n = 60)
	; 8 bytes
	pha
	tya
	ldy #136 ;hides 'dey'
	Jmi *-1
	tay
	pla
.elseif(n = 61)
	; 9 bytes
	pha
	lda #$0A ;hides 'asl a'
	delay_n 6
	Jpl *-4
	pla
.elseif(n = 62)
	; 7 bytes
	pha
	lda #$4A ;hides 'lsr a'
	clc
	Jne *-2
	pla
.elseif(n = 63)
	; 9 bytes
	delay_n (n-5)
	inc @zptemp
.elseif(n = 64)
	; 8 bytes
	pha
	clc
	lda #$2A ;hides 'rol a'
	clc
	Jne *-2
	pla
.endif
.else
; 66 to 87
.if(n = 66)
	; 8 bytes
	pha
	lda #$0A ;hides 'asl a'
	delay_n 7
	Jpl *-3
	pla
.elseif(n = 67 || n = 82)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 68)
	; 9 bytes
	pha
	clc
	lda #$0A ;hides 'asl a'
	delay_n 7
	Jpl *-3
	pla
.elseif(n = 69 || n = 73)
	; 9..10 bytes
	php
	delay_n (n-7)
	plp
.elseif(n = 70)
	; 8 bytes
	pha
	lda #$4A ;hides 'lsr a'
	sta @zptemp
	Jne *-3
	pla
.elseif(n = 71)
	; 9 bytes
	pha
	lda #$E9 ;hides 'sbc #$2A'
	rol a ;first loop only
	delay_n 4
	Jcs *-4
	pla
.elseif(n = 72 || n = 77 || n = 87)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 74 || n = 79 || n = 84)
	; 10 bytes
	delay_n (n-2)
	nop
.elseif(n = 75)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 76)
	; 9 bytes
	pha
	lda #$0A ;hides 'asl a'
	delay_n 7
	sec
	Jpl *-4
	pla
.elseif(n = 78)
	; 8 bytes
	pha
	lda #$4A ;hides 'lsr a'
	delay_n 4
	Jne *-3
	pla
.elseif(n = 80)
	; 9 bytes
	pha
	tya
	ldy #136 ;hides 'dey'
	nop
	Jmi *-2
	tay
	pla
.elseif(n = 81)
	; 10 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 83)
	; 10 bytes
	delay_n (n-5)
	inc @zptemp
.elseif(n = 85)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 86)
	; 9 bytes
	pha
	lda #$4A ;hides 'lsr a'
	clv
	Jvc *+2
	Jne *-4
	pla
.endif
.endif
.endif
.else
.if((n >= 88) && (n < 132))
.if((n >= 88) && (n < 110))
; 88 to 109
.if(n = 88)
	; 9 bytes
	pha
	lda #$69 ;hides 'adc #$18'
	clc ;first loop only
	inc @zptemp
	Jcc *-4
	pla
.elseif(n = 89)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 90)
	; 8 bytes
	pha
	sec
	lda #$69 ;hides 'adc #$F8'
	sed ;first loop only
	Jne *-2
	pla
.elseif(n = 91)
	; 9 bytes
	pha
	lda #$0A ;hides 'asl a'
	jsr @nearby_rts
	Jpl *-4
	pla
.elseif(n = 92 || n = 97 || n = 107)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 93)
	; 10 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 94)
	; 9 bytes
	pha
	lda #$4A ;hides 'lsr a'
	delay_n 6
	Jne *-4
	pla
.elseif(n = 95)
	; 8 bytes
	pha
	sec
	lda #$69 ;hides 'adc #$F8'
	sed ;first loop only
	Jcs *-2
	pla
.elseif(n = 96 || n = 103)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 98)
	; 9 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 4
	Jmi *-4
	pla
.elseif(n = 99)
	; 10 bytes
	delay_n (n-2)
	nop
.elseif(n = 100)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 101)
	; 9 bytes
	pha
	lda #$0A ;hides 'asl a'
	jsr @nearby_rts_14cyc
	Jpl *-4
	pla
.elseif(n = 102)
	; 8 bytes
	pha
	lda #$4A ;hides 'lsr a'
	delay_n 7
	Jne *-3
	pla
.elseif(n = 104)
	; 9 bytes
	pha
	sec
	lda #$0A ;hides 'asl a'
	delay_n 7
	Jne *-3
	pla
.elseif(n = 105)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 106)
	; 9 bytes
	pha
	lda #$0A ;hides 'asl a'
	jsr @nearby_rts_15cyc
	Jpl *-4
	pla
.elseif(n = 108)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.elseif(n = 109)
	; 10 bytes
	php
	delay_n (n-7)
	plp
.endif
.else
; 110 to 131
.if(n = 110)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 111)
	; 10 bytes
	pha
	lda #$0A ;hides 'asl a'
	jsr @nearby_rts_14cyc
	sec
	Jpl *-5
	pla
.elseif(n = 112 || n = 122 || n = 127)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 113)
	; 10 bytes
	sty @zptemp
	ldy #24 ;hides 'clc'
	dey
	dey
	Jne *-3
	ldy @zptemp
.elseif(n = 114 || n = 119)
	; 10 bytes
	delay_n (n-2)
	nop
.elseif(n = 115)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 116)
	; 10 bytes
	pha
	lda #$0A ;hides 'asl a'
	jsr @nearby_rts_15cyc
	sec
	Jpl *-5
	pla
.elseif(n = 117)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 118)
	; 7 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	Jne *-2
	pla
.elseif(n = 120)
	; 8 bytes
	pha
	sec
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	Jne *-2
	pla
.elseif(n = 121 || n = 126)
	; 9 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 123)
	; 7 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	Jcs *-2
	pla
.elseif(n = 124 || n = 131)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 125)
	; 8 bytes
	pha
	sec
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	Jcs *-2
	pla
.elseif(n = 128)
	; 9 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 7
	Jmi *-4
	pla
.elseif(n = 129)
	; 9 bytes
	pha
	sec
	lda #$69 ;hides 'adc #$F8'
	sed ;first loop only
	nop
	Jcs *-3
	pla
.elseif(n = 130)
	; 8 bytes
	stx @zptemp
	ldx #232 ;hides 'inx'
	Jne *-1
	ldx @zptemp
.endif
.endif
.else
.if((n >= 132) && (n < 154))
; 132 to 153
.if(n = 132 || n = 152)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 133)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 134 || n = 139 || n = 144 || n = 149 || n = 151)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 135)
	; 8 bytes
	pha
	txa
	ldx #232 ;hides 'inx'
	Jne *-1
	tax
	pla
.elseif(n = 136)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.elseif(n = 137 || n = 142 || n = 147)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 138)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 140 || n = 145)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 141)
	; 10 bytes
	pha
	txa
	ldx #232 ;hides 'inx'
	Jne *-1
	inc @zptemp,x
	tax
	pla
.elseif(n = 143)
	; 10 bytes
	pha
	sec
	lda #(n-17)/7
	nop
	sbc #1
	Jpl *-3
	pla
.elseif(n = 146)
	; 10 bytes
	pha
	sec
	lda #$69 ;hides 'adc #$F8'
	sed ;first loop only
	sta @zptemp
	Jcs *-4
	pla
.elseif(n = 148)
	; 10 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 9
	Jmi *-5
	pla
.elseif(n = 150)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 153)
	; 11 bytes
	pha
	tya
	ldy #24 ;hides 'clc'
	nop
	dey
	dey
	Jpl *-4
	tay
	pla
.endif
.else
; 154 to 175
.if(n = 154 || n = 156 || n = 174)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 155 || n = 160)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 157 || n = 172)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 158)
	; 9 bytes
	pha
	lda #$4A ;hides 'lsr a'
	jsr @nearby_rts_14cyc
	Jne *-4
	pla
.elseif(n = 159)
	; 9 bytes
	sty @zptemp
	ldy #234 ;hides 'nop'
	iny
	Jne *-2
	ldy @zptemp
.elseif(n = 161)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 162)
	; 8 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	nop
	Jne *-3
	pla
.elseif(n = 163)
	; 7 bytes
	pha
	lda #$69 ;hides 'adc #$38'
	sec ;first loop only
	Jne *-2
	pla
.elseif(n = 164)
	; 9 bytes
	pha
	tya
	ldy #234 ;hides 'nop'
	iny
	Jne *-2
	tay
	pla
.elseif(n = 165)
	; 8 bytes
	pha
	clc
	lda #$69 ;hides 'adc #$38'
	sec ;first loop only
	Jne *-2
	pla
.elseif(n = 166)
	; 9 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 167)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 168)
	; 9 bytes
	delay_n (n-5)
	inc @zptemp
.elseif(n = 169)
	; 8 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	nop
	Jcs *-3
	pla
.elseif(n = 170)
	; 9 bytes
	php
	delay_n (n-7)
	plp
.elseif(n = 171)
	; 9 bytes
	pha
	sec
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	nop
	Jcs *-3
	pla
.elseif(n = 173)
	; 9 bytes
	sty @zptemp
	ldy #24 ;hides 'clc'
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 175)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.endif
.endif
.endif
.endif
.else
.if((n >= 176) && (n < 263))
.if((n >= 176) && (n < 219))
; 176 to 218
.if(n = 176)
	; 10 bytes
	php
	delay_n (n-7)
	plp
.elseif(n = 177 || n = 182 || n = 192 || n = 202 || n = 212 || n = 217)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 178)
	; 9 bytes
	pha
	tya
	ldy #24 ;hides 'clc'
	dey
	Jne *-2
	tay
	pla
.elseif(n = 179 || n = 189 || n = 191 || n = 199 || n = 204 || n = 209 || n = 211 || n = 214 || n = 216)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 180 || n = 190 || n = 210)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 181)
	; 10 bytes
	pha
	tya
	ldy #200 ;hides 'iny'
	iny
	iny
	Jne *-3
	tay
	pla
.elseif(n = 183)
	; 11 bytes
	pha
	tya
	ldy #(n-21)/9
	delay_n 4
	dey
	Jpl *-3
	tay
	pla
.elseif(n = 184)
	; 9 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	sta @zptemp
	Jne *-4
	pla
.elseif(n = 185)
	; 9 bytes
	pha
	tya
	ldy #24 ;hides 'clc'
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 186)
	; 10 bytes
	pha
	sec
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	sta @zptemp
	Jne *-4
	pla
.elseif(n = 187 || n = 197 || n = 207)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 188)
	; 11 bytes
	pha
	lda $A5
	tya
	ldy #24 ;hides 'clc'
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 193)
	; 11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 194 || n = 201)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 195 || n = 200 || n = 215)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 196)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 198)
	; 10 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	jsr @nearby_rts_14cyc
	Jmi *-5
	pla
.elseif(n = 203)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 205)
	; 8 bytes
	pha
	clc
	lda #$69 ;hides 'adc #$6A'
	ror a ;first loop only
	Jne *-2
	pla
.elseif(n = 206)
	; 9 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 4
	Jne *-4
	pla
.elseif(n = 208)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 213)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.elseif(n = 218)
	; 11 bytes
	pha
	sec
	lda #26
	bit $24
	sbc #1
	Jne *-4
	pla
.endif
.else
; 219 to 262
.if(n = 219 || n = 224 || n = 234 || n = 239 || n = 244 || n = 246 || n = 249 || n = 251 || n = 254 || n = 259)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 220 || n = 230 || n = 240)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 221)
	; 10 bytes
	sty @zptemp
	ldy #24 ;hides 'clc'
	nop
	dey
	Jne *-3
	ldy @zptemp
.elseif(n = 222 || n = 227 || n = 232 || n = 237 || n = 257)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 223 || n = 228 || n = 233 || n = 253)
	; 10..11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 225)
	; 8 bytes
	pha
	lda #$69 ;hides 'adc #$38'
	sec ;first loop only
	nop
	Jne *-3
	pla
.elseif(n = 226)
	; 10 bytes
	pha
	tya
	ldy #24 ;hides 'clc'
	nop
	dey
	Jne *-3
	tay
	pla
.elseif(n = 229)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 231)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 235 || n = 245 || n = 255 || n = 260)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 236 || n = 243)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 238)
	; 9 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	inc @zptemp
	Jcs *-4
	pla
.elseif(n = 241)
	; 10 bytes
	pha
	sec
	lda #(n-17)/7
	nop
	sbc #1
	Jpl *-3
	pla
.elseif(n = 242 || n = 247 || n = 252 || n = 262)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 248)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.elseif(n = 250)
	; 8 bytes
	pha
	sec
	lda #$E9 ;hides 'sbc #$B8'
	clv ;first loop only
	Jne *-2
	pla
.elseif(n = 256)
	; 9 bytes
	pha
	lda #$69 ;hides 'adc #$38'
	sec ;first loop only
	sta @zptemp
	Jne *-4
	pla
.elseif(n = 258)
	; 10 bytes
	pha
	clc
	lda #$69 ;hides 'adc #$38'
	sec ;first loop only
	sta @zptemp
	Jne *-4
	pla
.elseif(n = 261)
	; 10 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 6
	Jcs *-5
	pla
.endif
.endif
.else
.if((n >= 263) && (n < 307))
; 263 to 306
.if(n = 263 || n = 293 || n = 303)
	; 10..11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 264 || n = 278 || n = 299)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 265 || n = 285 || n = 300)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 266 || n = 301)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 267 || n = 272 || n = 282 || n = 287 || n = 297)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 268)
	; 11 bytes
	pha
	tya
	ldy #(n-20)/8
	cmp $C5
	dey
	Jpl *-3
	tay
	pla
.elseif(n = 269 || n = 274 || n = 279 || n = 281 || n = 289 || n = 294 || n = 296 || n = 304)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 270 || n = 275 || n = 280 || n = 305)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 271)
	; 9 bytes
	stx @zptemp
	ldx #202 ;hides 'dex'
	dex
	Jmi *-2
	ldx @zptemp
.elseif(n = 273)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 276)
	; 9 bytes
	pha
	txa
	ldx #202 ;hides 'dex'
	dex
	Jmi *-2
	tax
	pla
.elseif(n = 277 || n = 292 || n = 302)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 283)
	; 9 bytes
	pha
	clc
	lda #$69 ;hides 'adc #$6A'
	ror a ;first loop only
	nop
	Jne *-3
	pla
.elseif(n = 284)
	; 9 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 7
	Jcs *-4
	pla
.elseif(n = 286)
	; 10 bytes
	pha
	sec
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 7
	Jcs *-4
	pla
.elseif(n = 288)
	; 9 bytes
	pha
	clc
	lda #216 ;hides 'cld'
	adc #1
	Jne *-3
	pla
.elseif(n = 290)
	; 8 bytes
	pha
	sec
	lda #$69 ;hides 'adc #$D8'
	cld ;first loop only
	Jne *-2
	pla
.elseif(n = 291)
	; 10 bytes
	pha
	tya
	ldy #36 ;hides 'bit zp'
	dey ;first loop only
	dey
	Jne *-3
	tay
	pla
.elseif(n = 295)
	; 8 bytes
	pha
	tya
	ldy #200 ;hides 'iny'
	Jmi *-1
	tay
	pla
.elseif(n = 298)
	; 10 bytes
	pha
	lda $A5
	tya
	ldy #200 ;hides 'iny'
	Jmi *-1
	tay
	pla
.elseif(n = 306)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.endif
.else
; 307 to 350
.if(n = 307 || n = 332 || n = 337 || n = 342 || n = 347)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 308)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 309 || n = 314 || n = 319 || n = 321 || n = 324 || n = 326 || n = 329 || n = 331 || n = 339 || n = 344 || n = 349)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 310 || n = 315 || n = 335 || n = 340 || n = 350)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 311)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.elseif(n = 312 || n = 317 || n = 322 || n = 327)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 313)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 316)
	; 10 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	delay_n 9
	Jne *-5
	pla
.elseif(n = 318)
	; 10 bytes
	pha
	sec
	lda #(n-17)/7
	nop
	sbc #1
	Jpl *-3
	pla
.elseif(n = 320 || n = 325 || n = 330 || n = 345)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 323 || n = 328 || n = 333)
	; 11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 334 || n = 341 || n = 348)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 336 || n = 343)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 338)
	; 11 bytes
	pha
	sec
	lda #(n-18)/8
	bit $24
	sbc #1
	Jpl *-4
	pla
.elseif(n = 346)
	; 9 bytes
	pha
	sec
	lda #$E9 ;hides 'sbc #$B8'
	clv ;first loop only
	nop
	Jne *-3
	pla
.endif
.endif
.endif
.endif
.else
.if((n >= 351) && (n < 526))
.if((n >= 351) && (n < 438))
; 351 to 437
.if(n = 351 || n = 354 || n = 356 || n = 359 || n = 364 || n = 366 || n = 374 || n = 379 || n = 384 || n = 386 || n = 389 || n = 391 || n = 394 || n = 396 || n = 399 || n = 414 || n = 419 || n = 421 || n = 424 || n = 429 || n = 431 || n = 434 || n = 436)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 352 || n = 357 || n = 372 || n = 392 || n = 407 || n = 417 || n = 422 || n = 432)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 353 || n = 381 || n = 423)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.elseif((n .mod 5 = 0) && (n = 355 || (n > 364 && n < 381) || (n > 394 && n < 426)))
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 358 || n = 373 || n = 398 || n = 403 || n = 433)
	; 11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 360 || n = 430 || n = 435)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 361)
	; 10 bytes
	pha
	clc
	lda #$69 ;hides 'adc #$6A'
	ror a ;first loop only
	delay_n 4
	Jne *-4
	pla
.elseif(n = 362 || n = 367 || n = 377 || n = 382 || n = 387 || n = 397 || n = 402 || n = 412 || n = 427 || n = 437)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 363 || n = 408)
	; 11 bytes
	pha
	tya
	ldy #(n-12)/9
	delay_n 4
	dey
	Jne *-3
	tay
	pla
.elseif(n = 368)
	; 10 bytes
	pha
	clc
	lda #216 ;hides 'cld'
	nop
	adc #1
	Jne *-4
	pla
.elseif(n = 369 || n = 376 || n = 411)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 371)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 378 || n = 413)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 383 || n = 418)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 385)
	; 8 bytes
	stx @zptemp
	ldx #202 ;hides 'dex'
	Jmi *-1
	ldx @zptemp
.elseif(n = 388)
	; 10 bytes
	pha
	tya
	ldy #213 ;hides 'cmp zp,x'
	iny ;first loop only
	iny
	Jne *-3
	tay
	pla
.elseif(n = 390)
	; 8 bytes
	pha
	txa
	ldx #202 ;hides 'dex'
	Jmi *-1
	tax
	pla
.elseif(n = 393)
	; 10 bytes
	pha
	lda $A5
	txa
	ldx #202 ;hides 'dex'
	Jmi *-1
	tax
	pla
.elseif(n = 401)
	; 10 bytes
	sty @zptemp
	ldy #213 ;hides 'cmp zp,x'
	dey ;first loop only
	iny
	Jne *-3
	ldy @zptemp
.elseif(n = 404)
	; 9 bytes
	sty @zptemp
	ldy #56 ;hides 'sec'
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 406)
	; 10 bytes
	pha
	tya
	ldy #213 ;hides 'cmp zp,x'
	dey ;first loop only
	iny
	Jne *-3
	tay
	pla
.elseif(n = 409)
	; 9 bytes
	pha
	tya
	ldy #56 ;hides 'sec'
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 416)
	; 10 bytes
	pha
	sec
	lda #(n-17)/7
	nop
	sbc #1
	Jpl *-3
	pla
.elseif(n = 426)
	; 10 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	jsr @nearby_rts_14cyc
	Jne *-5
	pla
.elseif(n = 428)
	; 11 bytes
	pha
	txa
	ldx #202 ;hides 'dex'
	delay_n 4
	dex
	Jmi *-4
	tax
	pla
.endif
.else
; 438 to 525
.if(n = 438 || n = 443 || n = 463 || n = 503 || n = 513)
	; 11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 439 || n = 446 || n = 453 || n = 488 || n = 516)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 440 || n = 445 || n = 450 || n = 455 || n = 465 || n = 475 || n = 480 || n = 490 || n = 510 || n = 515)
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 441 || n = 448 || n = 511)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 442 || n = 447 || n = 452 || n = 457 || n = 472 || n = 482 || n = 497 || n = 507)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 444 || n = 449 || n = 454 || n = 456 || n = 459 || n = 461 || n = 464 || n = 469 || n = 471 || n = 479 || n = 484 || n = 489 || n = 494 || n = 499 || n = 501 || n = 504 || n = 506 || n = 519 || n = 524)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 451 || n = 458)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.elseif(n = 460 || n = 470 || n = 485 || n = 495 || n = 500 || n = 505 || n = 520 || n = 525)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 462 || n = 467 || n = 477 || n = 487 || n = 492 || n = 502 || n = 512 || n = 517 || n = 522)
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 466)
	; 10 bytes
	pha
	tya
	ldy #200 ;hides 'iny'
	sta @zptemp
	Jmi *-3
	tay
	pla
.elseif(n = 468)
	; 10 bytes
	pha
	lda #$E9 ;hides 'sbc #$0A'
	asl a ;first loop only
	jsr @nearby_rts_15cyc
	Jcs *-5
	pla
.elseif(n = 473)
	; 10 bytes
	sty @zptemp
	ldy #24 ;hides 'clc'
	iny
	iny
	Jpl *-3
	ldy @zptemp
.elseif(n = 474 || n = 481 || n = 523)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 476 || n = 518)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 478)
	; 10 bytes
	pha
	tya
	ldy #24 ;hides 'clc'
	iny
	iny
	Jpl *-3
	tay
	pla
.elseif(n = 483)
	; 10 bytes
	pha
	tya
	ldy #197 ;hides 'cmp zp'
	nop
	iny
	Jne *-3
	tay
	pla
.elseif(n = 486 || n = 493 || n = 521)
	; 10 bytes
	pha
	sec
	lda #(n-17)/7
	nop
	sbc #1
	Jpl *-3
	pla
.elseif(n = 491)
	; 10 bytes
	pha
	tya
	ldy #197 ;hides 'cmp zp'
	dey ;first loop only
	iny
	Jne *-3
	tay
	pla
.elseif(n = 496)
	; 10 bytes
	pha
	tya
	ldy #234 ;hides 'nop'
	dey
	dey
	Jmi *-3
	tay
	pla
.elseif(n = 498)
	; 11 bytes
	pha
	tya
	ldy #(n-12)/9
	delay_n 4
	dey
	Jne *-3
	tay
	pla
.elseif(n = 508)
	; 11 bytes
	pha
	tya
	ldy #62
	cmp $C5
	dey
	Jne *-3
	tay
	pla
.elseif(n = 509)
	; 9 bytes
	sty @zptemp
	ldy #56 ;hides 'sec'
	iny
	Jpl *-2
	ldy @zptemp
.elseif(n = 514)
	; 9 bytes
	pha
	tya
	ldy #56 ;hides 'sec'
	iny
	Jpl *-2
	tay
	pla
.endif
.endif
.else
.if((n >= 526) && (n < 613))
; 526 to 612
.if(n = 526 || n = 529 || n = 531 || n = 534 || n = 536 || n = 539 || n = 541 || n = 549 || n = 554 || n = 559 || n = 564 || n = 569 || n = 573 || n = 574 || n = 584 || n = 589 || n = 594 || n = 596 || n = 599 || n = 601 || n = 604 || n = 609 || n = 611)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif(n = 527 || n = 577 || n = 582 || n = 587 || n = 592 || n = 602)
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 528 || n = 556 || n = 591)
	; 10 bytes
	pha
	sec
	lda #(n-17)/7
	nop
	sbc #1
	Jpl *-3
	pla
.elseif((n .mod 5 = 0) && (n = 530 || (n > 544 && n < 561) || n = 575 || (n > 589 && n < 601) || n = 610))
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif((n .mod 5 = 2) && ((n > 531 && n < 573) || n = 597 || n = 607 || n = 612))
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 533 || n = 568 || n = 583 || n = 603 || n = 608)
	; 11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 535 || n = 540 || n = 565 || n = 570 || n = 580 || n = 585 || n = 605)
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 538 || n = 578)
	; 11 bytes
	pha
	sec
	lda #(n-18)/8
	bit $24
	sbc #1
	Jpl *-4
	pla
.elseif(n = 543 || n = 606)
	; 11 bytes
	pha
	tya
	ldy #(n-21)/9
	delay_n 4
	dey
	Jpl *-3
	tay
	pla
.elseif(n = 544 || n = 579 || n = 586)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif(n = 546 || n = 553 || n = 588)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif(n = 548)
	; 11 bytes
	pha
	tya
	ldy #(n-20)/8
	cmp $C5
	dey
	Jpl *-3
	tay
	pla
.elseif(n = 551 || n = 558 || n = 593)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif(n = 561)
	; 11 bytes
	pha
	tya
	ldy #(n-12)/9
	delay_n 4
	dey
	Jne *-3
	tay
	pla
.elseif(n = 563)
	; 10 bytes
	pha
	tya
	ldy #197 ;hides 'cmp zp'
	dey ;first loop only
	dey
	Jmi *-3
	tay
	pla
.elseif(n = 566)
	; 10 bytes
	sty @zptemp
	ldy #197 ;hides 'cmp zp'
	nop
	dey
	Jmi *-3
	ldy @zptemp
.elseif(n = 571)
	; 10 bytes
	pha
	tya
	ldy #197 ;hides 'cmp zp'
	nop
	dey
	Jmi *-3
	tay
	pla
.elseif(n = 576)
	; 11 bytes
	pha
	tya
	ldy #n/12-2
	delay_n 7
	dey
	Jpl *-3
	tay
	pla
.elseif(n = 581)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 598)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.endif
.else
; 613 to 700
.if(n = 613 || n = 643 || n = 673 || n = 683 || n = 688)
	; 11 bytes
	delay_n (n-3)
	cmp $C5
.elseif(n = 614 || n = 621 || n = 656 || n = 691)
	; 10 bytes
	pha
	tya
	ldy #(n-19)/7
	nop
	dey
	Jpl *-2
	tay
	pla
.elseif((n .mod 5 = 0) && ((n > 614 && n < 646) || n = 655))
	; 9 bytes
	pha
	sec
	lda #n/5-3
	sbc #1
	Jpl *-2
	pla
.elseif(n = 616 || n = 623 || n = 651 || n = 686)
	; 10 bytes
	sty @zptemp
	ldy #n/7-2
	nop
	dey
	Jpl *-2
	ldy @zptemp
.elseif((n .mod 5 = 2) && (n = 617 || n = 642 || (n > 661 && n < 698)))
	; 9 bytes
	pha
	tya
	ldy #(n-12)/5
	dey
	Jne *-1
	tay
	pla
.elseif(n = 618)
	; 10 bytes
	pha
	txa
	ldx #202 ;hides 'dex'
	sta @zptemp
	Jmi *-3
	tax
	pla
.elseif(n = 619 || n = 624 || n = 629 || n = 634 || n = 639 || n = 641 || n = 644 || n = 646 || n = 654 || n = 659 || n = 664 || n = 666 || n = 669 || n = 671 || n = 674 || n = 676 || n = 679 || n = 681 || n = 689 || n = 694 || n = 699)
	; 10..11 bytes
	delay_n (n-2)
	nop
.elseif((n .mod 5 = 2) && ((n > 621 && n < 638) || (n > 646 && n < 658)))
	; 9 bytes
	pha
	tya
	ldy #(n-17)/5
	dey
	Jpl *-1
	tay
	pla
.elseif(n = 626 || n = 661 || n = 668)
	; 10 bytes
	pha
	sec
	lda #(n-17)/7
	nop
	sbc #1
	Jpl *-3
	pla
.elseif(n = 628)
	; 9 bytes
	sty @zptemp
	ldy #216 ;hides 'cld'
	dey
	Jmi *-2
	ldy @zptemp
.elseif(n = 631)
	; 9 bytes
	pha
	sec
	lda #216 ;hides 'cld'
	sbc #1
	Jmi *-3
	pla
.elseif(n = 633)
	; 9 bytes
	pha
	tya
	ldy #216 ;hides 'cld'
	dey
	Jmi *-2
	tay
	pla
.elseif(n = 636)
	; 11 bytes
	pha
	tya
	ldy #52
	delay_n 7
	dey
	Jne *-3
	tay
	pla
.elseif(n = 638)
	; 11 bytes
	php
	delay_n (n-7)
	plp
.elseif(n = 648)
	; 11 bytes
	pha
	tya
	ldy #n/12-2
	delay_n 7
	dey
	Jpl *-3
	tay
	pla
.elseif(n = 649 || n = 663 || n = 684 || n = 698)
	; 10 bytes
	pha
	tya
	ldy #(n-12)/7
	nop
	dey
	Jne *-2
	tay
	pla
.elseif((n .mod 5 = 0) && (n = 650 || (n > 659 && n < 686) || n = 700))
	; 9 bytes
	pha
	sec
	lda #n/5-2
	sbc #1
	Jne *-2
	pla
.elseif(n = 653)
	; 10 bytes
	sty @zptemp
	ldy #56 ;hides 'sec'
	nop
	iny
	Jpl *-3
	ldy @zptemp
.elseif(n = 658)
	; 10 bytes
	pha
	tya
	ldy #56 ;hides 'sec'
	nop
	iny
	Jpl *-3
	tay
	pla
.elseif(n = 678)
	; 11 bytes
	pha
	tya
	ldy #(n-21)/9
	delay_n 4
	dey
	Jpl *-3
	tay
	pla
.elseif(n = 690)
	; 8 bytes
	sty @zptemp
	ldy #136 ;hides 'dey'
	Jne *-1
	ldy @zptemp
.elseif(n = 693)
	; 10 bytes
	sty @zptemp
	ldy #n/7-1
	nop
	dey
	Jne *-2
	ldy @zptemp
.elseif(n = 695)
	; 8 bytes
	pha
	tya
	ldy #136 ;hides 'dey'
	Jne *-1
	tay
	pla
.elseif(n = 696)
	; 10 bytes
	pha
	sec
	lda #(n-10)/7
	nop
	sbc #1
	Jne *-3
	pla
.endif
.endif
.endif
.endif

.endif
	rest
.endmacro
