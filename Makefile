VERSION=1.3.0
CXXFLAGS=-std=c++14 -Wall -Ofast -fopenmp -Wextra -march=native
ARCHDIR=archives/
ARCHFILES=\
	tokumaru.cc algorithm.txt \
	demo/decompress.s \
	demo/graphics.cc \
	demo/link.cfg \
	demo/main.s \
	demo/Makefile \
	demo/macros.inc \
	demo/avatar/finger1.inc \
	demo/avatar/finger2.inc \
	demo/avatar/happycup1.inc \
	demo/avatar/happycup2.inc \
	demo/avatar/helmet.inc \
	demo/test_page0.bin \
	demo/test_page1.bin \
	progdesc.php README
ARCHNAME=tokumaru-$(VERSION)

all: tokumaru tokumaru-w32.exe tokumaru-w64.exe

tokumaru-w32.exe: tokumaru.cc
	i686-w64-mingw32-g++   -o "$@" "$<" $(CXXFLAGS) -s -static-libgcc -static-libstdc++ -Wl,-static -lgomp -lpthread -Wl,-shared
	- upx-nrv --best --ultra-brute "$@" || upx-ucl --best --ultra-brute "$@"
	rm -f tokumaru-w32.{000,001,upx}

tokumaru-w64.exe: tokumaru.cc
	x86_64-w64-mingw32-g++ -o "$@" "$<" $(CXXFLAGS) -s -static-libgcc -static-libstdc++ -Wl,-static -lgomp -lpthread -Wl,-shared
	- upx-nrv --best --ultra-brute "$@" || upx-ucl --best --ultra-brute "$@"
	rm -f tokumaru-w64.{000,001,upx}

tokumaru: tokumaru.cc
	g++ -o "$@" "$<" $(CXXFLAGS)

source.zip: tokumaru.cc Makefile algorithm.txt README
	zip -9 "$@" $^
	- advzip -z4 -i512 "$@"
	- DeflOpt "$@"

test:	tokumaru smb1a.dat kirbyu.dat
	#
	bash -c 'cmp smb1a.dat  <(./tokumaru smb1a.dat -  | ./tokumaru -d - -)'
	bash -c 'cmp kirbyu.dat <(./tokumaru kirbyu.dat - | ./tokumaru -d - -)'
	#
	bash -c 'cmp smb1a.dat  <(./tokumaru -e smb1a.dat -  | ./tokumaru -ed - -)'
	bash -c 'cmp kirbyu.dat <(./tokumaru -e kirbyu.dat - | ./tokumaru -ed - -)'

demo/test.nes: demo/decompress.s demo/main.s demo/link.cfg demo/graphics.cc tokumaru demo/main.s
	make -C demo test.nes

clean:
	rm -f tokumaru tokumaru-w32.exe tokumaru-w64.exe
	make -C demo clean

include depfun.mak
