#!/usr/bin/env bash

echo -e "Install Zephir Parser..."

CURRENT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
TRAVIS_BUILD_DIR="${TRAVIS_BUILD_DIR:-$(dirname $(dirname $CURRENT_DIR))}"

git clone --depth=1 -v https://github.com/phalcon/php-zephir-parser.git /tmp/parser
cd /tmp/parser

# Only for Travis CI
TRAVIS_BUILD_DIR=$(pwd) bash ./tests/ci/install-travis

cd ${TRAVIS_BUILD_DIR}
