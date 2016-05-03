static constexpr char graphics[] = R"(
$30      $31      $32      $33      $34      $35      $24      $25      $26      $27
:        :        : .. ..  : .. ..  :        : .. ..  :        :        :        :        |
:        :        : .. ..  : .. ..  :        : .. ..  :        :        :        :        |
: .......:......  : .. ....:... ..  :........: .. ..  :        :        :   .    :   cc   |
: ..     :    ..  : ..     :    ..  :        : .. ..  :   c    :   c    :  .#.   :  c##c  |
: .. ....:... ..  : .......:......  :........: .. ..  :        :        :   .    :   cc   |
: .. ..  : .. ..  :        :        :        : .. ..  :        :        :        :        |
: .. ..  : .. ..  :        :        :        : .. ..  :        :        :        :        |
: .. ..  : .. ..  :        :        :        : .. ..  :        :        :        :        |
$20      $3A      $40      $2E      $3F      $2C      $21      $2D      $22
:        :        :        :        : ####   :        :  ##  ##:        : ## ##  |
:        :        :        :        :##  ##  :        : ### ###:        : ## ##  |
:        :  #c    :        :        :#c  #c  :        : #c# #c#:        :  c  c  |
:        :  cc    :        :        :   cc   :        : cc  cc : cccccc : c  c   |
:        :        :        :        :  ##    :   ##   :        : ###### :        |
:        :  cc    :  cc  cc:  cc    :        :   cc   :cc  cc  :        :        |    
:        :  ..    :  ..  ..:  ..    :  ..    :    .   :..  ..  :        :        |    
:        :        :        :        :        :   .    :        :        :        |
$41      $42      $43      $44      $45      $46      $47      $48      |
:   ##   :######  :  ### # :#####   :####### :####### :  ### # :#### ###|
:   ##   : ##  ## : ### ## : ## ##  : ##   # : ##   # : ### ## : ##  ## |
:  #c#c  : #c  #c :#c#     : #c  #c : #c #   : #c #   :#c#     : #c  #c |
:  c cc  : ccccc  :ccc     : cc  cc : cccc   : cccc   :ccc ccc : cccccc |
: ###### : ##  ## :###     : ##  ## : ## #   : ## #   :###  ## : ##  ## |
: c   cc : cc  cc : ccc  c : cc cc  : cc   c : cc     : ccc cc : cc  cc |
:c.   .c.:.c.c.c  :  .c.c  :.c.c.   :.c.c.c. :.c.c    :  .c.c  :.c.c .c.|
:        :        :        :        :        :        :        :        |
$49      $4A      $4B      $4C      $4D      $4E      $4F      $50      $51     | 
:  ####  :   #### :###  ## :####    :##   ###:###  ###:  ###   :######  : ##### |
:   ##   :    ##  : ##  #  : ##     : ## ### : ###  # : ## ##  : ##  ## :##   ##|
:   #c   :    #c  : #c #   : #c     : #c#c#c : #c#c # :#c   #c : #c  #c :#c   #c|
:   cc   :    cc  : cccc   : cc     : cccccc : c cccc :cc   cc : ccccc  :cc   cc|
:   ##   :##  ##  : #####  : ##     : # # ## : #  ### :##   ## : ##     :## ## #|
:   cc   :c  ccc  : cc ccc : cc   c : c   cc : c   cc : cc cc  : cc     :cc  cc |
:  c.c.  : c.c.   :.c.  .c.:.c.c.c. :.c.  .c.:.c.   . :  .c.   :.c.c    : .c. c.|
:        :        :        :        :        :        :        :        :       |
$5C      $52      $53      $54      $55      $56      $57      $58      $59     |
: ## ##  :######  : #### # :########:###  ###:####   #:### ## #:##   ## :###  ##|
:   #    : ##  ## :###  ## :#  ##  #: ##   # : ###   #: ## ## #:### ##  : ### # |
:  # #   : #c  #c :#c#c    :   #c   : #c   # :  #c# # : #c #c #: #c#c   :  #c#  |
:  c cc  : cccc   : ccccc  :   cc   : cc   c :  ccccc :  ccccc :  ccc   :   cc  |
: ###### : ## ##  :#  #### :   ##   : ##   # :   ###  :  ##### :  ####  :   ##  |
: c   cc : cc cc  :cc  ccc :   cc   : cc   c :   ccc  :   ccc  : cc ccc :   cc  |
:c.   .c.:.c.c .c.:. .c.c  :  .c.c  :  .c.c  :    .   :   . .  :.c   .. :  c.c. |
:        :        :        :        :        :        :        :        :       |
$5A      $5D      $5B      |
:####### : ## ##  :   #    |
:#   ### :        :  # #   |
:   #c#  :  #c#   :   #    |
:  ccc   : cc cc  :  c cc  |
: ###    :##   ## : ###### |
:ccc   c : cc cc  : c   cc |
:.c.c.c. :  c.c   :.c   .c.|
:        :        :        |
$61      $62      $63      $64      $65      $66      $67      $7B      |
:        :###     :        :   ###  :        :   ###  :        :  ##    |
: ####   : ##     :  ### # :    ##  :  ####  :  ##    : #### # : #  #   |
:#   #c  : #c#c#  : #c  #  : #c#c#  : #c  #c :  #c    :#c   #c : #c#c   |
: ccccc  : cc  cc : cc     :cc  cc  : cccccc : ccccc  :cc   cc :c   cc  |
:##  ##  : ##  ## : ##     :##  ##  : ##     :  ##    : #####  : #####  |
:cc  cc  : cc  cc : cc   c :cc  cc  : cc   c :  cc    :     cc :cc  cc  |
: .c. c. : .c.c.  :  .c.c  : c.c c. :  c.c.  : c.c.   :.    c. : .c. c. |
:        :        :        :        :        :        : .....  :        |
$68      $69      $6A      $6B      $6C      $6D      $6E      $6F    
:###     :  ###   :  ###   :###     :  ###   :        :        :        !
: ##     :        :        : ##  ## :   ##   :######  :## ###  :  ####  !
: #c#c#  :  #c#   :  #c#   : #c  #  :   #c   :#c # #c : #c  #c : #c  #c !
: cc  cc :   cc   :   cc   : cccc   :   cc   :cc c cc : cc  cc : cc  cc !
: ##  ## :   ##   :   ##   : ## ##  :   ##   :## # ## : ##  ## : ##  ## !
: cc  cc :   cc   :   cc   : cc  cc :   cc   :cc c cc : cc  cc : cc  cc !
:.c.c c.c:  .c.c  :   c.   :c.c. ...:  c.c.  :.c . .c.:.c. c.c.:  c.c.  !
:        :        : ...    :        :        :        :        :        !
$70      $71      $7C      $72      $73      $74      $75      $76
:        :        :##  ##  :        :        :  ##    :        :        !
:######  : ###### :        :### ##  :  ####  : ####   :### ### :###  ## !
: #c  #c :#c  #c  : #c#c   : #c# #c : #c   # :  #c    : #c  #c : #c   # !
: cc  cc :cc  cc  :c   cc  : cc     :  ccc   :  cc    : cc  cc : ccc  c !
: ##  ## :##  ##  : #####  : ##     :   ###  :  ##    : ##  ## :  ####  !
: ccccc  : ccccc  :cc  cc  : cc     : c   cc :  cc    : cc ccc :   cc   !
: c.     :    c.  : c.c c. :c.c.    :  c.c.  :   c.   :  c.c c.:   .    !
:...     :    ... :        :        :        :        :        :        !
$77      $78      $79      $7A      $7D      $7E      $7F      $60      $0F
:        :        :        :        : ##  ## : ##  ## : ##  ## :  ####  :        !
:###  ## :### ### :### ### :####### :        :        :        : ##  ## :### ### !
: #c   # : #c# #  : #c  #  :#    #c :  #c#c  :#c#  #c#:#c# #c# :#c# #c  :#.# #.. !
: cc c c :  ccc   : cc c   :   ccc  : cc  cc : cc   c : cc  cc : cc  cc :c c ccc !
:  ##### :  ###   :  ##    :  ###   : ##  ## : ##   # : ##  ## : ##  ## :# # #.. !
:  ccccc : c ccc  :  cc    : cc   c : cc  cc : cc   c : cc ccc : ccc  c :ccc c   !
:   . .  :c.c c.c : c.     :c.c.c.c :  c.c.  :  c.c.  :  c.c c.: .c c.  :.c. .   !
:        :        :..      :        :        :        :        :        :        !

