#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../../")

shopt -s nullglob

export PHP_MAJOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 1)"
export PHP_MINOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 2)"

if [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.3" ] || [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.4" ]; then
    export USE_ZEND_ALLOC=1

    composer remove --dev phpunit/phpunit
    composer require --dev ${DEFAULT_COMPOSER_FLAGS} "phpunit/phpunit:^7.4"

    ${PROJECT_ROOT}/unit-tests/phpunit ${PROJECT_ROOT}/unit-tests
else
    ${PROJECT_ROOT}/unit-tests/phpunit ${PROJECT_ROOT}/unit-tests
    ${PROJECT_ROOT}/unit-tests/ci/memcheck.sh
fi
