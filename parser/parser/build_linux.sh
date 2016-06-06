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

#! /bin/sh

rm -f parser.o
rm -f parser.lo
rm -f scanner.o
rm -f scanner.lo

if [ ! -f lemon ]; then
	gcc -g lemon.c -o lemon
fi

re2c -o scanner.c scanner.re

./lemon -s parser.php5.lemon
if [ ! -f parser.php5.c ]; then
    echo "error: couldn't generate parser"
    exit 2
fi

./lemon -s parser.php7.lemon
if [ ! -f parser.php7.c ]; then
    echo "error: couldn't generate parser"
    exit 2
fi

echo "#include <php.h>" > parser.c
echo "#if PHP_VERSION_ID < 70000" >> parser.c
cat parser.php5.c >> parser.c
echo "#else" >> parser.c
cat parser.php7.c >> parser.c
echo "#endif" >> parser.c
cat base.c >> parser.c

sed s/"\#line"/"\/\/"/g scanner.c > xx && mv -f xx scanner.c
sed s/"#line"/"\/\/"/g parser.c > xx && mv -f xx parser.c
