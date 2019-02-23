#!/bin/sh

test_description="Test generate commands"

. ./setup.sh

# Skip all remaining tests if prerequisite is not met.
if test $PHP_VERSION_ID -lt 70000; then
	skip_all='skipping geneartion tests, these tests are designed to use for PHP > 7.0.0'
	test_done
fi

# See: https://github.com/phalcon/zephir/issues/1758
test_expect_success "Should generate valid code with inheritance of prototype interfaces" '
	cd $FIXTURESDIR/protodir &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine3 2>&1 >/dev/null &&
	test_cmp ext/protodir/connectionexception.zep.h connectionexception.h
'

test_done
