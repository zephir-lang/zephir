#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test init command for success"

# shellcheck disable=SC1091
source ./setup.sh

test_expect_success "Should not dump config file" "
  cd $OUTPUTDIR &&
  zephirc --version -Wunused-variable 2>&1 >/dev/null &&
  test ! -f ./config.json
"

test_expect_success "Should generate a project and dump config file" "
  cd $OUTPUTDIR &&
  zephirc init success &&
  test -d ./success &&
  test -d ./success/success &&
  test -d ./success/ext &&
  test -f ./success/config.json
"

test_done
