#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generate commands"

# shellcheck disable=SC1091
source ./setup.sh

test_expect_success "Life Cycle Test" "
  cd $FIXTURESDIR/lifecycle &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc generate 2>&1 >/dev/null &&
  test_cmp ext/lifecycle.c expected3.c
"

# See: https://github.com/zephir-lang/zephir/issues/1758
test_expect_success "Should generate valid code with inheritance of prototype interfaces" "
  cd $FIXTURESDIR/protodir &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc generate 2>&1 >/dev/null &&
  test_cmp ext/protodir/connectionexception.zep.h connectionexception.h
"

test_done
