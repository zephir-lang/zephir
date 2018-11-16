#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

say() {
  echo -e "$1"
}

if [ -z ${CODECOV_TOKEN+x} ]; then
	say "The CODECOV_TOKEN variable is absent or empty.\nSkip uploading reports to Codecov"
	exit 0;
fi

if [ ! -z ${TRAVIS_PHP_VERSION+x} ] && [ "${TRAVIS_PHP_VERSION}" = "5.6" ]; then
	say "Uploading reports to Codecov is disabled for PHP 5.6"
	say "Skip"
	exit 0;
fi

curl -sSl https://codecov.io/bash -o codecov.sh
chmod +x codecov.sh

./codecov.sh -h
./codecov.sh
