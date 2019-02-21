#!/bin/sh

test_description="Test compile commands"

. ./setup.sh

cflags="'-O2 -fvisibility=hidden -Wparentheses\( -flto\)\? -DZEPHIR_RELEASE=1'"
test_expect_success "Compile the extension in production mode" '
	cd $FIXTURESDIR/devmode &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir compile --no-dev 2>&1 >/dev/null &&
	cat ext/config.nice | grep -e "^CFLAGS=$cflags"
'

cflags="'-O0 -g3'"
test_expect_success "Compile the extension in development mode" '
	cd $FIXTURESDIR/devmode &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir compile --dev 2>&1 >/dev/null &&
	cat ext/config.nice | grep -e "^CFLAGS=$cflags"
'

test_done
