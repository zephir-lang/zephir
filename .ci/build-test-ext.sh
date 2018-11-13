#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

# Exit the script if any statement returns a non-true return value
set -e

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../")
DEFAULT_ZFLAGS="-Wnonexistent-function -Wnonexistent-class -Wunused-variable"

shopt -s nullglob

./zephir clean 2>&1
./zephir fullclean 2>&1
./zephir generate ${DEFAULT_ZFLAGS} 2>&1
./zephir stubs 2>&1
./zephir api 2>&1

cd ext

phpize

CFLAGS="${CFLAGS}"
LDFLAGS="${LDFLAGS}"

if [ "${REPORT_COVERAGE}" = "1" ]; then
	# The ltmain.sh which bundled with PHP it's from libtool 1.5.26.
	# However, the version of libtool that claims to no longer remove
	# ".gcno" profiler information is libtool 2.2.6. The fix is probably
	# in later libtool versions as well.
	if [ "$(uname -s 2>/dev/null)" = "Darwin" ]; then
		# macOS
		LIBTOOLIZE_BIN=$(command -v glibtoolize 2>/dev/null)
	else
		# Linux, Gentoo, etc
		LIBTOOLIZE_BIN=$(command -v libtoolize 2>/dev/null)
	fi

	aclocal && ${LIBTOOLIZE_BIN} --copy --force && autoheader && autoconf

	CFLAGS="--coverage -fprofile-arcs -ftest-coverage $CFLAGS"
	LDFLAGS="--coverage ${LDFLAGS}"
fi

./configure --enable-test CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"

make -j"$(getconf _NPROCESSORS_ONLN)"

cd ..

if [ "${REPORT_COVERAGE}" = "1" ]; then
	lcov --directory ./ext --zerocounters
	lcov --directory ./ext --capture --compat-libtool --initial --output-file ./unit-tests/output/coverage.info
fi

exit $?
