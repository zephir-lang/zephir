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

# Ensure that this is being run inside a CI container
if [ "${CI}" != "true" ];
then
	>&2 echo "This script is designed to run inside a CI container only."
	>&2 echo "Aborting."
	exit 1
fi

PHP_INI="$(phpenv root)/versions/$(phpenv version-name)/etc/php.ini"

: ${ZEPHIR_PARSER_VERSION:=master}

# {{{ Install psr extension
printf "Install psr extension\n"
printf "\n" | pecl install --force psr 1> /dev/null
# }}}

# {{{ Install Zephir Parser
printf "Install Zephir Parser\n"
git clone -b "${ZEPHIR_PARSER_VERSION}" --depth 1 -q https://github.com/phalcon/php-zephir-parser
cd php-zephir-parser
$(phpenv which phpize)
./configure --silent --with-php-config=$(phpenv which php-config) --enable-zephir_parser
make --silent -j"$(getconf _NPROCESSORS_ONLN)"
make --silent install
echo 'extension="zephir_parser.so"' > "$(phpenv root)/versions/$(phpenv version-name)/etc/conf.d/zephir_parser.ini"
# }}}

# {{{ Install Box
printf "Install Box\n"
if [ "${PHP_VERNUM}" -ge "70100" ] && [ "${PHP_VERNUM}" -lt "70400" ]; then
	printf "PHP version number is ${PHP_VERNUM}\nDownloading humbug/box...\n"
	wget \
		"https://github.com/humbug/box/releases/download/${BOX_VERSION}/box.phar" \
		--quiet \
		-O "${HOME}/bin/box"

	chmod +x "${HOME}/bin/box"

	box --version
fi
# }}}

# Local variables:
# tab-width: 4
# c-basic-offset: 4
# End:
# vim600: noet sw=4 ts=4 fdm=marker
# vim<600: noet sw=4 ts=4
