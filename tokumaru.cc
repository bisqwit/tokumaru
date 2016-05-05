/* Tokumaru tile compression algorithm implementation
 *
 * Copyright (C) 2016 Joel Yliluoma - http://iki.fi/bisqwit/
 *
 * #include "zlib license.txt"
 *
 * Based on algorithm described at http://pastebin.com/GNnimLzX
 * The documentation is based on code released by Tokumaru
 * at http://forums.nesdev.com/viewtopic.php?p=54230#p54230
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from
 * the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <algorithm>
#include <cstdio>
#include <vector>
#include <assert.h>
#include <queue>

#ifdef _OPENMP
# include <omp.h>
#endif

static unsigned compressionlevel = 5;
static bool threads  = true;
static bool extension_invertbit     = false; // Make decompressor smaller
static bool extension_persistentrow = false; // Makes decompressor 1 byte smaller, but compression ratio worse :(

static std::FILE* diagnostics = nullptr;

// Extract one pixel from a tile.
static unsigned ExtractColor(const unsigned char* tile, unsigned y, unsigned x)
{
    return ((tile[y+0]>>(7-x))&1)*1
         + ((tile[y+8]>>(7-x))&1)*2;
}
// Compare a single row of two tiles (8 pixels).
static bool IdenticalRows(const unsigned char* tilerow1, const unsigned char* tilerow2)
{
    return tilerow1[0] == tilerow2[0] && tilerow1[8] == tilerow2[8];
}

static const unsigned char blankrow[9] = {0};


// The color frequency information collected before blocks are encoded
struct FollowupData
{
    unsigned next[4][4], count[4];
};

template<typename F>
static void CompressWholeBlock(const unsigned char* tiles, unsigned numtiles, F&& PutBits,
                               const unsigned char* previous)
{
    // First encode colors
    FollowupData f;
    for(unsigned c=0; c<4; ++c) for(unsigned d=0; d<4; ++d) f.next[c][d] = d + (c==d ? 0 : 4);

    const unsigned char* const first_prevrow = extension_persistentrow ? previous : blankrow;

    // Measure the number of transitions from each color to another
    for(const unsigned char* prevrow = first_prevrow, *tp = tiles; tp < tiles+16*numtiles; tp += 16)
        for(unsigned y=0; y<8; prevrow = tp + y++)
            if(!IdenticalRows(prevrow, tp+y))
                for(unsigned d,c=0, x=0; x<8; ++x, c=d)
                {
                    d = ExtractColor(tp,y,x);
                    if(x > 0 && c != d) f.next[c][d] += 4;
                }

    // Sort the follow-ups in descending order of uses. Make sure c itself is last.
    // This is ensured by the use-count remaining zero whenever c->c transition occurs.
    for(unsigned c=0; c<4; ++c)
        std::sort(f.next[c]+0, f.next[c]+4, [](unsigned a,unsigned b) { return a/4 > b/4; });
    for(unsigned c=4; c-- > 0; )
    {
        // Count the number of colors that can follow this color
        unsigned num_next = 0, pick;
        for(unsigned d=0; d<4; ++d) if(f.next[c][d] >= 2*4) ++num_next;
        f.count[c] = PutBits(num_next, 2);
        // Remove the use counts, so that only the color index remains
        for(unsigned d=0; d<4; ++d) f.next[c][d] &= 3;
        // Write just one color, and this is enough to reconstruct both
        // the most popular color and the list of colors that can follow.
        switch(num_next)
        {
            default: assert(num_next==0); continue; // If c is always followed by c, send nothing
            case 1:
                pick = f.next[c][0]; // Show the only non-c continuation
                break;
            case 2:
                pick = f.next[c][2]; // Show the only invalid continuation
                if(f.next[c][0] > f.next[c][1]) std::swap(f.next[c][0], f.next[c][1]);
                break;
            case 3:
                pick = f.next[c][0]; // Show the best continuation (will be encoded in fewer bits)
                if(f.next[c][1] > f.next[c][2]) std::swap(f.next[c][1], f.next[c][2]);
                break;
        }
        /**/ if(pick==(1-(c>=1)))        PutBits(0,1);   // Bit pattern: 0
        else if(pick==(2-(c>=2)))        PutBits(2,2);   // Bit pattern: 10
        else { assert(pick==(3-(c>=3))); PutBits(3,2); } // Bit pattern: 11
    }

    // Then encode tiles
    for(const unsigned char* prevrow = first_prevrow, *tp = tiles; tp < tiles+16*numtiles; tp += 16)
    {
        if(tp!=tiles) PutBits(0, 1); // flag for read more tiles
        for(unsigned y=0; y<8; prevrow = tp + y++)
            if(!PutBits(IdenticalRows(prevrow, tp+y), 1))
                for(unsigned c=0, x=0; x<8; ++x)
                {
                    unsigned prev=c; c=ExtractColor(tp,y,x);
                    if(x == 0) { PutBits(c, 2); continue; }
                    if(f.count[prev]==0 || (extension_invertbit ? !PutBits(c != prev, 1)
                                                                :  PutBits(c == prev, 1))
                    || f.count[prev]==1 || !PutBits(c != f.next[prev][0], 1)
                    || f.count[prev]==2 || !PutBits(c != f.next[prev][1], 1)) continue;
                }
    }
}

