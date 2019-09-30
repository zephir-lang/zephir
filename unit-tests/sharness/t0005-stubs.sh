#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generate IDE stubs"

source ./setup.sh

# See: https://github.com/phalcon/zephir/issues/1922
test_expect_success "Should fail when there are no required modules" "
  cd $FIXTURESDIR/stubs/issue1922 &&
  zephirc generate --no-ansi 2>&1 >/dev/null &&
  test_cmp expected/Test.zep.php ide/0.0.1/Stubs/Test.zep.php
"

test_done
