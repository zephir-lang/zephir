
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
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Test_BranchPrediction) {

	ZEPHIR_REGISTER_CLASS(Test, BranchPrediction, test, branchprediction, test_branchprediction_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_BranchPrediction, testLikely1) {


	if (likely(1 == 1)) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

PHP_METHOD(Test_BranchPrediction, testLikely2) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	if (likely(ZEPHIR_IS_LONG_IDENTICAL(a, 1))) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

PHP_METHOD(Test_BranchPrediction, testUnlikely1) {


	if (likely(1 == 1)) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

PHP_METHOD(Test_BranchPrediction, testUnlikely2) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	if (likely(ZEPHIR_IS_LONG_IDENTICAL(a, 1))) {
		RETURN_BOOL(1);
	} else {
		RETURN_BOOL(0);
	}

}

