CXXFLAGS=-std=c++14 -Wall -Ofast -fopenmp -Wextra -march=native

all: tokumaru tokumaru-w32.exe tokumaru-w64.exe

tokumaru-w32.exe: tokumaru.cc
	i686-w64-mingw32-g++   -o "$@" "$<" $(CXXFLAGS) -s -static-libgcc -static-libstdc++ -Wl,-static -lgomp -lpthread -Wl,-shared
	- upx-nrv "$@" || upx-ucl "$@"

tokumaru-w64.exe: tokumaru.cc
	x86_64-w64-mingw32-g++ -o "$@" "$<" $(CXXFLAGS) -s -static-libgcc -static-libstdc++ -Wl,-static -lgomp -lpthread -Wl,-shared
	- upx-nrv "$@" || upx-ucl "$@"

tokumaru: tokumaru.cc
	g++ -o "$@" "$<" $(CXXFLAGS)


clean:
	rm -f tokumaru tokumaru-w32.exe tokumaru-w64.exe
