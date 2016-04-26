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

re2c -o scanner.c scanner.re && ./lemon -s parser.php7.lemon && cat base.c >> parser.php7.c
if [ ! -f parser.php7.c ]; then
  echo "error: re2c is not installed"
  exit 2
fi

mv -f parser.php7.c parser.c

sed s/"\#line"/"\/\/"/g scanner.c > xx && mv -f xx scanner.c
sed s/"#line"/"\/\/"/g parser.c > xx && mv -f xx parser.c
