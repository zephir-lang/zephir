#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test init commands"

source ./setup.sh

test_expect_success "Should fail when not enough arguments" "
  cd $OUTPUTDIR &&
  echo 'Not enough arguments (missing: \"namespace\").' >expected &&
  test_expect_code 1 php $ZEPHIR_BIN init 2>actual &&
  test_cmp expected actual
"

cleanup_output "expected actual"

test_done