template<typename F>
static void CompressTilesWithBlockSplitting(const unsigned char* tiles, unsigned numtiles, F&& PutBits)
{
    // Create a directed acyclic graph from all starting positions
    // to blocks of different lengths, where the length of the arc
    // is the bit count required to encode that block
    std::vector<unsigned> compress_tree(numtiles*numtiles, ~0u);

    const unsigned MaxLeap = 1u << compressionlevel;

    if(diagnostics) std::fprintf(diagnostics, "Creating compression benchmark... ");
    #pragma omp parallel for schedule(static) if(threads)
    for(unsigned position=0; position<numtiles; ++position)
    {
        int tn = 0, nt = 1;
        #ifdef _OPENMP
        tn = omp_get_thread_num(); nt = omp_get_num_threads();
        #endif
        if(tn == 0 && diagnostics)
        {
            std::fprintf(diagnostics, "%6.2f%%\b\b\b\b\b\b\b", (position+1)*100.0/double(numtiles/nt));
            std::fflush(diagnostics);
        }
        for(unsigned length=std::min(MaxLeap,numtiles-position); length>0; --length)
        {
            unsigned bits=0;
            CompressWholeBlock(tiles+position*16, length,
                [&](unsigned value, unsigned numbits) { bits += numbits; return value; },
                               position>0 ? tiles+position*16-9 : blankrow);
            compress_tree[position*numtiles + length] = bits;
        }
    }
    if(diagnostics) std::fprintf(diagnostics, "\n");

    // Figure out the shortest path through that graph (use Dijkstra's algorithm)
    struct Record { unsigned distance=~0u, previous=~0u; bool visited=false; };
    std::vector<Record> targets(numtiles+1);

    typedef std::pair<unsigned,unsigned> P;
    struct Prioritize { bool operator()(const P& a,const P& b) { return a.second > b.second; } };
    std::priority_queue<P, std::vector<P>, Prioritize> queue;
    queue.emplace(0,0);
    targets[0] = Record{0, ~0u, false};
    while(!queue.empty())
    {
        // Take the shortest candidate
        auto top = std::move(queue.top()); queue.pop();
        unsigned vert = top.first, cost = top.second;
        if(targets[vert].visited) continue;
        targets[vert].visited = true;
        assert(targets[vert].distance == cost);
        // Try all different size leaps from that position
        for(unsigned leap = 1; leap <= std::min(MaxLeap,numtiles-vert); ++leap)
        {
            unsigned ends_at  = vert + leap;
            unsigned distance = compress_tree[vert*numtiles + leap] + !!vert;

            if(!targets[ends_at].visited && distance+cost < targets[ends_at].distance)
            {
                targets[ends_at].previous = vert;
                queue.emplace(ends_at, targets[ends_at].distance = distance+cost);
            }
        }
    }
    std::vector<unsigned> leaps;
    for(unsigned n=0,t=numtiles; t!=0; t=n) leaps.push_back(t - (n = targets[t].previous));
    unsigned position = 0;
    for(auto i = leaps.crbegin(); i != leaps.crend(); ++i)
    {
        unsigned leap = *i;
        if(position) PutBits(1,1);
        CompressWholeBlock(tiles + position*16, leap, PutBits,
                           position>0 ? tiles+position*16-9 : blankrow);
        position += leap;
    }
    assert(position == numtiles);
}

