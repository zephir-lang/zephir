#!/usr/bin/env bash
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

# Ensure that this is being run inside a CI container
if [ "${CI}" != "true" ]
then
  (>&2 echo "This script is designed to run inside a CI container only.")
  (>&2 echo "Aborting.")
  exit 1
fi

: "${ZEPHIR_PARSER_VERSION:=master}"

# Install psr extension
(>&1 echo "Install psr extension...")
printf "\\n" | pecl install --force psr 1> /dev/null

# Install Zephir Parser
(>&1 echo "Install Zephir Parser...")
git clone -b "$ZEPHIR_PARSER_VERSION" --depth 1 -q https://github.com/phalcon/php-zephir-parser
cd php-zephir-parser
# shellcheck disable=SC2091
$(phpenv which phpize)
./configure --silent --with-php-config="$(phpenv which php-config)" --enable-zephir_parser
make --silent -j"$(getconf _NPROCESSORS_ONLN)"
make --silent install
echo 'extension="zephir_parser.so"' > "$(phpenv root)/versions/$(phpenv version-name)/etc/conf.d/zephir_parser.ini"

# Install Box
if [ "$BUILD_PHAR" -eq "1" ]
then
  (>&1 echo "Install Box...")
  wget \
    "https://github.com/humbug/box/releases/download/${BOX_VERSION}/box.phar" \
    --quiet \
    -O "${HOME}/bin/box"

  chmod +x "${HOME}/bin/box"
  box --version
fi
