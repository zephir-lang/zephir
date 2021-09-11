#!/bin/sh
#
# This file is part of the Zephir.
#
# (c) Phalcon Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view
# the LICENSE file that was distributed with this source code.

# -e  Exit immediately if a command exits with a non-zero status.
# -u  Treat unset variables as an error when substituting.
set -eu

php \
  -d extension=ext/modules/stub.so \
  vendor/bin/phpunit \
  --bootstrap tests/ext-bootstrap.php \
  --testsuite Extension

php \
  vendor/bin/phpunit \
  --colors=always \
  --testsuite Zephir
