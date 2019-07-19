#!/bin/sh
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

# -e  Exit immediately if a command exits with a non-zero status.
# -u  Treat unset variables as an error when substituting.
set -eu

project_root=$(readlink -enq "$(dirname $0)/../")

gcov_report=${project_root}/unit-tests/output/lcov.info
phpunit_report=${project_root}/unit-tests/output/clover.xml

if [ -z ${COLLECT_COVERAGE+x} ] || [ "$COLLECT_COVERAGE" != "true" ]; then
  printf "Uploading coverage data is not enabled.\nSkip uploading reports to Codecov.\n"
  exit 0
fi

if [ $(command -v lcov 2>/dev/null) = "" ]; then
  printf "lcov does not exist.\nSkip capturing coverage data.\n"
else
  # Capture coverage data
  lcov \
    --quiet \
    --no-checksum \
    --directory ext \
    --base-directory=${project_root} \
    --capture \
    --compat-libtool \
    --output-file ${gcov_report} 2>/dev/null

  # Remove files matching non-project patterns
  lcov \
    --quiet \
    --remove ${gcov_report} "/usr*" \
    --remove ${gcov_report} "${HOME}/.phpenv/*" \
    --compat-libtool \
    --output-file ${gcov_report} 2>/dev/null

  # FIXME: Fix the report
  #   Cannot open source file ${PROJECT_ROOT}/kernel/fcall.h
  sed -i.bak s_${project_root}/kernel_${project_root}/ext/kernel_g ${gcov_report}
fi

# Note: to upload a coverage report, set the CODECOV_TOKEN environment variable
#    export CODECOV_TOKEN=<codecov token>

curl -sSl https://codecov.io/bash -o "${HOME}/bin/codecov"
chmod +x "${HOME}/bin/codecov"

if [ -f ${gcov_report} ]; then
  codecov -f ${gcov_report}
fi

if [ -f ${phpunit_report} ]; then
  codecov -f ${phpunit_report}
fi
