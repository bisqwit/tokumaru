<?php
//TITLE=Codemasters/Tokumaru NES tile compressor+decompressor

$title = 'Tile decompressor+decompressor';
$progname = 'tokumaru';

$text = array(
   '1. Purpose' => "

Compress and decompress bitmaps.
 <p>
<img src=\"/src/tokumaru-demo.gif\" alt=\"Demo screenshot\" title=\"Demo screenshot\">
 <p>
In tests, it usually achieves about 40% reduction in data size,
and as such, compresses better than any other known image
compressor that targets the NES/Famicom platform.<br>
It is based on Tokumaru's compression tool, which itself
is based on commercial compression &amp; decompression algorithm
used by some Codemasters games. Hence the name.

 <p>
The distribution includes: <ul>
 <li>A commandline compressor &amp; uncompressor tool, written in C++14</li>
 <li>An example CA65-compatible NES project and a 6502 decompressor for the compression scheme</li>
</ul>
 <p>
In theory, the tool can compress any data as long its size
is a multiple of 16 bytes (size of a NES tile), but it is
optimized and designed for NES bitmaps.
It will not do very well with other kinds of data.

", '1. Copying' => "

This program has been written by Joel Yliluoma, a.k.a.
<a href=\"http://iki.fi/bisqwit/\">Bisqwit</a>,<br>
and is distributed under the terms of the Zlib license.

");
include '/WWW/progdesc.php';
