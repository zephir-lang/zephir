#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

set -e +o pipefail

say() {
  echo -e "$1"
}

if [[ -z ${CODECOV_TOKEN+x} ]]; then
	say "\nThe CODECOV_TOKEN variable is absent or empty.\nSkip uploading reports to Codecov.\n"
	exit 0;
fi

if [[ "${CI}" = "true" ]]; then
	if [[ ! -z ${TRAVIS_PHP_VERSION+x} ]] && [[ "${TRAVIS_PHP_VERSION}" = "5.6" ]]; then
		say "\nUploading reports is disabled for PHP 5.6.\nSkip uploading reports to Codecov.\n"
		exit 0;
	fi
fi

curl -sSl https://codecov.io/bash -o codecov.sh
chmod +x codecov.sh

./codecov.sh -v
