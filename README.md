# Tokumaru compressor+decompressor

Compress and decompress bitmaps.

![Demo screenshot](http://bisqwit.iki.fi/src/tokumaru-demo.gif)

In tests, it usually achieves about 40% reduction in data size,
and as such, compresses better than any other known image
compressor that targets the NES/Famicom platform.    
It is based on Tokumaru's compression tool, which itself
is based on commercial compression & decompression algorithm
used by some Codemasters games. Hence the name.

The distribution includes:
* A commandline compressor & uncompressor tool, written in C++14
* An example CA65-compatible NES project and a 6502 decompressor for the compression scheme

In theory, the tool can compress any data as long its size
is a multiple of 16 bytes (size of a NES tile), but it is
optimized and designed for NES bitmaps.
It will not do very well with other kinds of data.

## Copying

This program has been written by Joel Yliluoma, a.k.a.
[Bisqwit](http://iki.fi/bisqwit/),    
and is distributed under the terms of the Zlib license.
