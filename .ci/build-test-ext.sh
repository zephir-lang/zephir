#!/bin/sh
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

# -e  Exit immediately if a command exits with a non-zero status.
# -u  Treat unset variables as an error when substituting.
set -eu

# This allows the configuration of the executable path as follows:
#   ZEPHIR_BIN=zephir.phar .ci/build-test-ext.sh
#   ZEPHIR_BIN=./zephir .ci/build-test-ext.sh
: ${ZEPHIR_BIN:=zephir}

project_root=$(readlink -enq "$(dirname $0)/../")
gcov_report=${project_root}/unit-tests/output/lcov.info

alias zephir="${ZEPHIR_BIN}"

zephir clean 2>&1 || exit 1
zephir fullclean 2>&1 || exit 1

# TODO: Export ZFLAGS and process by Config class
ZFLAGS=""
# ZFLAGS="${ZFLAGS} -Wnonexistent-function -Wnonexistent-class -Wunused-variable -Wnonexistent-constant"
# ZFLAGS="${ZFLAGS} -Wunreachable-code -Wnot-supported-magic-constant -Wnon-valid-decrement"

zephir generate ${ZFLAGS} 2>&1 || exit 1
zephir stubs ${ZFLAGS} 2>&1 || exit 1
zephir api ${ZFLAGS} 2>&1 || exit 1

cd ext

phpize

if [ ! -z ${COLLECT_COVERAGE+x} ] && [ "$COLLECT_COVERAGE" = "true" ]; then
  # The ltmain.sh which bundled with PHP it's from libtool 1.5.26.
  # However, the version of libtool that claims to no longer remove
  # ".gcno" profiler information is libtool 2.2.6. The fix is probably
  # in later libtool versions as well.
  if [ "$(uname -s 2>/dev/null)" = "Darwin" ]; then
    # macOS
    libtoolize_bin=$(command -v glibtoolize 2>/dev/null)
  else
    # Linux, Gentoo, etc
    libtoolize_bin=$(command -v libtoolize 2>/dev/null)
  fi

  aclocal && ${libtoolize_bin} --copy --force && autoheader && autoconf

  CFLAGS=`echo "${CFLAGS}" | sed -e 's/-O[0-9s]*//g'`
  CXXFLAGS=`echo "${CXXFLAGS}" | sed -e 's/-O[0-9s]*//g'`
  LDFLAGS=`echo "${LDFLAGS}" | sed -e 's/--coverage//g'`

  LDFLAGS="${LDFLAGS} --coverage"
  CFLAGS="${CFLAGS} -O0 -ggdb -fprofile-arcs -ftest-coverage"
  CXXFLAGS="${CXXFLAGS} -O0 -ggdb -fprofile-arcs -ftest-coverage"
fi

if [ ! -z ${CFLAGS+x} ] && [ ! -z ${LDFLAGS+x} ]; then
  ./configure --enable-test CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"
else
  ./configure --enable-test
fi

make -j"$(getconf _NPROCESSORS_ONLN)"

cd ..

if [ ! -z ${COLLECT_COVERAGE+x} ] && [ "$COLLECT_COVERAGE" = "true" ]; then
  if [  $(command -v lcov 2>/dev/null) = ""  ]; then
    printf "lcov does not exist.\nSkip capturing coverage data.\n"
  else
    # Reset all execution counts to zero
    lcov \
      --quiet \
      --directory ext \
      --base-directory=${project_root} \
      --zerocounters 2>/dev/null

    # Capture coverage data
    lcov \
      --quiet \
      --directory ext \
      --base-directory=${project_root} \
      --capture \
      --compat-libtool \
      --initial \
      --output-file ${gcov_report} 2>/dev/null

    # FIXME: Fix the report
    # 	geninfo: WARNING: could not open ${PROJECT_ROOT}/kernel/fcall.h
    # 	geninfo: WARNING: some exclusion markers may be ignored
    sed -i.bak s_${project_root}/kernel_${project_root}/ext/kernel_g ${gcov_report}
  fi
fi

exit $?
