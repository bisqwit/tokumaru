VERSION=1.0.0
CXXFLAGS=-std=c++14 -Wall -Ofast -fopenmp -Wextra -march=native
ARCHDIR=archives/
ARCHFILES=\
	tokumaru.cc algorithm.txt \
	progdesc.php README
ARCHNAME=tokumaru-$(VERSION)

all: tokumaru tokumaru-w32.exe tokumaru-w64.exe

tokumaru-w32.exe: tokumaru.cc
	i686-w64-mingw32-g++   -o "$@" "$<" $(CXXFLAGS) -s -static-libgcc -static-libstdc++ -Wl,-static -lgomp -lpthread -Wl,-shared
	- upx-nrv "$@" || upx-ucl "$@"
	rm -f tokumaru-w32.{000,001,upx}

tokumaru-w64.exe: tokumaru.cc
	x86_64-w64-mingw32-g++ -o "$@" "$<" $(CXXFLAGS) -s -static-libgcc -static-libstdc++ -Wl,-static -lgomp -lpthread -Wl,-shared
	- upx-nrv "$@" || upx-ucl "$@"
	rm -f tokumaru-w64.{000,001,upx}

tokumaru: tokumaru.cc
	g++ -o "$@" "$<" $(CXXFLAGS)

source.zip: tokumaru.cc Makefile algorithm.txt README
	zip -9 "$@" $^
	- advzip -z4 -i512 "$@"
	- DeflOpt "$@"

test:	tokumaru smb1a.dat kirbyu.dat
	./tokumaru smb1a.dat  tmp1
	./tokumaru kirbyu.dat tmp2
	./tokumaru -d tmp1 tmp1out
	./tokumaru -d tmp2 tmp2out
	cmp smb1a.dat  tmp1out
	cmp kirbyu.dat tmp2out
	rm -f tmp1 tmp2 tmp1out tmp2out


clean:
	rm -f tokumaru tokumaru-w32.exe tokumaru-w64.exe

include depfun.mak
