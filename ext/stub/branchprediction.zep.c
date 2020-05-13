
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Stub_BranchPrediction) {

	ZEPHIR_REGISTER_CLASS(Stub, BranchPrediction, stub, branchprediction, stub_branchprediction_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_BranchPrediction, testLikely1) {

	zval *this_ptr = getThis();


	if (EXPECTED(1 == 1)) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

PHP_METHOD(Stub_BranchPrediction, testLikely2) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	if (EXPECTED(ZEPHIR_IS_LONG_IDENTICAL(a, 1))) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

PHP_METHOD(Stub_BranchPrediction, testUnlikely1) {

	zval *this_ptr = getThis();


	if (EXPECTED(1 == 1)) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

PHP_METHOD(Stub_BranchPrediction, testUnlikely2) {

	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &a);



	if (EXPECTED(ZEPHIR_IS_LONG_IDENTICAL(a, 1))) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

