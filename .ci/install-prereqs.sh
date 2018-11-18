#!/bin/sh
#
# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

# -e	Exit immediately if a command exits with a non-zero status.
# -u	Treat unset variables as an error when substituting.
set -eu

if [ -z ${CI+x} ] || [ "$CI" != "true" ]; then
	printf "This script is designed to run inside a CI container only.\nAborting.\n"
	exit 1
fi

printf "\n" | pecl install --force psr

php_vernum="$(php-config --vernum)"

install_ext_from_src () {
	pkgname=$1
	source=$2
	cfgflags=$3
	downloaddir="${HOME}/.cache/${pkgname}/${pkgname}-${php_vernum}"
	prefix="${HOME}/.local/opt/${pkgname}/${pkgname}-${php_vernum}"
	libdir="${prefix}/lib"

	if [ ! -f "${libdir}/${pkgname}.so" ]; then
		if [ ! -d `dirname ${downloaddir}` ]; then
			mkdir -p `dirname ${downloaddir}`
		fi

		cd `dirname ${downloaddir}`

		if [ ! -d "${pkgname}-${php_vernum}" ]; then
			git clone --depth=1 -v -b master "${source}" "${pkgname}-${php_vernum}"
		fi

		if [ ! -f "${pkgname}-${php_vernum}/config.m4" ]; then
			printf "Unable to locate ${pkgname}-${php_vernum}/config.m4 file.\nAborting.\n"
			exit 1
		fi

		if [ ! -d "${downloaddir}" ]; then
			printf "Unable to locate ${pkgname} source.\nAborting.\n"
			exit 1
		fi

		if [ ! -d "${libdir}" ]; then
			mkdir -p "${libdir}"
		fi

		cd "${downloaddir}"
		phpize
		printf "\n" | ./configure "${cfgflags}"

		make --silent -j"$(getconf _NPROCESSORS_ONLN)"
		make --silent install

		if [ -f `$(phpenv which php-config) --extension-dir`/${pkgname}.so ]; then
			cp `$(phpenv which php-config) --extension-dir`/${pkgname}.so "${libdir}/${pkgname}.so"
		elif [ -f "${downloaddir}/modules/${pkgname}.so" ]; then
			cp "${downloaddir}/modules/${pkgname}.so" "${libdir}/${pkgname}.so"
		fi
	fi

	if [ ! -x "${libdir}/${pkgname}.so" ]; then
		printf "Unable to locate ${libdir}/${pkgname}.so.\nAborting.\n"
		exit 1
	fi

	echo "extension=${libdir}/${pkgname}.so" > "$(phpenv root)/versions/$(phpenv version-name)/etc/conf.d/${pkgname}.ini"
}

install_ext_from_src "zephir_parser" "https://github.com/phalcon/php-zephir-parser" ""

if [ "${php_vernum}" -gt 70000 ]; then
	wget \
		"https://github.com/humbug/box/releases/download/${BOX_VERSION}/box.phar" \
		--quiet \
		-O "${HOME}/bin/box"

	chmod +x "${HOME}/bin/box"
fi
