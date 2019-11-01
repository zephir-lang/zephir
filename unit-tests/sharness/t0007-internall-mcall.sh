#!/usr/bin/env bash

# shellcheck disable=SC2034
test_description="Test internal methods call"

# shellcheck disable=SC1091
source ./setup.sh

code="echo get_class((new InternalTest\\Test)->issue1956());"
test_expect_success "Return 'this' inside a method when 'internal-call-transformation' is enabled" "
  cd $FIXTURESDIR/internal &&
  zephirc fullclean 2>&1 >/dev/null &&
  zephirc compile 2>&1 >/dev/null &&
  php -d extension=ext/modules/internaltest.so -r '$code' > ext/issue-1956.txt &&
  test_cmp expected/issue-1956.txt ext/issue-1956.txt
"

test_done
