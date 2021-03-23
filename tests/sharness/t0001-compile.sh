#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test compile commands"

# shellcheck disable=SC1091
source ./setup.sh

regexp="'-O2 -fvisibility=hidden -Wparentheses\( -flto\)\? -DZEPHIR_RELEASE=1'"
test_expect_success "Compile the extension in production mode" "
  cd $FIXTURESDIR/devmode &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc generate --no-dev 2>&1 >/dev/null &&
  zephirc compile --no-dev 2>&1 >/dev/null &&
  cat ext/config.nice | grep -q -e \"^CFLAGS=$regexp\"
"

regexp="'-O0 -g3'"
test_expect_success "Compile the extension in development mode" "
  cd $FIXTURESDIR/devmode &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc generate --dev 2>&1 >/dev/null &&
  zephirc compile --dev 2>&1 >/dev/null &&
  cat ext/config.nice | grep -q -e \"^CFLAGS=$regexp\"
"

test_done
