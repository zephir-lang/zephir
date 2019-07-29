# This file is part of the Zephir.
#
# (c) Zephir Team <team@zephir-lang.com>
#
# For the full copyright and license information, please view the LICENSE
# file that was distributed with this source code.

# NOTE: run with TEST_VERBOSE=1 for verbose sharness tests.

T = $(sort $(wildcard t[0-9][0-9][0-9][0-9]-*.sh))
LIBDIR = lib
SHARNESSDIR = sharness
SHARNESS = $(LIBDIR)/$(SHARNESSDIR)/sharness.sh
AGGREGATE = $(LIBDIR)/$(SHARNESSDIR)/aggregate-results.sh

PROJECTDIR=$(LIBDIR)/../../..
TESTSDIR=$(PROJECTDIR)/unit-tests

BINS = $(PROJECTDIR)/zephir

.PHONY: all
all: aggregate

.PHONY: clean
clean: clean-test-results
	find $(TESTSDIR)/fixtures -name compile-errors.log -o -name compile.log | xargs rm -f
	find $(TESTSDIR)/fixtures -name .zephir -type d | xargs rm -rf
	find $(TESTSDIR)/fixtures -name ext -type d | xargs rm -rf

.PHONY: clean-test-results
clean-test-results:
	@echo "*** $@ ***"
	-rm -rf test-results

.PHONY: $(T)
$(T): clean-test-results deps
	@echo "*** $@ ***"
	./$@

.PHONY: aggregate
aggregate: clean-test-results $(T)
	@echo "*** $@ ***"
	ls test-results/t*-*.sh.*.counts | $(AGGREGATE)

.PHONY: deps
deps: sharness $(BINS)

.PHONY: sharness
$(SHARNESS): FORCE
	@echo "*** checking $@ ***"
	$(LIBDIR)/install-sharness.sh
