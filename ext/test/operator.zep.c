
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
#include "kernel/object.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Operator) {

	ZEPHIR_REGISTER_CLASS(Test, Operator, test, operator, test_operator_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Operator, testIdentical) {

	zval *param1, param1_sub, *param2, param2_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param1_sub);
	ZVAL_UNDEF(&param2_sub);

	zephir_fetch_params_without_memory_grow(2, 0, &param1, &param2);



	RETURN_BOOL(ZEPHIR_IS_IDENTICAL(param1, param2));

}

PHP_METHOD(Test_Operator, testIdenticalIfComplex) {

	zend_bool b = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *input, input_sub, a, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&input_sub);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &input);



	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0 TSRMLS_CC);
	zephir_array_update_string(&a, SL("1"), input, PH_COPY | PH_SEPARATE);
	b = 0;
	zephir_array_fetch_string(&_0, &a, SL("1"), PH_NOISY | PH_READONLY, "test/operator.zep", 16 TSRMLS_CC);
	if (!ZEPHIR_IS_FALSE_IDENTICAL(&_0)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_Operator, testIdenticalVarFalse) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &param);



	RETURN_BOOL(ZEPHIR_IS_FALSE_IDENTICAL(param));

}

PHP_METHOD(Test_Operator, testIdenticalFalseVar) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &param);



	RETURN_BOOL(ZEPHIR_IS_FALSE_IDENTICAL(param));

}

PHP_METHOD(Test_Operator, testBoolComparison) {

	zend_bool var1, var2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *varTrue, varTrue_sub, *varFalse, varFalse_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&varTrue_sub);
	ZVAL_UNDEF(&varFalse_sub);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &varTrue, &varFalse);



	var1 = 1;
	var2 = 0;
	zephir_create_array(return_value, 8, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, ZEPHIR_IS_BOOL_VALUE(varTrue, var1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, ZEPHIR_IS_BOOL_IDENTICAL(varTrue, var1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, ZEPHIR_IS_BOOL_VALUE(varFalse, var2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, ZEPHIR_IS_BOOL_IDENTICAL(varFalse, var2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, !ZEPHIR_IS_BOOL_VALUE(varTrue, var2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, !ZEPHIR_IS_BOOL_IDENTICAL(varTrue, var2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, !ZEPHIR_IS_BOOL_VALUE(varFalse, var1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, !ZEPHIR_IS_BOOL_IDENTICAL(varFalse, var1));
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();

}

