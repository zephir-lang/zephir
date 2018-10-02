#!/usr/bin/env bash

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
      --not-exit \
      -c phpunit.xml.dist \
      --no-coverage \
      unit-tests/
