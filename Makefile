testsdir := test
TESTS := $(shell find $(testsdir) -type f -name "*.zep")

.PHONY : test

test : build
	@phpunit

build : ext/modules/test.so
ext/modules/test.so : $(TESTS)
	./bin/zephir build
