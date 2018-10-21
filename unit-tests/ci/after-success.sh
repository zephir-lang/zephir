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

c_output=${PROJECT_ROOT}/unit-tests/output/coverage.info
p_output=${PROJECT_ROOT}/unit-tests/output/clover.xml

if [ "x${REPORT_COVERAGE}" = "x1" ]; then
    lcov --no-checksum --directory ${PROJECT_ROOT}/ext --capture --compat-libtool --output-file ${c_output}
    lcov \
        --remove ${c_output} "/usr*" \
        --remove ${c_output} "*/.phpenv/*" \
        --remove ${c_output} "${HOME}/build/include/*" \
        --compat-libtool \
        --output-file ${c_output}
fi

if [ ! -z "${CODECOV_TOKEN}" ]; then
    curl -sSL https://codecov.io/bash -o ./codecov
    chmod +x ./codecov

    ./codecov -s ${PROJECT_ROOT}/unit-tests/output
else
    echo "Skip uploading code coverage..."
fi
