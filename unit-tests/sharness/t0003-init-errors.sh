#!/bin/sh

test_description="Test init command for failures"

. ./setup.sh

test_expect_success "Should fail when not enough arguments" '
    cd $OUTPUTDIR &&
    echo "Not enough arguments (missing: \"namespace\")." >expected &&
    test_expect_code 1 ${PHP} ${ZEPHIR_BIN} init 2>actual &&
    test_cmp expected actual
'

for f in expected actual; do
    test -f $OUTPUTDIR/$f && rm -f $OUTPUTDIR/$f
done

test_done
