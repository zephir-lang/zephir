
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 *
 */
ZEPHIR_INIT_CLASS(Stub_Oo_DynamicProp)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, DynamicProp, stub, oo_dynamicprop, stub_oo_dynamicprop_method_entry, 0);

	zend_declare_property_null(stub_oo_dynamicprop_ce, SL("test"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyInt)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, _0;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 10);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyBoolTrue)
{
	zval property_zv, __$true;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_BOOL(&__$true, 1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&property_zv, property);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &__$true);
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyBoolFalse)
{
	zval property_zv, __$false;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_BOOL(&__$false, 0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&property_zv, property);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &__$false);
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, _0;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "string");
	zephir_update_property_zval_zval(this_ptr, &property_zv, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyChar)
{
	char a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, _0;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	a = 'A';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyUChar)
{
	unsigned char a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, _0;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	a = 'A';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyNull)
{
	zval property_zv, __$null;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_NULL(&__$null);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&property_zv, property);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &__$null);
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyVariableInt)
{
	zend_long a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, _0;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	a = 10;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyVariableString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, a;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "test");
	zephir_update_property_zval_zval(this_ptr, &property_zv, &a);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyVariableBoolTrue)
{
	zend_bool a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, _0;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	a = 1;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setPropertyVariableBoolFalse)
{
	zend_bool a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval property_zv, _0;
	zend_string *property = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(property)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&property_zv);
	ZVAL_STR_COPY(&property_zv, property);
	a = 0;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property_zv, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setExistingStringProperty)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "works");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 67, &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Oo_DynamicProp, setExistingStringPropertyString)
{
	zval value_zv;
	zend_string *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_zv);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("eproperty", 9, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&value_zv, value);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 68, &value_zv);
}

PHP_METHOD(Stub_Oo_DynamicProp, setNonExistingStringProperty)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("property", 8, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "works");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 69, &_0);
	ZEPHIR_MM_RESTORE();
}

