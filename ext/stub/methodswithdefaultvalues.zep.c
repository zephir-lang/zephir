
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


ZEPHIR_INIT_CLASS(Stub_MethodsWithDefaultValues)
{
	ZEPHIR_REGISTER_CLASS(Stub, MethodsWithDefaultValues, stub, methodswithdefaultvalues, stub_methodswithdefaultvalues_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testBool)
{
	zval *param_param = NULL;
	zend_bool param;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 1;
	} else {
		}
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableBool)
{
	zval *param_param = NULL;
	zend_bool param;

	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL_OR_NULL(param, is_null_true)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 0;
	} else {
		}
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *param_param = NULL;
	zval param;

	ZVAL_UNDEF(&param);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &param_param);
	if (!param_param) {
		ZEPHIR_INIT_VAR(&param);
		ZVAL_STRING(&param, "va'lue");
	} else {
		zephir_get_strval(&param, param_param);
	}
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *param_param = NULL;
	zval param;

	ZVAL_UNDEF(&param);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &param_param);
	if (!param_param) {
		ZEPHIR_INIT_VAR(&param);
	} else {
		zephir_get_strval(&param, param_param);
	}
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testInt)
{
	zval *param_param = NULL;
	zend_long param;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 1337;
	} else {
		}
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableInt)
{
	zval *param_param = NULL;
	zend_long param;

	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG_OR_NULL(param, is_null_true)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 0;
	} else {
		}
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testDouble)
{
	zval *param_param = NULL;
	double param;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 13.37;
	} else {
		param = zephir_get_doubleval(param_param);
	}
}

PHP_METHOD(Stub_MethodsWithDefaultValues, testNullableDouble)
{
	zval *param_param = NULL;
	double param;

	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 0;
	} else {
		param = zephir_get_doubleval(param_param);
	}
}

