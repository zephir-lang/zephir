#!/bin/sh

test_description="Test generate commands"

. ./setup.sh

test_expect_success "Should correctly specify ARGINFO for ZendEngine3" '
	cd $FIXTURESDIR/typehints &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine3 2>&1 >/dev/null &&
	test_cmp ext/typehints.c expected3.c &&
	test_cmp ext/typehints/args.zep.h expected_args3.h &&
	test_cmp ext/typehints/retval.zep.h expected_retval3.h &&
	test_cmp ext/typehints/both.zep.h expected_both3.h
'

test_expect_success "Should correctly specify ARGINFO for ZendEngine2" '
	cd $FIXTURESDIR/typehints &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine2 2>&1 >/dev/null &&
	test_cmp ext/typehints.c expected2.c &&
	test_cmp ext/typehints/args.zep.h expected_args2.h &&
	test_cmp ext/typehints/retval.zep.h expected_retval2.h &&
	test_cmp ext/typehints/both.zep.h expected_both2.h
'

test_expect_success "Life Cycle Test for ZendEngine3" '
	cd $FIXTURESDIR/lifecycle &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine3 2>&1 >/dev/null &&
	test_cmp ext/lifecycle.c expected3.c
'

test_expect_success "Life Cycle Test for ZendEngine2" '
	cd $FIXTURESDIR/lifecycle &&
	zephir fullclean 2>&1 >/dev/null &&
	zephir generate --backend=ZendEngine2 2>&1 >/dev/null &&
	test_cmp ext/lifecycle.c expected2.c
'

test_done
