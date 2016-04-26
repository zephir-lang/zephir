REM Build Lemon
cl lemon.c
del parser.c parser.h scanner.c
re2c -o scanner.c scanner.re
lemon -s parser.php5.lemon
type parser.php5.c > parser.c
type base.c >> parser.c
