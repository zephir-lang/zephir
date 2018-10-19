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

phpenv config-rm zephir-parser.ini || true

pushd "${PROJECT_ROOT}/ext"

$(phpenv which phpize)

./configure --with-php-config=$(phpenv which php-config) --enable-test

make -j"$(getconf _NPROCESSORS_ONLN)"
make install

popd

exit $?
