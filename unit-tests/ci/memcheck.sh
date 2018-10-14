#!/usr/bin/env bash

export PHPUNIT_DONT_EXIT=1
export ZEND_DONT_UNLOAD_MODULES=1
export USE_ZEND_ALLOC=0

cd ${TRAVIS_BUILD_DIR}

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
