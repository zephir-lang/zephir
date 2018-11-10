#!/usr/bin/env bash
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

echo -e "Install a tool for writing fast and flexible scanners in C from regular expressions"

if [ "${CI}" != "true" ]; then
	echo "This script is designed to run inside a CI container only. Stop."
	exit 1
fi

PHP_MAJOR="$(`phpenv which php` -r 'echo phpversion();' | cut -d '.' -f 1)"
PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../../")

if [ ! -f "$HOME/.cache/php-psr/php_psr.c" ]; then
	rm -rf "$HOME/.cache/php-psr"
	mkdir "$HOME/.cache/php-psr"

	git clone https://github.com/jbboehr/php-psr.git "$HOME/.cache/php-psr"
fi

cd "$HOME/.cache/php-psr"

phpize
./configure --with-php-config=$(phpenv which php-config) --enable-psr
make -j"$(getconf _NPROCESSORS_ONLN)"

cp "$HOME/.cache/php-psr/modules/psr.so" `$(phpenv which php-config) --extension-dir`

echo extension=psr.so > "$(phpenv root)/versions/$(phpenv version-name)/etc/conf.d/psr.ini"
php --ri psr
