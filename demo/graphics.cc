#include <iostream>

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
$130     $131     $132     $133     $134     $135     $124     $125     $126     $127
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
$68      $69      $6A      $6B      $6C      $6D      $6E      $6F   ;  
:###     :  ###   :  ###   :###     :  ###   :        :        :        !
: ##     :        :        : ##  ## :   ##   :######  :## ###  :  ####  !
: #c#c#  :  #c#   :  #c#   : #c  #  :   #c   :#c # #c : #c  #c : #c  #c !
: cc  cc :   cc   :   cc   : cccc   :   cc   :cc c cc : cc  cc : cc  cc !
: ##  ## :   ##   :   ##   : ## ##  :   ##   :## # ## : ##  ## : ##  ## !
: cc  cc :   cc   :   cc   : cc  cc :   cc   :cc c cc : cc  cc : cc  cc !
:.c.c c.c:  .c.c  :   c.   :c.c. ...:  c.c.  :.c . .c.:.c. c.c.:  c.c.  !
:        :        : ...    :        :        :        :        :        !
$70      $71      $7C      $72      $73      $74      $75      $76 ;
:        :        :##  ##  :        :        :  ##    :        :        !
:######  : ###### :        :### ##  :  ####  : ####   :### ### :###  ## !
: #c  #c :#c  #c  : #c#c   : #c# #c : #c   # :  #c    : #c  #c : #c   # !
: cc  cc :cc  cc  :c   cc  : cc     :  ccc   :  cc    : cc  cc : ccc  c !
: ##  ## :##  ##  : #####  : ##     :   ###  :  ##    : ##  ## :  ####  !
: ccccc  : ccccc  :cc  cc  : cc     : c   cc :  cc    : cc ccc :   cc   !
: c.     :    c.  : c.c c. :c.c.    :  c.c.  :   c.   :  c.c c.:   .    !
:...     :    ... :        :        :        :        :        :        !
$77      $78      $79      $7A      $7D      $7E      $7F      $60      $0F ;
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
$10      $11      $12      $13      $14      $15      $16      $17      $18      $19      $1A ;
:  ccc   :ccc     :     cc : ccc    :        :        :        :        :        :        :   cccc !
: cc cc  : cc   cc:cc  cccc:  cc  cc:   cccc :   ccc c:c  cccc : ccc cc : ccc  cc:    cccc:  cc  cc!
:cc   cc : cc  cc : cc  cc :  cc  c :  cc  cc:    cc  :c c   cc:  ccc cc:  cccccc:c  c   c:c c   cc!
:cc   cc : cc  ccc:ccc  cc :  cccc  :  cc  cc:    cccc:c  ccccc:  cc    :  cc c c:c   cccc:c    cc !
:cc   cc : cc  cc :     cc :  cc cc :  cc  cc:     ccc:  cc  cc:  cc    :  cc c c:c  cc  c:c   cc  !
: cc cc  : cc  cc :  c  cc :  cc  cc:  cc  cc:      cc:  cc  cc:  cc    :  cc c c:c  cc  c:c       !
:  ccc   :cccc  cc:cc    cc: cccc cc:c  cccc :      c :   ccc c:ccccc   : ccc c c:cc  ccc :cc  cc  !
:        :        :        :        :        :        :        :        :        :        :        !
;CHOOSE SLOT
$01      $02      $03      $04      $05      $06      $07      $08      $09      $0A      $0B      $0C   ;
:....###.:..######:...###..:###.####:########:########:.#######:##...##.:..######:########:########:########!
:#...####:.#######:#..#####:##..####:########:#######.:.#######:###..###:..######:########:########:########!
:##...###:.#....##:#..##...:#....##.:...###..:..####..:..##....:###..###:..###...:.#..#...:##..#...:###....#!
:###...#.:#.###..#:#..###..:##..##..:###.#..#:#..####.:.##.###.:.##..###:..##..##:..#..##.:.##..##.:.#..##..!
:###.....:##.....#:#..###..:##..###.:..###...:...####.:.###....:.##..###:..##....:..#..##.:.##..##.:.#......!
:####...#:#..##..#:#..###..:##..####:...##..#:#######.:.##..##.:.##..###:..##..##:###..##.:.##..##.:.#..####!
:####...#:#..##..#:#..###..:##..##.#:##..#..#:##.####.:.##..##.:.##..###:..##..##:#.#..##.:.##..##.:.#..###.!
:#####.##:##...#..:....#...:.##..##.:...###..:..######:..##...#:......#.:...##...:.#...#..:.....#..:..#....#!
;YES
$1B      $1C      $1D      $1E      $1F ;
:###cc##c:cccccccc:c###cc##:#cc##cc#:#ccccccc!
:c##cc#cc:####c##c:cc##ccc#:#ccccccc:cccccccc!
:c##c#ccc:c###cc#c:cc##ccc#:#ccc####:cccccccc!
:c####ccc:cc####cc:cc##ccc#:#cc#ccc#:#ccccccc!
:c#####cc:ccc###cc:cc##ccc#:#ccc####:#ccccccc!
:c##c###c:cccc#ccc:cc##ccc#:#cc##cc#:#ccccccc!
:###cc###:c#c#cccc:#####c##:##cc###c:##cccccc!
:cccccccc:c##ccccc:cccccccc:cccccccc:cccccccc!
)"
#include "avatar/finger1.inc"
#include "avatar/finger2.inc"
#include "avatar/happycup1.inc"
#include "avatar/happycup2.inc"
#include "avatar/helmet.inc"
;

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
    const char c0 = hex[hexcode>>8], c1 = hex[(hexcode>>4)&15], c2 = hex[hexcode&15];
    unsigned line = 1, linebegin=0;
    for(unsigned p=0; p < sizeof(graphics); ++p)
    {
        if(graphics[p]=='\n') { linebegin=p+1; ++line; continue; }
        if(graphics[p]=='$')
        {
            if(graphics[p+1]==c0
            && graphics[p+2]==c1
            && graphics[p+3]==c2) return line*65536+(p+1-linebegin);

            if(c0 == '0'
            && graphics[p+1]==c1
            && graphics[p+2]==c2
            && graphics[p+3]==' ') return line*65536+(p+1-linebegin);
        }
    }
    return ~0u;
}

/*static constexpr char GetTileChar(unsigned begin, unsigned x,unsigned y)
{
    begin += y*65536 + x;
    unsigned offset = FindLine(begin>>16) + (begin&0xFFFF);
    return offset >= sizeof(graphics) ? ' ' : graphics[offset];
}*/
static constexpr unsigned IdentifyTileChar(char c)
{
    return c=='#' ? 3 : c=='.' ? 1 : c=='c' ? 2 : 0;
}
static constexpr unsigned char GetTileByte(unsigned c, unsigned index)
{
    unsigned begin = FindOffset(c);
    if(begin == ~0u) return 0x00;
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

static constexpr unsigned char TileData[0x2000] = {w64(0),w64(64),w64(128),w64(192),
                                                   w64(0x100),w64(0x140),w64(0x180),w64(0x1C0)};

int main()
{
    std::cout.write( (const char*) TileData,0x2000);
}
