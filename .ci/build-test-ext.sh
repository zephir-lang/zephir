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
LCOV_REPORT=${PROJECT_ROOT}/unit-tests/output/lcov.info

# TODO: Export ZFLAGS and process by Config class
ZFLAGS="-Wnonexistent-function -Wnonexistent-class -Wunused-variable -Wnonexistent-constant"
ZFLAGS="${ZFLAGS} -Wunreachable-code -Wnot-supported-magic-constant -Wnon-valid-decrement"

shopt -s nullglob

zephir clean 2>&1 || exit 1
zephir fullclean 2>&1 || exit 1
zephir generate ${ZFLAGS} 2>&1 || exit 1
zephir stubs ${ZFLAGS} 2>&1 || exit 1
zephir api ${ZFLAGS} 2>&1 || exit 1

cd ext

phpize

if [[ ! -z ${REPORT_COVERAGE+x} ]] && [[ "$REPORT_COVERAGE" = "true" ]]; then
	# The ltmain.sh which bundled with PHP it's from libtool 1.5.26.
	# However, the version of libtool that claims to no longer remove
	# ".gcno" profiler information is libtool 2.2.6. The fix is probably
	# in later libtool versions as well.
	if [[ "$(uname -s 2>/dev/null)" = "Darwin" ]]; then
		# macOS
		LIBTOOLIZE_BIN=$(command -v glibtoolize 2>/dev/null)
	else
		# Linux, Gentoo, etc
		LIBTOOLIZE_BIN=$(command -v libtoolize 2>/dev/null)
	fi

	aclocal && ${LIBTOOLIZE_BIN} --copy --force && autoheader && autoconf

	CFLAGS=`echo "${CFLAGS}" | sed -e 's/-O[0-9s]*//g'`
	CXXFLAGS=`echo "${CXXFLAGS}" | sed -e 's/-O[0-9s]*//g'`
	LDFLAGS=`echo "${LDFLAGS}" | sed -e 's/--coverage//g'`

	LDFLAGS="${LDFLAGS} --coverage"
	CFLAGS="${CFLAGS} -O0 -ggdb -fprofile-arcs -ftest-coverage"
	CXXFLAGS="${CXXFLAGS} -O0 -ggdb -fprofile-arcs -ftest-coverage"
fi

./configure --enable-test CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"

make -j"$(getconf _NPROCESSORS_ONLN)"

cd ..

if [[ ! -z ${REPORT_COVERAGE+x} ]] && [[ "$REPORT_COVERAGE" = "true" ]]; then
	if [[ $(command -v lcov 2>/dev/null) = "" ]]; then
		echo -e "lcov does not exist.\nSkip capturing coverage data."
	else
		# Reset all execution counts to zero
		lcov \
			--quiet \
			--directory ext \
			--base-directory=${PROJECT_ROOT} \
			--zerocounters 2>/dev/null

		# Capture coverage data
		lcov \
			--quiet \
			--directory ext \
			--base-directory=${PROJECT_ROOT} \
			--capture \
			--compat-libtool \
			--initial \
			--output-file ${LCOV_REPORT} 2>/dev/null

		# FIXME: Fix the report
		# 	geninfo: WARNING: could not open ${PROJECT_ROOT}/kernel/fcall.h
		# 	geninfo: WARNING: some exclusion markers may be ignored
		sed -i.bak s_${PROJECT_ROOT}/kernel_${PROJECT_ROOT}/ext/kernel_g ${LCOV_REPORT}
	fi
fi

exit $?
