#!/usr/bin/env bash

# trace ERR through pipes
set -o pipefail

# trace ERR through 'time command' and other functions
set -o errtrace

# set -u : exit the script if you try to use an uninitialised variable
set -o nounset

# set -e : exit the script if any statement returns a non-true return value
set -o errexit

# Ensure that this is being run inside a CI container
if [ "${CI}" != "true" ]; then
    echo "This script is designed to run inside a CI container only. Exiting"
    exit 1
fi

CURRENT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
TRAVIS_BUILD_DIR="${TRAVIS_BUILD_DIR:-$(dirname $(dirname $CURRENT_DIR))}"

PARSER_DIR=$HOME/zephir-parser-${ZEPHIR_PARSER_VERSION}

# Use Travis cache
if [ ! -f ${PARSER_DIR}/unit-tests/ci/install-travis ]; then
    git clone --depth=1 -v https://github.com/phalcon/php-zephir-parser.git -b ${ZEPHIR_PARSER_VERSION} ${PARSER_DIR}
fi

cd ${PARSER_DIR}

if [ -z "${TRAVIS}" ]; then
    bash ./install
else
    bash ./unit-tests/ci/install-travis
fi
