#!/usr/bin/env bash

if [ $(valgrind -v glibtoolize 2>/dev/null) = "" ]; then
	echo "Skip check for memory leaks. Valgring does not exist"
	exit 1
fi

# Correctly show the stack frames for extensions compiled as shared libraries
export ZEND_DONT_UNLOAD_MODULES=1

# Disable Zend memory manager before running PHP with valgrind
export USE_ZEND_ALLOC=0

# Do not stop testing on failures
export PHPUNIT_DONT_EXIT=1

PROJECT_ROOT=$(readlink -enq "$(dirname $0)/../../")

valgrind
	--read-var-info=yes \
	--error-exitcode=1 \
	--fullpath-after= \
	--track-origins=yes \
	--leak-check=full \
	--num-callers=20 \
	--run-libc-freeres=no \
	$(phpenv which php) -d "extension=${PROJECT_ROOT}/ext/modules/test.so" \
		"${PROJECT_ROOT}/unit-tests/phpunit" \
			-c "${PROJECT_ROOT}/phpunit.xml.dist" \
			--no-coverage \
			--testsuite "Extension Test Suite"
