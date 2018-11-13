#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

if [ "${CI}" != "true" ]; then
	echo "This script is designed to run inside a CI container only. Stop."
	exit 1
fi

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../")

if [ ! -f "$HOME/.cache/php-psr/php_psr.c" ]; then
	rm -rf "$HOME/.cache/php-psr"
	mkdir -p "$HOME/.cache/php-psr"

	git clone https://github.com/jbboehr/php-psr.git "$HOME/.cache/php-psr"
fi

cd "$HOME/.cache/php-psr"

phpize
./configure --with-php-config=$(phpenv which php-config) --enable-psr
make -j"$(getconf _NPROCESSORS_ONLN)"

cp "$HOME/.cache/php-psr/modules/psr.so" `$(phpenv which php-config) --extension-dir`

echo extension=psr.so > "$(phpenv root)/versions/$(phpenv version-name)/etc/conf.d/psr.ini"
php --ri psr
