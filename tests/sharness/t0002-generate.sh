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

test_done
