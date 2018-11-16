#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

set -e +o pipefail

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../")
LCOV_REPORT=${PROJECT_ROOT}/unit-tests/output/lcov.info
PHPUNIT_REPORT=${PROJECT_ROOT}/unit-tests/output/clover.xml

if [[ $(command -v lcov 2>/dev/null) = "" ]]; then
	echo -e "lcov does not exist.\nSkip capturing coverage data."
else
	# Capture coverage data
	lcov \
		--quiet \
		--no-checksum \
		--directory ext \
		--base-directory=${PROJECT_ROOT} \
		--capture \
		--compat-libtool \
		--output-file ${LCOV_REPORT}

	# Remove files matching non-project patterns
	lcov \
		--quiet \
		--remove ${LCOV_REPORT} "/usr*" \
		--remove ${LCOV_REPORT} "${HOME}/.phpenv/*" \
		--compat-libtool \
		--output-file ${LCOV_REPORT}

	# FIXME: Fix the report
	# 	Cannot open source file ${PROJECT_ROOT}/kernel/fcall.h
	sed -i.bak s_${PROJECT_ROOT}/kernel_${PROJECT_ROOT}/ext/kernel_g ${LCOV_REPORT}
fi

# Note: to upload a coverage report, set the CODECOV_TOKEN environment variable
#    export CODECOV_TOKEN=<codecov token>

if [[ -z ${CODECOV_TOKEN+x} ]]; then
	echo -e "\nThe CODECOV_TOKEN variable is absent or empty.\nSkip uploading reports to Codecov.\n"
	exit 0
fi

curl -sSl https://codecov.io/bash -o codecov.sh
chmod +x codecov.sh

if [[ -f "${LCOV_REPORT}" ]]; then
	echo -e "Uploading coverage report: ${LCOV_REPORT}...\n"
	./codecov.sh -f "${LCOV_REPORT}"
fi

if [[ -f "${PHPUNIT_REPORT}" ]]; then
	echo -e "Uploading coverage report: ${PHPUNIT_REPORT}...\n"
	./codecov.sh -f "${PHPUNIT_REPORT}"
fi
