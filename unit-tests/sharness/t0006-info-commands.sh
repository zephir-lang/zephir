#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test informative commands"

# shellcheck disable=SC1091
source ./setup.sh

find_local_cache() {
  if [ -d .zephir ]
  then
    return 0
  else
    return 1
  fi
}

test_expect_success "Shouldn't create .zephir direcory when print compiler version" "
  cd /tmp &&
  zephirc -V &&
  zephirc --version &&
  test_expect_code 1 find_local_cache
"

test_expect_success "Shouldn't create .zephir direcory when print compiler short version" "
  cd /tmp &&
  zephirc --dumpversion &&
  test_expect_code 1 find_local_cache
"

test_expect_success "Shouldn't create .zephir direcory when print compiler help" "
  cd /tmp &&
  zephirc &&
  zephirc help &&
  zephirc -h &&
  zephirc --help &&
  test_expect_code 1 find_local_cache
"

test_done
