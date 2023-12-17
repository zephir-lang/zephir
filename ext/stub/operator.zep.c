
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
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Operator)
{
	ZEPHIR_REGISTER_CLASS(Stub, Operator, stub, operator, stub_operator_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Operator, testIdentical)
{
	zval *param1, param1_sub, *param2, param2_sub;

	ZVAL_UNDEF(&param1_sub);
	ZVAL_UNDEF(&param2_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(param1)
		Z_PARAM_ZVAL(param2)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &param1, &param2);
	RETURN_BOOL(ZEPHIR_IS_IDENTICAL(param1, param2));
}

PHP_METHOD(Stub_Operator, testIdenticalIfComplex)
{
	zend_bool b = 0;
	zval *input, input_sub, a, _0;

	ZVAL_UNDEF(&input_sub);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(input)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &input);
	ZVAL_NULL(&a);
	zephir_create_array(&a, 1, 0);
	zephir_array_update_string(&a, SL("1"), input, PH_COPY | PH_SEPARATE);
	b = 0;
	zephir_array_fetch_string(&_0, &a, SL("1"), PH_NOISY | PH_READONLY, "stub/operator.zep", 16);
	if (!ZEPHIR_IS_FALSE_IDENTICAL(&_0)) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_Operator, testIdenticalVarFalse)
{
	zval *param, param_sub;

	ZVAL_UNDEF(&param_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &param);
	RETURN_BOOL(ZEPHIR_IS_FALSE_IDENTICAL(param));
}

PHP_METHOD(Stub_Operator, testIdenticalFalseVar)
{
	zval *param, param_sub;

	ZVAL_UNDEF(&param_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &param);
	RETURN_BOOL(ZEPHIR_IS_FALSE_IDENTICAL(param));
}

PHP_METHOD(Stub_Operator, testBoolComparison)
{
	zend_bool var1, var2;
	zval *varTrue, varTrue_sub, *varFalse, varFalse_sub, _0;

	ZVAL_UNDEF(&varTrue_sub);
	ZVAL_UNDEF(&varFalse_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(varTrue)
		Z_PARAM_ZVAL(varFalse)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &varTrue, &varFalse);
	var1 = 1;
	var2 = 0;
	zephir_create_array(return_value, 8, 0);
	ZVAL_NULL(&_0);
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
	return;
}

PHP_METHOD(Stub_Operator, testNewInstanceOperator)
{
	zend_class_entry *_1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className, className_sub, _0;

	ZVAL_UNDEF(&className_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(className)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &className);
	zephir_fetch_safe_class(&_0, className);
	_1 = zephir_fetch_class_str_ex(Z_STRVAL_P(&_0), Z_STRLEN_P(&_0), ZEND_FETCH_CLASS_AUTO);
	if(!_1) {
		RETURN_MM_NULL();
	}
	object_init_ex(return_value, _1);
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	return;
}

