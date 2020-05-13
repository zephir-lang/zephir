#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the
# LICENSE file that was distributed with this source code.

# -e	Exit immediately if a command exits with a non-zero status.
# -u	Treat unset variables as an error when substituting.
set -eu

: "${ZEPHIR_PARSER_VERSION:=development}"

echo "Install Zephir Parser using version: $ZEPHIR_PARSER_VERSION"

PHP_INI_DIR="$(dirname "$(php -i | grep /.+/conf.d/.+.ini -oE | head -n 1)")"

git clone -b "$ZEPHIR_PARSER_VERSION" \
      --depth 1 \
      -q https://github.com/phalcon/php-zephir-parser \
      php-zephir-parser

cd php-zephir-parser || exit 1

phpize
./configure --with-php-config="$(command -v php-config)" --enable-zephir_parser
make -j"$(getconf _NPROCESSORS_ONLN)"
sudo make install

echo 'extension="zephir_parser.so"' |\
  sudo tee "$PHP_INI_DIR/zephir_parser.ini"

php --ri "Zephir Parser"
