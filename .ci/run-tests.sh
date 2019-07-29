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

if [ -z ${CI+x} ] || [ "$CI" != "true" ]; then
  >&2 printf "This script is designed to run inside a CI container only.\n"
  >&2 printf "Aborting.\n"
  exit 1
fi

if [ "$(php-config --vernum)" -lt "70200" ]; then
  test_suite="Extension_Php70"
else
  test_suite="Extension_Php72"
fi

php \
  -d extension=ext/modules/test.so \
  vendor/bin/simple-phpunit \
  --colors=always \
  --bootstrap unit-tests/ext-bootstrap.php \
  --testsuite ${test_suite}

php \
  vendor/bin/simple-phpunit \
  --colors=always \
  --testsuite Zephir
