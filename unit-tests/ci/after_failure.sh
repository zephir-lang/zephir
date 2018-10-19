#!/usr/bin/env bash

echo "Used ZendEngine varibales:"
echo "  ZEND_DONT_UNLOAD_MODULES=${ZEND_DONT_UNLOAD_MODULES}"
echo "  USE_ZEND_ALLOC=${USE_ZEND_ALLOC}"
echo "PCRE JIT:"
php -r "echo ' pcre.jit=' . ini_get('pcre.jit') . PHP_EOL;"
echo "Stack size:"
ulimit -a | grep stack
echo "glibc version:"
ldd --version | head -n 1

shopt -s nullglob

export LC_ALL=C

for i in core*; do
	if [ -f "$i" -a "$(file "$i" | grep -o 'core file')" ]; then
		gdb -q $(phpenv which php) "$i" <<EOF
set pagination 0
backtrace full
info registers
x/16i \$pc
thread apply all backtrace
quit
EOF
	fi
done
