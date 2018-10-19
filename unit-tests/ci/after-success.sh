#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

if [ "${CI}" != "true" ]; then
    echo "This script is designed to run inside a CI container only. Stop."
    exit 1
fi

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../../")

if [ "x${REPORT_COVERAGE}" = "x1" ]; then
    output=${PROJECT_ROOT}/unit-tests/output/coverage.info

    lcov --no-checksum --directory ${PROJECT_ROOT}/ext --capture --compat-libtool --output-file ${output}
    lcov \
        --remove ${output} "/usr*" \
        --remove ${output} "*/.phpenv/*" \
        --remove ${output} "${HOME}/build/include/*" \
        --compat-libtool \
        --output-file ${output}

    coveralls-lcov ${output}
fi
