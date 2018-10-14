#!/usr/bin/env bash

set -e
cd ${TRAVIS_BUILD_DIR}/ext
$(phpenv which phpize)
./configure --silent --with-php-config=$(phpenv which php-config) --enable-test
make ${DEFAULT_MAKE_FLAGS} -j"$(getconf _NPROCESSORS_ONLN)"
make ${DEFAULT_MAKE_FLAGS} install