; SRAM strings:
;ARE YOU SURE?
$10      $11      $12      $13      $14      $15      $16      $17      $18      $19      $1A
:  ccc   :ccc     :     cc : ccc    :        :        :        :        :        :        :   cccc !
: cc cc  : cc   cc:cc  cccc:  cc  cc:   cccc :   ccc c:c  cccc : ccc cc : ccc  cc:    cccc:  cc  cc!
:cc   cc : cc  cc : cc  cc :  cc  c :  cc  cc:    cc  :c c   cc:  ccc cc:  cccccc:c  c   c:c c   cc!
:cc   cc : cc  ccc:ccc  cc :  cccc  :  cc  cc:    cccc:c  ccccc:  cc    :  cc c c:c   cccc:c    cc !
:cc   cc : cc  cc :     cc :  cc cc :  cc  cc:     ccc:  cc  cc:  cc    :  cc c c:c  cc  c:c   cc  !
: cc cc  : cc  cc :  c  cc :  cc  cc:  cc  cc:      cc:  cc  cc:  cc    :  cc c c:c  cc  c:c       !
:  ccc   :cccc  cc:cc    cc: cccc cc:c  cccc :      c :   ccc c:ccccc   : ccc c c:cc  ccc :cc  cc  !
:        :        :        :        :        :        :        :        :        :        :        !
;CHOOSE SLOT
$01      $02      $03      $04      $05      $06      $07      $08      $09      $0A      $0B      $0C
:....###.:..######:...###..:###.####:########:########:.#######:##...##.:..######:########:########:########!
:#...####:.#######:#..#####:##..####:########:#######.:.#######:###..###:..######:########:########:########!
:##...###:.#....##:#..##...:#....##.:...###..:..####..:..##....:###..###:..###...:.#..#...:##..#...:###....#!
:###...#.:#.###..#:#..###..:##..##..:###.#..#:#..####.:.##.###.:.##..###:..##..##:..#..##.:.##..##.:.#..##..!
:###.....:##.....#:#..###..:##..###.:..###...:...####.:.###....:.##..###:..##....:..#..##.:.##..##.:.#......!
:####...#:#..##..#:#..###..:##..####:...##..#:#######.:.##..##.:.##..###:..##..##:###..##.:.##..##.:.#..####!
:####...#:#..##..#:#..###..:##..##.#:##..#..#:##.####.:.##..##.:.##..###:..##..##:#.#..##.:.##..##.:.#..###.!
:#####.##:##...#..:....#...:.##..##.:...###..:..######:..##...#:......#.:...##...:.#...#..:.....#..:..#....#!
;YES
$1B      $1C      $1D      $1E      $1F
:###cc##c:cccccccc:c###cc##:#cc##cc#:#ccccccc!
:c##cc#cc:####c##c:cc##ccc#:#ccccccc:cccccccc!
:c##c#ccc:c###cc#c:cc##ccc#:#ccc####:cccccccc!
:c####ccc:cc####cc:cc##ccc#:#cc#ccc#:#ccccccc!
:c#####cc:ccc###cc:cc##ccc#:#ccc####:#ccccccc!
:c##c###c:cccc#ccc:cc##ccc#:#cc##cc#:#ccccccc!
:###cc###:c#c#cccc:#####c##:##cc###c:##cccccc!
:cccccccc:c##ccccc:cccccccc:cccccccc:cccccccc!
$80      $81      $82      $83      $84      $85      $86      $87      $88      $89      $8A      $8B      $8C      $8D      $8E      $8F
:# # # # :# # # # :# # # # :        :      # :# # # # :# # # # :# # # # :c#c#c#c#:c#c#c#c#:c#c#c#c#:c#c#c#c#:c#c#c#c#:c#c#c#c#:.#c#c#c#:c#. ..c#!!
: #   #  : #   #  : #      :        :        : #   #  : #   #  : #   #  :.c.c.c.c:.c.c.c.c:.c.c.c.c:.c......:........:........:........:.... ...!!
:# # # # :# # # # :# #     :        :        :  # # # :# # # # :# # # # :c#c#c#c#:c#c#c#c#:c#c#c#.#:c . c#.#:c#c#c#c#:c#c#c#c#:c#.#.#. :.#. . .#!!
:   #    :   #    :        :        :        :   #    :   #    :   #    :.c.ccccc:cc.c.c.c:.c.c.##.:# #.....:........:........:........:.. . . .!!
:# # # # :# # # # :        :        :        :      # :# # # # :# # # # :cccccccc:ccc#c#c#:c#c#. . :. .#c#. :.#c#c#c#:c .#c#c#:c#c#c#c#:c#.#. . !!
: #   #  : #   #  :        :        :        :        : #   #  : #   #  :cccccccc:cc.ccc.c:cc.. .#.:  ##    :#.#.#.#.:#.......:........:......#.!!
:# # # # :# # #   :        :        :        :        :# # # # :# # # # :cccccccc:c#c#c#c#:c c . . :. . . . :. . . .#:c#c#c#c#:c#c#c#c#:c#c#c#.#!!
:       #:        :       .:        :        :        :       #:       #:cc.c.c.c:.c.c.c.c:#.#. .#.:###.#.#.: . .   .:##...c..:........:........!!
$90      $91      $92      $93      $94      $95      $96      $97      $98      $99      $9A      $9B      $9C      $9D      $9E      $9F
:# # # # :# #     :      . :.       :        :        :  # # # :# # # # :c#c#c#c#:c#c#c#c#:. . .#c#:.#c#c . :. . . . :. . c.c#:c#c#c#c#:c#c#c..#!!
: #   #  : #      :        :        :        :        :     #  : #   #  :.c.c.c.c:.c.c.c..:#..c..#.:#.#...#.: .      :   #....:........:........!!
:# # # # :#       :        :.       :    .   :        :    # # :# # # # :c#c#c#c#:c#c#c#c :.#c#c . :. . . . :. . . . :. . .#c#:c#c#c#c#:c#c#c#.#!!
:   #    :        :       .: .      :        :        :   #    :   #    :.c.c.c.c:.c.c.c#.:#....c..:....#.#.: .      :     ...:........:........!!
:# # # # :        :    .   :.   .   :        :        :    # # :# # # # :c#c#c#c#:c#c#. . :.#c#c#c#:c#c . . :. . . . :. . . .#:c#c#c#c#:c#c#c#c#!!
: #   #  :        : . . . .: . . . .:   . .  :        :     #  : #   #  :.c.c.c.c:.c.##...:........:....#. .: .      :       .:........:........!!
:# # # # :        :. . . . :. . . . :.       :        :      # :# # # # :c#c#c#c#:c#c . c#:c#c#c#c#:. . . . :. . . . :. . . . :.#c#c#c#:.#.#.#c#!!
:        :       .: . . .  :   .    : . .    :        :       #:       #:.c.c.c.c:.c#. ...:........:..#.#.  : . . . .:        :#.......:........!!
$A0      $A1      $A2      $A3      $A4      $A5      $A6      $A7      $A8      $A9      $AA      $AB      $AC      $AD      $AE      $AF
:# # # # :      . :. . . . :    .   :. . .   :        :      # :# # # # :c#c#c#c#:c c#. c#:c#c#. . :. . . . :. . . . :. . . . :.#c#c#c#:c#c#c#.#!!
: #   #  :       .: . . . .:     .  :   . . .:        :        : #   #  :.c.c.c.c:....#...:....#.##: . . . .: .   .  :   .    : .......:........!!
:# # #   :      . :. .   . :.     . :. . . . :. .     :        :# # # # :c#c#c#c#:c#c .#c#:c#.#c.c :. . . . :. . . . :. . . . :. c#c#c#:c#c#c#c#!!
:   #    :     . .: . .   .:   . . .: .   .  : .      :        :   #    :.ccc.c.c:.c#.....:..#c.c.c:.c#c#. .:#. .#.#c:#  .    : .......:........!!
:# # #   :      . :. . .   :.   . . :. .     :  .     :        :# # # # :c#c#c#c#:c#. . . :. . c.c.:c.. . . :... ... :c . . . :. c#c#c#:c#.#c#.#!!
: #   #  :     . .: .     .: .   . #:#. . .  : . .    :        : #   #  :.c.ccc.c:.c#.#. .: .#c.c.c:.c.c...c:....#..c:.c#.    : .......:........!!
:# # #   :      . :. . . . :. . . . :. . . . :  . .   :        :# # # # :c#c#c#c#:c#. . . :. . c.c.:c.c.c.c.:c#c.c#c.:c .#. . :. c#.#c#:.#.#c#.#!!
:        :     . .: . . . c:.c.c.# .: . . . .: # .    :        :       #:.c.c.c.c:.c#     :   ..c.c:.c.c.c.c:.c.c.c.c:.c#.#. .: .......:........!!
$B0      $B1      $B2      $B3      $B4      $B5      $B6      $B7      $B8      $B9      $BA      $BB      $BC      $BD      $BE      $BF
:# # #   :      . :. . . ..:c.c.c.. :c . c ..:..c . . :        :  # # # :c#c#c#c#:c#. . . :. .#c.c.:c.c.c.c.:c.c.c.c.:c.c#. . :. c#c#c#:c#.#.#.#!!
: #   #  :     . .: . . c c:.c.c.c.#:.c . c #: ..# .  :        : #   #  :.....c.c:.c#     : .#c.c.c:.c.c.c.c:.c.c.c.c:.c.c.. .: .......:......#.!!
:# # #   :      . :. . ... :. . ..c.:c.. . . :. c .   :        :  # # # :c#.#c#c#:c#. . . :. ..c.c.:c.c.c.c.:c.c.c.c.:c.c.c . :. c#c#c#:c#c#c#. !!
:   #    :     . .: . . .#c:.c.. #.c:.c#. #.c:.# . .  :        :   #    :.....c.c:.c#  . .: ..c.c.c:.c.c.c.c:.c.c.c.c:.c.c.. .: .......:........!!
:# # #   :      . :. . ..c.:c.c.c.c.:c.c c.c.:c.c   . :        :  # # # :.#.#c#c#:c#. . . :. c.c#. :. c.c.c.:c.c.c.. :.#..c . :.#c#c#c#:c#c#c#.#!!
: #   #  :     . .: . c.c.c:.c.c.c.c:.c.c.c.c:.c.#    :        : #   #  :.....c.c:.c## . .: ..c.. .: .#.#..c:.c..#. .: .#... .:#.......:........!!
:# # #   :      . :. c.c.c.:c.c.c.c.:c.c.c.c.:c.c .   :        :# # # # :c#.#c#c#:c#c . . :. c.. c :c . . c.:c.c . c :c c#c . :.#c#c#c#:c#c#c#c#!!
:        :        : ..c.c.c:.c.c.c.c:.c.c.c.c:.c.c    :       #:       #:.....c.c:.c..   .: #.c.c.c:#....c.c:.c.c#...:.c.c..  :###.....:........!!
$C0      $C1      $C2      $C3      $C4      $C5      $C6      $C7      $C8      $C9      $CA      $CB      $CC      $CD      $CE      $CF
:# # # # :        :  c.c . :#   c.c.:c.c.. # :  ...   :        :# # # # :c#.#c#c#:c#c . . :. c.c#. :. . ..c.:c.c . . :.#c.c . :.#c#c#c#:c#c#c#c#!!
: #   #  :     . .:  .c# #.:## .#c.c:.c.##. #: . #    :        : #   #  :...c.c#.:.c#...#.: ..c..#c:###c.c.c:.c...###:..#c..##: .......:........!!
:# # # # :      . :  c.. c#:# . c.c.:c.c.c ##:.c. .   :      # :# # # # :.#c#c#.#:c#. c .#:. c.c.c.:c .#c.c.:c.c.c#.#:c.c.c . :. c#c#c#:c#c#c#c#!!
:   #    :     ###:  .c.##c: .#c.c.c:.c.c#.#.:#.#c .  :        :   #    :...c....:.c#...#.:#..c.c.c:...c.c.c:.c.c.c.c:.c.c...#:#.......:........!!
:# # # # :      . :. c.c.c.:. ..c.c.:c.c.c.. :c.c.. . :      # :# # # # :.#c#c#. :.#. .#.#:c c.c.c.:c.c.c.c.:c.c.c.c.:c.c.c#c :.#c#c#c#:c#.#c#c#!!
: #   #  :       c:#..c.c.c:.c.c.c.c:.c c.c.c:.c.c#.  :     #  : #   #  :.c.c....:.c##.c#c:...c.c.c:.c.c.c.c:.c.c.c.c:.c.c.c..:#.......:........!!
:# # #   :      ..:c c.c.c.:c.c.c.c.:c.c.c.c.:c.c..   :      # :# # # # :c#c#c#.#:c#c#..c#:c#c.c.c.:c.c.c.c.:c.c.c.c.:c.c.c.c :.#c#c#c#:c#c#c#.#!!
:        :       .:#..c.c.c:.c.c.c.c:.c.#.c.c:.c.c    :       #:       #:.c.c....:.c.c#c.c:.c.c.c.c:.c.c.c.c:.c.c.c.c:.c.c.c#.:#.......:........!!
$D0      $D1      $D2      $D3      $D4      $D5      $D6      $D7      $D8      $D9      $DA      $DB      $DC      $DD      $DE      $DF
:# # #   :        :  . ..c.:c.c.c.c.:c.c c.c.:c.c..   :      # :# # # # :c#c#.#.#:c#c#c#.#:c#c.c.c.:c.c.c.c.:c.c c.c.:c.c.c . :c#c#c#c#:c#c#c#c#!!
: #      :        :   c c.c:.c.c.c #:.. #.c.c:.c.c    :     #  : #   #  :#c.c..#.:#.#c#.##:.c.c.c.c:.c.c.c.c:.c...c.c:.c.c..#.:........:........!!
:# #     :        :  . ..c.:c.c.c.c.:c...c.c.:c.c..   :      # :# # # # :c#c .#. :c c c c :.#c.c.c.:c.c.c.. :c . c.c.:c.c.c .#:c#c#c#c#:c#c#c#c#!!
:        :        : ....c.c:.c.c.c.c:.c.c.c.c:.c.c    :        :   #    :#c#.#.#.:#.###.#.:#c.c.c.c:.c.c..#.:..#c.c.c:.c.c..#.:........:........!!
:#       :        :... c.c.:..c.c.c.:c.c.c.c.:..c     :      # :# # # # :c .#. c :. . . . :.#c.c.c.:c#c.c.c.:c.c.c#c :c#c.c . :..c#c#.#:c#c#c.c#!!
:        :       .:.. ..c.c:.c.c.c.c:.c.c.c.c:.c..    :        : #   #  :..#.#.#.:..## # #:  #c.c.c:...c.c.c:.c.c...c:.c.c.#  : .......:........!!
:        :      ..:.   ..c.:c.c.c . :c . c.c.:c..     :        :# # # # :. . . . :.#. . . :. ..c.c.:c . . . :. . .#c.:c.c.. . :. .#.#.#:.#c#c#c#!!
:        :     #..:     #.c:.c.c.##.:.###.c.c:.#      :       #:       #:..#.#.#.:..##    :  #..c.c:.c..#.#.:#.#..c.c:.c.c#.#.:#. .....:........!!
$E0      $E1      $E2      $E3      $E4      $E5      $E6      $E7      $E8      $E9      $EA      $EB      $EC      $ED      $EE      $EF
:        :  . . . :    c ..:c.c.c.c.:c.c.c.c.:.       :        :# # # # :.#. . . :.#. . . :. ##c.c.:c.c.c.c.:c.c.c.c.:c.c . c#:.#. c#c#:c#c#c#.#!!
:       .:#c c.c  :  #  c .:.c.c.c.c:.c.c.c..: c#     :        : #   #  :.. ##.##:#.#.   #:   #.c.c:.c.c.c.c:.c.c.c.c:.c..  #.:#.......:........!!
:      . :c.. c.c : #c#..# :....c.. :. c.. . :cc.     :        :  # # # :. . . . :.#. . . :. . . . :. . c.c.:c.c.c.c.:c . . .#:c#.#c#c#:c#c#.#.#!!
:      #c:#c.. c.c:#.cc c .: . c # .: . . . c:cc .    :        :   #    : # # .##: .#.    : .#.#.#.:..###c.c:.c.c.c.c:..#.#  .:..#.....:........!!
:      c.:. c.. c :. ... c :#   . . :.   . cc:c.##c   :        :    # # :. . .#. :. . . . :.#cccccc:ccc c.c.:c.c.c.c :. . .#. :.#.#c#.#:.#.#. .#!!
:     c.c:.. c.. .:#. . ccc:c. . . .: . . ..c:cc##cc c:#       :     #  :##  #..#:     ...:#c.ccccc:ccc##.#.:..#.#.#.:.# ##.#.:#.#.....:...... .!!
:    ..c.:c.. c...:c.c cccc:cc. . . :  . .ccc:cc. cc.#:c       :      # :. # .#. :# . . . :.#c#cccc:ccc . . :. . . .#:. . . c :.#c#c#c#:c#c#.#. !!
:   #.c.c:##.# c .:###. ccc:cc . . .: . ..ccc:cc ccc#.:cc #    :       #:##   .##: ...#...:#c.ccccc:ccc.#...:........:..   .#.:#.......:........!!
$F0      $F1      $F2      $F3      $F4      $F5      $F6      $F7      $F8      $F9      $FA      $FB      $FC      $FD      $FE      $FF
:  ..c.c.:. ... . :  .#cccc:ccc # . :. ccccc :. ..c .#:c.c..   :      # :. # # . :c#. . . :.#c#cccc:ccc .#.#:.#.#.#.#:c#. # . :.#c#c#c#:c#c#c#. !!
: ..c.c.c:   c   #: . . ccc:cccc . c: ccccc##:#c.c . c:cc.c#   :        :#. #   .:..   .##:#c.ccccc:ccc##...:........:..#c## #:#.#.....:........!!
:  c.c.c.:. . . . :.cc .#c.:ccccc.. :.#c.. . :ccc . cc:ccc . . :        :. # # .#:c#. . .#:.#c#cccc:ccc .#.#:.#.#.#. :c .cc#. :. . .#c#:c#c#c#c#!!
: c.c.c.#:   . .##:#ccc###.: ccccccc:#c . ccc:cc . ccc:cc.. c.c:        : .   ##c:.. .#. #:#c.ccccc:ccc.#...:......#c:...ccc.c:..## ##.:........!!
:....c.. :. . .#c :. ccc.. :.#c.c.c.:c#. ccc.:c . ccc.:c . c.c.:c .     :. . . . :c#c . . :.#c#cccc:ccc .#.#:.#.#.#c.:c cccccc:ccc#c#. :.#. .#c#!!
:.c . . .:#.   ..c: . ..ccc:#. ##.#.: .#ccc c:#. ccc #: # c.c.c:.c #    :#.#c..#c:.c.c.. #:#c.ccccc:ccc#....:.....c.c:...ccccc:cccccccc:c.#.#.#.!!
:c.c . c :  .#c . :cc. . cc:c.c . # :c.c.c . :##c . ##:c ..c.c.:c.. .   :ccccc . :c . . . :.#cccccc:ccc c .#:.#. c.c#:. cccccc:cccccccc:ccccc#c#!!
:.c.c.c..: # .cc#.: ccc# #c:.ccccc c: c c # .:.c# #. c:cc c.c.c:.c###.  :cccc..#.:.c..####: ##.#.#.:###..c.c:...c.c..:#ccccccc:cccccccc:cccccccc!!
)";


