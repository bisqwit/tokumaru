OBJS=decompress.o main.o

test.nes: $(OBJS) link.cfg
	ld65 -o "$@" $(OBJS) -C link.cfg -m test.map

make_graphics: graphics.cc
	g++ -o "$@" "$<" -O0 -std=c++14

test_page0.bin: tokumaru make_graphics
	./make_graphics | ./tokumaru -e16 - "$@"

tokumaru: ../tokumaru.cc
	make -C .. tokumaru
	ln -f ../tokumaru "$@"

.s.o:
	ca65 --cpu 6502 -g --smart -t nes -o "$@" "$<"

main.o: main.s test_page0.bin


clean:
	rm -f $(OBJS) test_page0.bin tokumaru test.nes make_graphics test.map
