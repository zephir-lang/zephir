REM Build Lemon
cl lemon.c
del parser.c parser.h scanner.c
re2c -o scanner.c scanner.re
lemon -s parser.lemon
type base.c >> parser.c
