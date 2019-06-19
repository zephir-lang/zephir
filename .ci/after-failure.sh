#!/bin/bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

# -e	Exit immediately if a command exits with a non-zero status.
# -u	Treat unset variables as an error when substituting.
set -eu

shopt -s nullglob

export LC_ALL=C

ls -al

for i in /tmp/core.*; do
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

$(phpenv which php) -m