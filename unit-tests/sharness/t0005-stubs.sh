#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generate IDE stubs"

source ./setup.sh

# See: https://github.com/phalcon/zephir/issues/1922
test_expect_success "Should properly generate type hint" "
  cd $FIXTURESDIR/stubs/issues &&
  zephirc generate --no-ansi 2>&1 >/dev/null &&
  zephirc stubs --no-ansi 2>&1 >/dev/null &&
  test_cmp expected/Test1922.zep.php ide/0.0.1/Stubs/Test1922.zep.php
"

# See: https://github.com/phalcon/zephir/issues/1778
test_expect_success "Should properly namespace imports (use block)" "
  cd $FIXTURESDIR/stubs/issues &&
  zephirc generate --no-ansi 2>&1 >/dev/null &&
  zephirc stubs --no-ansi 2>&1 >/dev/null &&
  test_cmp expected/UseBlock.zep.php ide/0.0.1/Stubs/UseBlock.zep.php
"

# See: https://github.com/phalcon/zephir/issues/1907
test_expect_success "Should properly generate Namespace for extends" "
  cd $FIXTURESDIR/stubs/issues &&
  zephirc generate --no-ansi 2>&1 >/dev/null &&
  zephirc stubs --no-ansi 2>&1 >/dev/null &&
  test_cmp expected/Exception.zep.php ide/0.0.1/Stubs/Exception.zep.php
"

test_done
