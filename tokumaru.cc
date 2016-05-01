/* Tokumaru tile compression algorithm implementation
 *
 * Copyright (C) 2016 Joel Yliluoma - http://iki.fi/bisqwit/
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
static bool quiet   = false;
static bool threads = true;

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
static FollowupData EncodeColors(const unsigned char* tiles, unsigned numtiles, F&& PutBits)
{
    FollowupData f;
    for(unsigned c=0; c<4; ++c) for(unsigned d=0; d<4; ++d) f.next[c][d] = d + (c==d ? 0 : 4);
    const unsigned char* prevrow = blankrow;
    for(unsigned n=0; n<numtiles; ++n, tiles += 16)
        for(unsigned y=0; y<8; prevrow = tiles + y++)
            if(!IdenticalRows(prevrow, tiles+y))
                for(unsigned d,c=0, x=0; x<8; ++x, c=d)
                {
                    d = ExtractColor(tiles,y,x);
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
    return f;
}

template<typename F>
static void EncodeTilesAndColors(const unsigned char* tiles, unsigned numtiles, F&& PutBits)
{
    FollowupData f = EncodeColors(tiles, numtiles, PutBits);

    const unsigned char* prevrow = blankrow;
    for(unsigned n=0; n<numtiles; ++n, tiles += 16)
    {
        if(n > 0) PutBits(0, 1); // flag for read more tiles
        for(unsigned y=0; y<8; prevrow = tiles + y++)
            if(!PutBits(IdenticalRows(prevrow, tiles+y), 1))
                for(unsigned d,c=0, x=0; x<8; ++x, c=d)
                {
                    d = ExtractColor(tiles,y,x);
                    if(x == 0)
                        PutBits(d, 2);
                    else
                    {
                        if(f.count[c]==0) { assert(d == c); }
                        if(f.count[c]==1) { assert(d == c || d == f.next[c][0]); }
                        if(f.count[c]==2) { assert(d == c || d == f.next[c][0] || d == f.next[c][1]); }

                        if(f.count[c]==0 || PutBits(c==d, 1)
                        || f.count[c]==1 || !PutBits(d!=f.next[c][0], 1)
                        || f.count[c]==2 || !PutBits(d!=f.next[c][1], 1)) continue;
                    }
                }
    }
}

static unsigned CountEncodeTiles(const unsigned char* tiles, unsigned numtiles)
{
    unsigned total=0;
    auto PutBits = [&](unsigned value, unsigned numbits) { total += numbits; return value; };
    EncodeTilesAndColors(tiles, numtiles, PutBits);
    return total;
}

template<typename F>
void EncodeTiles(const unsigned char* tiles, unsigned numtiles, F&& PutBits)
{
    // Benchmark to beat: 4908 (Tokumaru)
    std::vector<unsigned> compress_tree(numtiles*numtiles, ~0u);

    const unsigned MaxLeap = 1u << compressionlevel;

    if(!quiet) std::printf("Creating compression benchmark... ");
    #pragma omp parallel for schedule(static) if(threads)
    for(unsigned begin=0; begin<numtiles; ++begin)
    {
        int tn = 0, nt = 1;
        #ifdef _OPENMP
        tn = omp_get_thread_num(); nt = omp_get_num_threads();
        #endif
        if(tn == 0 && !quiet)
        {
            std::printf("%6.2f%%\b\b\b\b\b\b\b", (begin+1)*100.0/double(numtiles/nt));
            std::fflush(stdout);
        }
        for(unsigned length=std::min(MaxLeap,numtiles-begin); length>0; --length)
        {
            unsigned bits = CountEncodeTiles(tiles+begin*16, length);
            compress_tree[begin*numtiles + length] = bits;
        }
    }
    if(!quiet) std::printf("\n");

    // Figure out the shortest path through the compression tree (use Dijkstra's algorithm)
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
        EncodeTilesAndColors(tiles + position*16, leap, PutBits);
        position += leap;
    }
    assert(position == numtiles);
}

template<typename F>
void EncodeTilesTo(const unsigned char* tiles, unsigned numtiles, F&& PutByte)
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
    PutByte(numtiles);
    EncodeTiles(tiles, numtiles, PutBits);
    // flush bits
    while(bitBuffer != 0x01) PutBits(0,1);
}

static void DecodeTilesFrom(const unsigned char* data, int bytesremain, std::vector<unsigned char>& output)
{
    unsigned BitCount = 0x10000u;
    auto GetBit = [&]() -> bool
    {
        if(BitCount & 0x10000u)
        {
            if(bytesremain-- <= 0) return 0;
            BitCount = *data++ | 0x100u;
        }
        return (BitCount <<= 1) & 0x100u;
    };
    auto Get2Bits = [&]() -> unsigned
    {
        unsigned r = GetBit();
        return r*2 + GetBit();
    };
    unsigned num_tiles = bytesremain-- ? *data++ : 0; if(!num_tiles) num_tiles=512;
    printf("Decoding %u tiles...\n", num_tiles);
    unsigned char Count[4], Next0[4]{0,0,0,0}, Next1[4]{0,0,0,0}, Next2[4]{0,0,0,0};
read_colors:
    for(unsigned c=4; c-- > 0; )
    {
        Count[c] = Get2Bits();
        if(!Count[c]) continue;
        unsigned A = 1;
        if(GetBit()) A += 1 + GetBit(); // Map 0, 10 and 11 into 1,2,3 respectively
        if(c >= A) --A;
        unsigned B = 0;   while(B==A || B==c) ++B;
        unsigned C = B+1; while(C==A || C==c) ++C;
        switch(Count[c])
        {
            case 1: Next0[c] = A; break;
            case 2:               Next0[c] = B; Next1[c] = C; break;
            case 3: Next0[c] = A; Next1[c] = B; Next2[c] = C; break;
        }
        //printf("Next for %u are (count=%u): %d %d %d\n", c, Count[c], Next0[c],Next1[c],Next2[c]);
    }
    unsigned char part0=0x00, part1=0x00, plane2[8];
read_tiles:
    for(unsigned y=0; y<8; output.push_back(part0), plane2[y++] = part1)
        if(!GetBit())
            for(unsigned c = Get2Bits(), d,
                x=0; x<8; ++x,
                c = d,
                part0 = (part0 << 1) | ((c>>0)&1),
                part1 = (part1 << 1) | ((c>>1)&1))
            {
                if(x==0 || !Count[c] || GetBit()) { d=c; continue; }
                // Possible counts: -123. Bits so far: 0
                d = Next0[c];              if(Count[c]==1) continue; // 0  = keep color0 (count=1 only)
                // Possible counts: --23. Bits so far: 0
                if(GetBit()) d = Next1[c]; else continue;            // 00 = keep color0
                // Possible counts: --23. Bits so far: 01
                                           if(Count[c]==2) continue; // 01 = keep color1 (count=2 only)
                // Possible counts: ---3. Bits so far: 01
                if(GetBit()) d = Next2[c];                           // 010 = keep color1, 011 = keep color2
            }
    output.insert(output.end(), plane2+0, plane2+8);
    if(num_tiles-- > 1)
        { if(GetBit()) goto read_colors; else goto read_tiles; }
    printf("%u bytes produced, %d bytes remain\n", unsigned(output.size()), bytesremain);
}

int main(int argc, char** argv)
{
    std::vector<unsigned char> input;
    std::vector<unsigned char> output;
    bool decompress = false;
    if(argc==1)
    {
        std::printf("Usage: tokumaru [OPTION]... [INFILE] [OUTFILE]\n"
                    "Try    %s -h\n", argv[0]);
        return 0;
    }
    while(argv[1][0] == '-')
    {
        for(char*s = argv[1]+1; *s; ++s)
            switch(*s)
            {
                case 'd':
                    decompress = true;
                    break;
                case 'q':
                    quiet = true;
                    break;
                case 't':
                    threads = false;
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
                    std::printf(
                        "Usage: tokumaru [OPTION]... [INFILE] [OUTFILE]\n"
                        "    -d         Decompress\n"
                        "    -q         Suppress output\n"
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

    std::FILE* fp = std::fopen(argv[1], "rb");
    if(!fp) { std::perror(argv[1]); return 1; }
    std::fseek(fp, 0, SEEK_END);
    input.resize(std::ftell(fp));
    std::rewind(fp);
    std::fread(&input[0], 1, input.size(), fp);
    std::fclose(fp);
    if(decompress)
        DecodeTilesFrom(&input[0], input.size(), output);
    else
        EncodeTilesTo(&input[0], input.size()/16, [&](unsigned char c) { output.push_back(c); });

    if(argv[2])
    {
        std::FILE* fp = std::fopen(argv[2], "wb");
        if(!fp) { std::perror(argv[2]); return 2; }
        std::fwrite(&output[0], 1, output.size(), fp);
        std::fclose(fp);
    }

    if(!quiet)
    {
        if(decompress)
            std::printf("%u tiles (%u bytes) decompressed into %u bytes\n",
                input[0],
                unsigned(input.size()),
                unsigned(output.size()));
        else
            std::printf("%u tiles (%u bytes) compressed into %u bytes\n",
                unsigned(input.size()/16),
                unsigned(input.size()),
                unsigned(output.size()));
    }
}
