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

# Ensure that this is being run inside a CI container
if [ "${CI}" != "true" ]; then
    echo "This script is designed to run inside a CI container only. Exiting"
    exit 1
fi

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../../")

shopt -s nullglob

pushd "${PROJECT_ROOT}/ext"
pushd "${PROJECT_ROOT}"

$(phpenv which php) compiler.php help
$(phpenv which php) compiler.php clean
$(phpenv which php) compiler.php fullclean
$(phpenv which php) compiler.php generate ${DEFAULT_ZFLAGS}
$(phpenv which php) compiler.php stubs >/dev/null 2>&1
$(phpenv which php) compiler.php api >/dev/null 2>&1

popd

phpenv config-rm zephir-parser.ini || true

$(phpenv which phpize)

# However, the version of libtool that claims to no longer remove .gcno profiler information is libtool 2.2.6.
# The fix is probably in later libtool versions as well.
if [ -f /etc/gentoo-release ]; then
    # Gentoo Linux
    LIBTOOLIZE_BIN=$(command -v libtoolize 2>/dev/null)
    aclocal && ${LIBTOOLIZE_BIN} --copy --force && autoheader && autoconf
elif [ "$(uname -s 2>/dev/null)" = "Darwin" ]; then
    # macOS
    LIBTOOLIZE_BIN=$(command -v glibtoolize 2>/dev/null)
    aclocal && ${LIBTOOLIZE_BIN} --copy --force && autoheader && autoconf
else
    # Linux
    aclocal && libtoolize --copy --force && autoheader && autoconf
fi

CFLAGS="${CFLAGS}"
LDFLAGS="${LDFLAGS}"

if [ "${REPORT_COVERAGE}" = "1" ]; then
    CFLAGS="--coverage -fprofile-arcs -ftest-coverage $CFLAGS"
    LDFLAGS="--coverage ${LDFLAGS}"
fi

./configure --with-php-config=$(phpenv which php-config) --enable-test CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"

make -j"$(getconf _NPROCESSORS_ONLN)"

if [ "x${REPORT_COVERAGE}" = "x1" ]; then
    output=${PROJECT_ROOT}/unit-tests/output/coverage.info

    lcov --directory ${PROJECT_ROOT}/ext --zerocounters
    lcov --directory ${PROJECT_ROOT}/ext --capture --compat-libtool --initial --output-file ${output}
fi

popd

exit $?
