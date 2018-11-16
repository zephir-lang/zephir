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
C_REPORT=${PROJECT_ROOT}/unit-tests/output/lcov.info

if [[ $(command -v lcov 2>/dev/null) = "" ]]; then
	echo -e "lcov does not exist.\nSkip capturing coverage data."
else
	# Capture coverage data
	lcov \
		--no-checksum \
		--directory ${PROJECT_ROOT} \
		--capture \
		--compat-libtool \
		--output-file ${C_REPORT}

	# Remove files matching non-project patterns
	lcov \
		--remove coverage.info "/usr*" \
		--remove coverage.info "${HOME}/.phpenv/*" \
		--compat-libtool \
		--output-file ${C_REPORT}
fi

# Note: to upload a coverage report, set the CODECOV_TOKEN environment variable
#    export CODECOV_TOKEN=<codecov token>

if [[ -z ${CODECOV_TOKEN+x} ]]; then
	echo -e "\nThe CODECOV_TOKEN variable is absent or empty.\nSkip uploading reports to Codecov.\n"
	exit 0;
fi

if [[ "${CI}" = "true" ]] && [[ "$TRAVIS" = "true" ]]; then
	if [[ "$TRAVIS_PHP_VERSION" = "5.6" ]]; then
		echo -e "\nUploading reports is disabled for PHP 5.6.\nSkip uploading reports to Codecov.\n"
		exit 0;
	fi
fi

curl -sSl https://codecov.io/bash -o codecov.sh
chmod +x codecov.sh

if [[ -f "${C_REPORT}" ]]; then
	./codecov.sh -f "${C_REPORT}"
fi
