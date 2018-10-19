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

export PHP_MAJOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 1)"
export PHP_MINOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 2)"

if [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.4" ] || [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.3" ]; then
    echo "Skip test on unstable PHP versions"
    exit 0
fi

pushd ${PROJECT_ROOT}

export PHPUNIT_DONT_EXIT=1
export ZEND_DONT_UNLOAD_MODULES=1
export USE_ZEND_ALLOC=0

phpenv config-rm xdebug.ini || true
phpenv config-rm zephir-parser.ini || true

valgrind \
    --read-var-info=yes \
    --error-exitcode=1 \
    --fullpath-after= \
    --track-origins=yes \
    --leak-check=full \
    --num-callers=20 \
    --run-libc-freeres=no \
    $(phpenv which php) -d extension=ext/modules/test.so unit-tests/phpunit \
      -c phpunit.xml.dist \
      --no-coverage \
      --testsuite "Extension Test Suite"

popd

exit $?