static constexpr unsigned FindLine(unsigned n)
{
    unsigned p = 0;
    for(; n>0; --n, ++p)
        while(p < sizeof(graphics) && graphics[p] != '\n') ++p;
    return p;
}
static constexpr unsigned FindOffset(unsigned hexcode)
{
    constexpr char hex[]="0123456789ABCDEF";
    const char c1 = hex[hexcode>>4], c2 = hex[hexcode&15];
    unsigned line = 1, linebegin=0;
    for(unsigned p=0; p < sizeof(graphics); ++p)
    {
        if(graphics[p]=='\n') { linebegin=p+1; ++line; continue; }
        if(graphics[p]=='$'
        && graphics[p+1]==c1
        && graphics[p+2]==c2) return line*65536+(p+1-linebegin);
    }
    return ~0u;
}
static constexpr char GetTileChar(unsigned begin, unsigned x,unsigned y)
{
    begin += y*65536 + x;
    unsigned offset = FindLine(begin>>16) + (begin&0xFFFF);
    return offset >= sizeof(graphics) ? ' ' : graphics[offset];
}
static constexpr unsigned IdentifyTileChar(char c)
{
    return c=='#' ? 3 : c=='.' ? 1 : c=='c' ? 2 : 0;
}
static constexpr unsigned char GetTileByte(unsigned c, unsigned index)
{
    unsigned begin = FindOffset(c);
    unsigned shift = index>>3;

    // A bit faster alternative
    unsigned offset = FindLine((begin>>16) + (index&7)) + (begin&0xFFFF);
    if(offset >= sizeof(graphics)) return 0x00;

    unsigned result = 0;
    for(unsigned x=0; x<8; ++x)
        result |= (((IdentifyTileChar(graphics[offset+7-x]) >> shift) & 1) << x);
    return result;
}

#define m(c,n) GetTileByte(c,n)
#define m16(c) m(c,0),m(c,1),m(c,2),m(c,3),m(c,4),m(c,5),m(c,6),m(c,7),\
               m(c,8),m(c,9),m(c,10),m(c,11),m(c,12),m(c,13),m(c,14),m(c,15)
#define w8(c) m16(c),m16(c+1),m16(c+2),m16(c+3),m16(c+4),m16(c+5),m16(c+6),m16(c+7)
#define w64(c) w8(c),w8(c+8),w8(c+16),w8(c+24),w8(c+32),w8(c+40),w8(c+48),w8(c+56)

static constexpr unsigned char TileData[0x1000] = {w64(0),w64(64),w64(128),w64(192)};

#include <iostream>
int main()
{
    std::cout.write( (const char*) TileData,0x1000);
}
