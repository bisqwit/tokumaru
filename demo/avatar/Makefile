AM=--dc=4 --gamma=2.95 --dm=4x4 --dr=2,1,4

all: finger1.inc finger2.inc happycup1.inc happycup2.inc helmet.inc

finger1.png: fingerpointing1.png Makefile
	convert "$<" -resize 64x64'!' tmp-"$@"
	~/src/animmerger/animmerger -pc tmp-"$@" --noalign -Q000000,2038EC,C84C0C,C5C5C5 $(AM) --gif=never -o"$@"
	advpng -z1 "$@"
finger1.xpm: finger1.png
	mogrify -format xpm "$<"
finger1.inc: finger1.xpm convert.sh
	./convert.sh "$<" 88 > "$@"


finger2.png: fingerpointing2.png Makefile
	convert "$<" -resize 64x64'!' tmp-"$@"
	~/src/animmerger/animmerger -pc tmp-"$@" --noalign -Q000000,2038EC,C84C0C,C5C5C5 $(AM) --gif=never -o"$@"
	advpng -z1 "$@"
finger2.xpm: finger2.png
	mogrify -format xpm "$<"
finger2.inc: finger2.xpm convert.sh
	./convert.sh "$<" 188 > "$@"

AM2=--dc=4 --gamma=2 --dm=4x4 --dr=1 --de 0.25

happycup1.png: happycup.png Makefile
	convert "$<" -resize 64x64'!' tmp-"$@"
	~/src/animmerger/animmerger -pc tmp-"$@" --noalign -Q000000,2038EC,C84C0C,C5C5C5 $(AM2) --gif=never -o"$@"
	advpng -z1 "$@"
happycup1.xpm: happycup1.png
	mogrify -format xpm "$<"
happycup1.inc: happycup1.xpm convert.sh
	./convert.sh "$<" 108 > "$@"

AM3=--dc=16 --gamma=4 --dm=4x4

happycup2.png: neutralcup.png Makefile
	convert "$<" -resize 64x64'!' tmp-"$@"
	~/src/animmerger/animmerger -pc tmp-"$@" --noalign -Q000000,2038EC,C84C0C,C5C5C5 $(AM3) --gif=never -o"$@"
	advpng -z1 "$@"
happycup2.xpm: happycup2.png
	mogrify -format xpm "$<"
happycup2.inc: happycup2.xpm convert.sh
	./convert.sh "$<" 180 > "$@"

AM4=--dc=16 --gamma=2 --dm=4x4

helmet.png: helmet.gif Makefile
	convert -background '#FFFFFF' "$<" -resize 64x62'!' -type truecolor tmp-"$@".pcx
	convert tmp-"$@"-0.pcx tmp-"$@" ; rm tmp-"$2"*.pcx
	~/src/animmerger/animmerger -pc tmp-"$@" --noalign -Q000000,2038EC,C84C0C,C5C5C5 $(AM4) --gif=never -o"$@"
	advpng -z1 "$@"
helmet.xpm: helmet.png
	mogrify -format xpm "$<"
helmet.inc: helmet.xpm convert.sh
	./convert.sh helmet.xpm 80 > "$@"
