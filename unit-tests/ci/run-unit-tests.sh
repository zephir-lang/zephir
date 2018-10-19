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

pushd ${PROJECT_ROOT}

export PHP_MAJOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 1)"
export PHP_MINOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 2)"
export PHPUNIT_DONT_EXIT=1

confing_file=${PROJECT_ROOT}/phpunit.xml.dist

if [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.3" ] || [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.4" ]; then
    export USE_ZEND_ALLOC=1
    confing_file=${PROJECT_ROOT}/phpunit-php-73.xml.dist
fi

$(phpenv which php) -d extension=ext/modules/test.so unit-tests/phpunit -c ${confing_file}
status=$?

popd

exit ${status}
