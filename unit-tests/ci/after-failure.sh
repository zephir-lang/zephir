#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

shopt -s nullglob

$(phpenv which php) -v
$(phpenv which php) -m

export LC_ALL=C

for i in core core*; do
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
