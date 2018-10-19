#!/usr/bin/env bash

if [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.4" ] || [ "${PHP_MAJOR}.${PHP_MINOR}" = "7.3" ]; then
    echo "Skip test on unstable PHP versions"
    exit 0
fi

shopt -s nullglob

pushd ${TRAVIS_BUILD_DIR}

export PHPUNIT_DONT_EXIT=1
export ZEND_DONT_UNLOAD_MODULES=1
export USE_ZEND_ALLOC=0

valgrind \
    --read-var-info=yes \
    --error-exitcode=1 \
    --fullpath-after= \
    --track-origins=yes \
    --leak-check=full \
    --num-callers=20 \
    --run-libc-freeres=no \
    ./unit-tests/phpunit \
      -c phpunit.xml.dist \
      --no-coverage \
      --testsuite "Extension Test Suite"

popd

exit $?
