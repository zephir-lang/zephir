
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
#include "kernel/memory.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_Issue2385)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2385, stub, issue2385, stub_issue2385_method_entry, 0);

	zend_declare_property_null(stub_issue2385_ce, SL("declaredOnly"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_issue2385_ce, SL("assignedNull"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_issue2385_ce, SL("assignedValue"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Issue2385, __construct)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, _0;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_update_property_zval(this_ptr, ZEND_STRL("assignedNull"), &__$null);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "set");
	zephir_update_property_zval(this_ptr, ZEND_STRL("assignedValue"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Issue2385, checkDeclaredOnly)
{
	zval *this_ptr = getThis();
	static zend_string *_zephir_isset_0 = NULL;
	if (UNEXPECTED(!_zephir_isset_0)) {
		_zephir_isset_0 = zend_string_init("declaredOnly", 12, 1);
	}
	RETURN_BOOL(zephir_isset_property_value_fast(this_ptr, _zephir_isset_0));
}

PHP_METHOD(Stub_Issue2385, checkAssignedNull)
{
	zval *this_ptr = getThis();
	static zend_string *_zephir_isset_0 = NULL;
	if (UNEXPECTED(!_zephir_isset_0)) {
		_zephir_isset_0 = zend_string_init("assignedNull", 12, 1);
	}
	RETURN_BOOL(zephir_isset_property_value_fast(this_ptr, _zephir_isset_0));
}

PHP_METHOD(Stub_Issue2385, checkAssignedValue)
{
	zval *this_ptr = getThis();
	static zend_string *_zephir_isset_0 = NULL;
	if (UNEXPECTED(!_zephir_isset_0)) {
		_zephir_isset_0 = zend_string_init("assignedValue", 13, 1);
	}
	RETURN_BOOL(zephir_isset_property_value_fast(this_ptr, _zephir_isset_0));
}

PHP_METHOD(Stub_Issue2385, checkDynamicKeyNull)
{
	zval key_zv;
	zend_string *key = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&key_zv, key);
	RETURN_BOOL(zephir_isset_property_value_zval(this_ptr, &key_zv));
}

PHP_METHOD(Stub_Issue2385, checkArrayKeyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, arr;

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&arr);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 1, 0);
	zephir_array_update_string(&arr, SL("a"), &__$null, PH_COPY | PH_SEPARATE);
	RETURN_MM_BOOL(zephir_array_isset_value_string(&arr, SL("a")));
}

PHP_METHOD(Stub_Issue2385, checkArrayKeyValue)
{
	zval arr;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 1, 0);
	add_assoc_stringl_ex(&arr, SL("a"), SL("v"));
	RETURN_MM_BOOL(zephir_array_isset_value_string(&arr, SL("a")));
}

PHP_METHOD(Stub_Issue2385, checkArrayKeyMissing)
{
	zval arr;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 1, 0);
	add_assoc_stringl_ex(&arr, SL("b"), SL("v"));
	RETURN_MM_BOOL(zephir_array_isset_value_string(&arr, SL("a")));
}

PHP_METHOD(Stub_Issue2385, checkArrayLongKeyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, arr;

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&arr);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 1, 0);
	zephir_array_update_long(&arr, 0, &__$null, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_MM_BOOL(zephir_array_isset_value_long(&arr, 0));
}

PHP_METHOD(Stub_Issue2385, checkArrayLongKeyValue)
{
	zval arr;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 1, 0);
	add_index_stringl(&arr, 0, SL("v"));
	RETURN_MM_BOOL(zephir_array_isset_value_long(&arr, 0));
}

PHP_METHOD(Stub_Issue2385, checkArrayVarKeyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key, key_sub, __$null, arr;

	ZVAL_UNDEF(&key_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&arr);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key);
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 1, 0);
	zephir_array_update_string(&arr, SL("a"), &__$null, PH_COPY | PH_SEPARATE);
	RETURN_MM_BOOL(zephir_array_isset_value(&arr, key));
}

PHP_METHOD(Stub_Issue2385, checkArrayVarKeyValue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key, key_sub, arr;

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&arr);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key);
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 1, 0);
	add_assoc_stringl_ex(&arr, SL("a"), SL("v"));
	RETURN_MM_BOOL(zephir_array_isset_value(&arr, key));
}

