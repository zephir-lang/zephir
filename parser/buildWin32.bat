REM Build Lemon
setlocal
set ARCH=x86
set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC"
call %VS_PATH%\vcvarsall.bat %ARCH%
@echo on

del parser.c parser.h scanner.c
cl lemon.c
re2c -o scanner.c scanner.re
lemon -s parser.lemon
cat base.c >> parser.c
del parser.exe
cl /MD /I .. parser.c scanner.c json-c.lib /link