template<typename F>
static void CompressTiles(const unsigned char* tiles, unsigned numtiles, F&& PutByte)
{
    unsigned char bitBuffer = 0x01;
    auto PutBits = [&](unsigned value, unsigned numbits) -> unsigned
    {
        for(unsigned mask = 1 << (numbits-1); mask; mask >>= 1)
        {
            bool bit = value & mask, full = bitBuffer & 0x80;
            bitBuffer = (bitBuffer << 1) | bit;
            if(full) { PutByte(bitBuffer); bitBuffer = 0x01; }
        }
        return value;
    };
    PutByte(numtiles < 256 ? numtiles : 0u);
    if(compressionlevel == 0)
        CompressWholeBlock(tiles, numtiles, PutBits, blankrow);
    else
        CompressTilesWithBlockSplitting(tiles, numtiles, PutBits);
    // flush bits
    while(bitBuffer != 0x01) PutBits(0,1);
}

static void DecompressTiles(const unsigned char* data, int bytesremain, std::vector<unsigned char>& output)
{
    unsigned BitCount = 0x10000u;
    auto GetBit = [&]() -> bool
    {
        if(BitCount & 0x10000u)
        {
            if(bytesremain-- <= 0) { BitCount = 0x100u; return 0; }
            BitCount = *data++ | 0x100u;
        }
        return (BitCount <<= 1) & 0x100u;
    };
    auto Get2Bits = [&]() { unsigned r = GetBit(); return r*2 + GetBit(); };
    //
    unsigned num_tiles = bytesremain-- ? *data++ : 0;
    if(diagnostics) std::fprintf(diagnostics, "Decoding %u tiles...\n", num_tiles);
    unsigned char Count[4], Next0[4]{0,0,0,0}, Next1[4]{0,0,0,0}, Next2[4]{0,0,0,0};
    unsigned char part0=0x00, part1=0x00, plane2[8];
read_colors:
    for(unsigned A,B,C,n, c=4; c-- > 0; Count[c] = n)
    {
        // Read number of colors that can follow c
        n = Get2Bits();
        if(!n) continue;
        // Read the pivot color
        A = GetBit() ? 1 + GetBit() : 0;
        A += !(A < c);
        // Calculate the two other colors, that are neither c nor A
        for(B = 0;   B==A || B==c; ++B) {}
        for(C = B+1; C==A || C==c; ++C) {}
        // Save
        if(n == 2) { A = B; B = C; } // When count=2, A is _not_ included
        Next0[c] = A; Next1[c] = B; Next2[c] = C;
    }
    if(!extension_persistentrow) { part0=0x00; part1=0x00; }
read_tiles:
    for(unsigned y=0; y<8; output.push_back(part0), plane2[y++] = part1)
        if(!GetBit())
            for(unsigned c=0, x=0; x<8; ++x)
            {
                c = (x==0) ? Get2Bits()
                  : (Count[c]==0 ||  GetBit()!=extension_invertbit) ? c
                  : (Count[c]==1 || !GetBit()) ? Next0[c]
                  : (Count[c]==2 || !GetBit()) ? Next1[c]
                  :                              Next2[c];
                part0 = (part0 << 1) | ((c>>0)&1);
                part1 = (part1 << 1) | ((c>>1)&1);
            }
    output.insert(output.end(), plane2+0, plane2+8);
    if((!num_tiles && bytesremain >= 1) || num_tiles-- > 1)
        { if(GetBit()) goto read_colors; else goto read_tiles; }
    if(diagnostics) std::fprintf(diagnostics, "%u bytes produced, %d bytes remain\n", unsigned(output.size()), bytesremain);
}

#include <unistd.h> // isatty

