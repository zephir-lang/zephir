#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test compile commands"

source ./setup.sh

regexp="'-O2 -fvisibility=hidden -Wparentheses\( -flto\)\? -DZEPHIR_RELEASE=1'"
test_expect_success "Compile the extension in production mode" "
cd $FIXTURESDIR/devmode &&
  zephir fullclean 2>&1 >/dev/null &&
  zephir compile --no-dev 2>&1 >/dev/null &&
  grep -q -e \"^CFLAGS=$regexp\" ext/config.nice
"

regexp="'-O0 -g3'"
test_expect_success "Compile the extension in development mode" "
  cd $FIXTURESDIR/devmode &&
  zephir fullclean 2>&1 >/dev/null &&
  zephir compile --dev 2>&1 >/dev/null &&
  grep -q -e \"^CFLAGS=$regexp\" ext/config.nice
"

test_done
