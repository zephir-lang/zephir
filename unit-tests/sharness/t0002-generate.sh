#!/bin/sh

test_description="Test generate commands"

. ./setup.sh

test_expect_success "Should correctly specify ARGINFO" '
	cd $FIXTURESDIR/typehints &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine3 2>&1 >/dev/null &&
	test_cmp ext/typehints.c expected3.c &&
	test_cmp ext/typehints/args.zep.h expected_args3.h &&
	test_cmp ext/typehints/retval.zep.h expected_retval3.h &&
	test_cmp ext/typehints/both.zep.h expected_both3.h
'

test_expect_success "Life Cycle Test" '
	cd $FIXTURESDIR/lifecycle &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine3 2>&1 >/dev/null &&
	test_cmp ext/lifecycle.c expected3.c
'

# See: https://github.com/phalcon/zephir/issues/1758
test_expect_success "Should generate valid code with inheritance of prototype interfaces" '
	cd $FIXTURESDIR/protodir &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine3 2>&1 >/dev/null &&
	test_cmp ext/protodir/connectionexception.zep.h connectionexception.h
'

test_done
