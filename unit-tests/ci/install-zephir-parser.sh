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

export PHP_MAJOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 1)"
export PHP_MINOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 2)"

if [ -z ${ZEPHIR_PARSER_VERSION+x} ]; then
    echo "The ZEPHIR_PARSER_VERSION is unset. Stop."
    exit 1
fi

LOCAL_SRC_DIR=${HOME}/.cache/zephir-parser/src
LOCAL_LIB_DIR=${HOME}/.local/lib
LOCAL_LIBRARY=${LOCAL_LIB_DIR}/zephir-parser-${ZEPHIR_PARSER_VERSION}-${PHP_MAJOR}.${PHP_MINOR}.so

EXTENSION_DIR=`$(phpenv which php-config) --extension-dir`

if [ ! -f ${LOCAL_LIBRARY} ] || [ "${ZEPHIR_PARSER_VERSION}" = "development" ]; then
    mkdir -p ${LOCAL_SRC_DIR}
    mkdir -p ${LOCAL_LIB_DIR}

    rm -rf ${LOCAL_SRC_DIR}/*
    git clone --depth=1 -v https://github.com/phalcon/php-zephir-parser.git -b ${ZEPHIR_PARSER_VERSION} ${LOCAL_SRC_DIR}

    bash ${LOCAL_SRC_DIR}/install

    if [ ! -f "${EXTENSION_DIR}/zephir_parser.so" ]; then
        echo "Unable to locate installed zephir_parser.so"
        exit 1
    fi

    cp "${EXTENSION_DIR}/zephir_parser.so" ${LOCAL_LIBRARY}
fi

echo "[Zephir Parser]" > $(phpenv root)/versions/$(phpenv version-name)/etc/conf.d/zephir-parser.ini
echo "extension=${LOCAL_LIBRARY}" >> $(phpenv root)/versions/$(phpenv version-name)/etc/conf.d/zephir-parser.ini

$(phpenv which php) --ri 'Zephir Parser'
