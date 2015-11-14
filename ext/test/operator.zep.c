
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
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Operator) {

	ZEPHIR_REGISTER_CLASS(Test, Operator, test, operator, test_operator_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Operator, testIdentical) {

	zval *param1, *param2;

	zephir_fetch_params(0, 2, 0, &param1, &param2);



	RETURN_BOOL(ZEPHIR_IS_IDENTICAL(param1, param2));

}

PHP_METHOD(Test_Operator, testIdenticalIfComplex) {

	zend_bool b = 0;
	zval *input, *a = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &input);



	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 1, 0 TSRMLS_CC);
	zephir_array_update_long(&a, 1, &input, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	b = 0;
	zephir_array_fetch_long(&_0, a, 1, PH_NOISY | PH_READONLY, "test/operator.zep", 16 TSRMLS_CC);
	if (!ZEPHIR_IS_FALSE_IDENTICAL(_0)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Operator, testIdenticalVarFalse) {

	zval *param;

	zephir_fetch_params(0, 1, 0, &param);



	RETURN_BOOL(ZEPHIR_IS_FALSE_IDENTICAL(param));

}

PHP_METHOD(Test_Operator, testIdenticalFalseVar) {

	zval *param;

	zephir_fetch_params(0, 1, 0, &param);



	RETURN_BOOL(ZEPHIR_IS_FALSE_IDENTICAL(param));

}

