#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test generate command for failures"

# shellcheck disable=SC1091
source ./setup.sh

test_expect_success "Should fail when Zephir files to compile couldn't be found" "
  cd $OUTPUTDIR &&
  zephirc init genfailure1 &&
  cd genfailure1 &&
  test_expect_code 1 zephirc generate --no-ansi 2>$OUTPUTDIR/actual &&
  test_cmp $FIXTURESDIR/genfailure1 $OUTPUTDIR/actual
"

test_expect_success "Should fail when there are no required modules" "
  cd $FIXTURESDIR/deps &&
  test_expect_code 1 zephirc generate --no-ansi 2>$OUTPUTDIR/actual &&
  test_cmp $FIXTURESDIR/genfailure2 $OUTPUTDIR/actual
"

test_done
