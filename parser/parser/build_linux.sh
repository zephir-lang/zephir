#!/bin/bash
rm -f *.o *.lo

#Compile lemon
if [ ! -f lemon ]; then
    gcc -w lemon.c -o lemon
fi

if [[ `re2c -v` != *re2c* ]]; then
  echo "error: re2c is not installed"
  exit 2
fi

re2c -o scanner.c scanner.re
if [ ! -f scanner.c ]; then
    echo "error: re2c is not installed"
    exit 2
fi

rm -f parser.c
./lemon -s parser.$1.lemon && cat base.c >> parser.$1.c
if [ ! -f parser.$1.c ]; then
    echo "error: couldn't generate parser"
    exit 2
fi

mv -f parser.$1.c parser.c

sed s/"\#line"/"\/\/"/g scanner.c > xx && mv -f xx scanner.c
sed s/"#line"/"\/\/"/g parser.c > xx && mv -f xx parser.c
