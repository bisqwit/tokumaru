OBJS=decompress.o main.o

test.nes: $(OBJS) link.cfg
	ld65 -o "$@" $(OBJS) -C link.cfg -m test.map

make_graphics: graphics.cc \
		avatar/finger1.inc \
		avatar/finger2.inc \
		avatar/happycup1.inc \
		avatar/happycup2.inc \
		avatar/helmet.inc
	g++ -o "$@" "$<" -O0 -std=c++14

test_page0.bin: tokumaru make_graphics
	./make_graphics | dd bs=4096 skip=0 count=1 | ./tokumaru -e -16 - "$@"

test_page1.bin: tokumaru make_graphics
	./make_graphics | dd bs=4096 skip=1 count=1 | ./tokumaru -e -16 - "$@"

tokumaru: ../tokumaru.cc
	make -C .. tokumaru
	ln -f ../tokumaru "$@"

.s.o:
	ca65 --cpu 6502 -g --smart -t nes -o "$@" "$<"

main.o: main.s test_page0.bin test_page1.bin macros.inc


clean:
	rm -f $(OBJS) test_page0.bin test_page1.bin tokumaru test.nes make_graphics test.map
