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

re2c -o scanner.c scanner.re && ./lemon -s parser.lemon && cat base.c >> parser.c
if [ ! -f parser.c ]; then
  echo "error: re2c is not installed"
  exit 2
fi

sed s/"\#line"/"\/\/"/g scanner.c > xx && mv -f xx scanner.c
sed s/"#line"/"\/\/"/g parser.c > xx && mv -f xx parser.c
