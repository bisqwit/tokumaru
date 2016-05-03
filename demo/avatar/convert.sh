code=$[0x$2]
c0="$(grep black  "$1"|sed 's/^.//;s/ c.*//')"
c1="$(grep C5C5C5 "$1"|sed 's/^.//;s/ c.*//')"
c2="$(grep 2038EC "$1"|sed 's/^.//;s/ c.*//')"
c3="$(grep C84C0C "$1"|sed 's/^.//;s/ c.*//')"
#echo "c0=$c0 c1=$c1 c2=$c2 c3=$c3"

echo 'R"('

egrep  '"......................' "$1" \
	| tr -d '",' \
	| tr "$c0""$c1""$c2""$c3" ' c#.' \
	| perl -pe 's/(........)/:\1/g;s/$/|/' |\
while read s; do
	for n in 0 1 2 3 4 5 6 7; do
		printf '$%03X     ' $((code+n))
	done
	echo ';'
	        if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	read s; if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	read s; if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	read s; if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	read s; if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	read s; if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	read s; if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	read s; if [ "$s" = "" ]; then printf '%-9s%-9s%-9s%-9s%-9s%-9s%-9s%-9s!\n' : : : : : : : : ;else echo "$s"; fi
	code=$((code+16))
done

echo ')"'
