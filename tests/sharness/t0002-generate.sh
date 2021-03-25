#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generate commands"

# shellcheck disable=SC1091
source ./setup.sh

test_expect_success "Should correctly specify ARGINFO" "
  cd $FIXTURESDIR/typehints &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc generate --backend=ZendEngine3 2>&1 >/dev/null &&
  test_cmp ext/typehints.c expected3.c &&
  test_cmp ext/typehints/args.zep.h expected_args3.h &&
  test_cmp ext/typehints/retval.zep.h expected_retval3.h &&
  test_cmp ext/typehints/both.zep.h expected_both3.h
"

test_expect_success "Life Cycle Test" "
  cd $FIXTURESDIR/lifecycle &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc generate --backend=ZendEngine3 2>&1 >/dev/null &&
  test_cmp ext/lifecycle.c expected3.c
"

# See: https://github.com/zephir-lang/zephir/issues/1758
test_expect_success "Should generate valid code with inheritance of prototype interfaces" "
  cd $FIXTURESDIR/protodir &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc generate --backend=ZendEngine3 2>&1 >/dev/null &&
  test_cmp ext/protodir/connectionexception.zep.h connectionexception.h
"

test_done
