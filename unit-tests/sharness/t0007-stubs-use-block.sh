#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generate IDE stubs with `use` block"

source ./setup.sh

# See: https://github.com/phalcon/zephir/issues/1778
test_expect_success "Should properly generate includes (use block)" "
  cd $FIXTURESDIR/stubs/issue1778 &&
  zephirc generate --no-ansi 2>&1 >/dev/null &&
  zephirc stubs --no-ansi 2>&1 >/dev/null &&
  test_cmp expected/Test.zep.php ide/0.0.1/Stubs/Test.zep.php
"

test_done