int main(int argc, char** argv)
{
    std::vector<unsigned char> input;
    std::vector<unsigned char> output;
    bool decompress = false;
    bool force      = false;
    diagnostics = stdout;

    if(argc==1)
    {
        std::fprintf(stderr,
            "Usage: tokumaru [OPTION]... [INFILE] [OUTFILE]\n"
            "Try    %s -h\n", argv[0]);
        return 0;
    }
    while(argv[1][0] == '-' && argv[1][1] != '\0')
    {
        for(char*s = argv[1]+1; *s; ++s)
            switch(*s)
            {
                case 'd':
                    decompress = true;
                    break;
                case 'e':
                {
                    unsigned mask = 1;
                    if(s[1] >= '0' && s[1] <= '9')
                        for(mask=0; s[1]>='0' && s[1]<='9'; ++s)
                            mask = mask*10 + (s[1] - '0');
                    extension_invertbit     = mask & 1;
                    extension_persistentrow = mask & 2;
                    break;
                }
                case 'q':
                    diagnostics = nullptr;
                    break;
                case 't':
                    threads = false;
                    break;
                case 'f':
                    force = true;
                    break;
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7':
                case '8': case '9':
                    compressionlevel = 0;
                    while(*s >= '0' && *s <= '9')
                        compressionlevel = compressionlevel*10 + (*s++ - '0');
                    if(compressionlevel > 16) compressionlevel = 16;
                    --s;
                    break;
                case '-':
                    break;
                case 'h':
                    std::fprintf(diagnostics,
                        "Usage: tokumaru [OPTION]... [INFILE] [OUTFILE]\n"
                        "    -d         Decompress\n"
                        "    -q         Suppress output\n"
                        "    -e[mask]   Enable extensions. Bitmask is optional and is a sum of following values:\n"
                        "                 1 = Invert the first tile color bit (5 bytes smaller decompressor)\n"
                        "                 2 = Enable persistent rows (1 byte smaller decompressor, but worse compression ratio)\n"
                        "               Default if -e is used without options: 1\n"
                    #ifdef _OPENMP
                        "    -t         Don't use threads\n"
                    #else
                        "    -t         Ignored, as this version was compiled without -fopenmp\n"
                    #endif
                        "    -h         Help\n"
                        "    -0 .. -16  Choose compression level (default: 7)\n"
                        "\n"
                        "Author: Joel Yliluoma - http://iki.fi/bisqwit/\n"
                        "Report bugs to: bisqwit@iki.fi\n"
                    );
                    return 0;
            }
        ++argv;
    }

    bool use_stdin = argv[1][0]=='\0' || (argv[1][0]=='-'&&argv[1][1]=='\0');
    bool use_stdout = argv[2][0]=='\0' || (argv[2][0]=='-'&&argv[2][1]=='\0');

    if(diagnostics && use_stdout) diagnostics = stderr;

    if(!argv[2])
    {
        use_stdout = true;
        if(!force && !decompress && isatty(1))
        {
            std::fprintf(stderr,
                "tokumaru: compressed data not written to a terminal. Use -f to force compression.\n"
                "For help, type: tokumaru -h\n");
            return 2;
        }
    }

    std::FILE* fp = use_stdin ? stdin : std::fopen(argv[1], "rb");
    if(!fp) { std::perror(argv[1]); return 1; }
    if(std::fseek(fp, 0, SEEK_END) >= 0)
    {
        input.resize(std::ftell(fp));
        std::rewind(fp);
        std::fread(&input[0], 1, input.size(), fp);
    }
    else
    {
        for(;;)
        {
            unsigned tail = input.size(), increment = 8192;
            input.resize(tail+increment);
            unsigned got = std::fread(&input[tail], 1, increment, fp);
            if(got < increment)
            {
                input.resize(tail+got);
                break;
            }
        }
    }
    std::fclose(fp);

    if(decompress)
        DecompressTiles(&input[0], input.size(), output);
    else
        CompressTiles(&input[0], input.size()/16, [&](unsigned char c) { output.push_back(c); });

    fp = use_stdout ? stdout : std::fopen(argv[2], "wb");
    if(!fp) { std::perror(argv[2]); return 2; }
    std::fwrite(&output[0], 1, output.size(), fp);
    std::fclose(fp);

    if(diagnostics)
    {
        if(decompress)
            std::fprintf(diagnostics,"%u tiles (%u bytes) decompressed into %u bytes\n",
                input[0],
                unsigned(input.size()),
                unsigned(output.size()));
        else
            std::fprintf(diagnostics,"%u tiles (%u bytes) compressed into %u bytes\n",
                unsigned(input.size()/16),
                unsigned(input.size()),
                unsigned(output.size()));
    }
}
