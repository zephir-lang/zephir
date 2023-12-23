
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
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


/**
 * Instance operations
 */
ZEPHIR_INIT_CLASS(Stub_Instance)
{
	ZEPHIR_REGISTER_CLASS(Stub, Instance, stub, instance, stub_instance_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Instance, __construct)
{
	zval *a1, a1_sub, *a2, a2_sub, *a3, a3_sub, *a4, a4_sub, *a5, a5_sub, *a6, a6_sub, *a7, a7_sub, *a8, a8_sub, *a9, a9_sub, *a10, a10_sub, *a11, a11_sub;

	ZVAL_UNDEF(&a1_sub);
	ZVAL_UNDEF(&a2_sub);
	ZVAL_UNDEF(&a3_sub);
	ZVAL_UNDEF(&a4_sub);
	ZVAL_UNDEF(&a5_sub);
	ZVAL_UNDEF(&a6_sub);
	ZVAL_UNDEF(&a7_sub);
	ZVAL_UNDEF(&a8_sub);
	ZVAL_UNDEF(&a9_sub);
	ZVAL_UNDEF(&a10_sub);
	ZVAL_UNDEF(&a11_sub);
	ZEND_PARSE_PARAMETERS_START(11, 11)
		Z_PARAM_OBJECT_OF_CLASS(a1, stub_arithmetic_ce)
		Z_PARAM_OBJECT_OF_CLASS(a2, stub_arrayobject_ce)
		Z_PARAM_OBJECT_OF_CLASS(a3, stub_assign_ce)
		Z_PARAM_OBJECT_OF_CLASS(a4, stub_bitwise_ce)
		Z_PARAM_OBJECT_OF_CLASS(a5, stub_branchprediction_ce)
		Z_PARAM_OBJECT_OF_CLASS(a6, stub_cast_ce)
		Z_PARAM_OBJECT_OF_CLASS(a7, stub_cblock_ce)
		Z_PARAM_OBJECT_OF_CLASS(a8, stub_chars_ce)
		Z_PARAM_OBJECT_OF_CLASS(a9, stub_closures_ce)
		Z_PARAM_OBJECT_OF_CLASS(a10, stub_compare_ce)
		Z_PARAM_OBJECT_OF_CLASS(a11, stub_concat_ce)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(11, 0, &a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, &a10, &a11);
}

PHP_METHOD(Stub_Instance, testIssue1339)
{
	zval parameters, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&parameters);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&parameters);
	zephir_create_array(&parameters, 11, 0);
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_arithmetic_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_arrayobject_ce);
	ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 48);
	zephir_check_call_status();
	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_assign_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_bitwise_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_branchprediction_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_cast_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_cblock_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_chars_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_closures_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_compare_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	object_init_ex(&_0, stub_concat_ce);
	if (zephir_has_constructor(&_0)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	zephir_array_fast_append(&parameters, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "Stub\\Instance");
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance_params(return_value, &_0, &parameters);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Instance, testInstanceCreate)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *className_param = NULL;
	zval className;

	ZVAL_UNDEF(&className);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(className)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &className_param);
	zephir_get_strval(&className, className_param);
	ZEPHIR_LAST_CALL_STATUS = zephir_create_instance(return_value, &className);
	zephir_check_call_status();
	RETURN_MM();
}

