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

: ${ZEPHIR_PARSER_VERSION:=development}
PHP_VERSION=$1

echo "Install Zephir Parser"

if [ ! -f "$HOME/php-zephir-parser/LICENSE" ]; then
  rm -rf "$HOME/php-zephir-parser"
  git clone -b "$ZEPHIR_PARSER_VERSION" \
      --depth 1 \
      -q https://github.com/phalcon/php-zephir-parser \
      "$HOME/php-zephir-parser"
fi

cd "$HOME/php-zephir-parser" || exit 1

phpize
./configure --with-php-config=/usr/bin/php-config --enable-zephir_parser
make -j"$(getconf _NPROCESSORS_ONLN)"

sudo make install
echo 'extension="zephir_parser.so"' |\
  sudo tee "/etc/php/$PHP_VERSION/cli/conf.d/zephir_parser.ini"
