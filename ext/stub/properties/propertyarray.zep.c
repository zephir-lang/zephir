
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


/**
 * @link https://github.com/zephir-lang/zephir/issues/520
 */
ZEPHIR_INIT_CLASS(Stub_Properties_PropertyArray)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, PropertyArray, stub, properties_propertyarray, stub_properties_propertyarray_method_entry, 0);

	/**
	 * This is a public property with an initial empty-array value
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_properties_propertyarray_ce, SL("someEmptyArray"), &_zc0, ZEND_ACC_PUBLIC);
	}

	/**
	 * This is a public property with an initial array value
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 5);
		add_next_index_long(&_zc0, 1);
		add_next_index_long(&_zc0, 2);
		add_next_index_long(&_zc0, 3);
		add_next_index_long(&_zc0, 4);
		zephir_declare_property_array(stub_properties_propertyarray_ce, SL("someArray"), &_zc0, ZEND_ACC_PUBLIC);
	}

	/**
	 * This is a protected property with an initial empty-array value
	 */
	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_property_array(stub_properties_propertyarray_ce, SL("otherArray"), &_zc0, ZEND_ACC_PROTECTED);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Properties_PropertyArray, __construct)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("someArray", 9, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 117, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Properties_PropertyArray, appendSome)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	zephir_cast_to_string(&_0, value);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_array_append(this_ptr, SL("someArray"), &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Properties_PropertyArray, setOtherArray)
{
	zval *arr, arr_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("otherArray", 10, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(arr)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &arr);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 118, arr);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Properties_PropertyArray, getOtherArray)
{

	RETURN_MEMBER(getThis(), "otherArray");
}

PHP_METHOD(Stub_Properties_PropertyArray, testIssues1831)
{
	zval info, headers, _0$$3, *_1$$3, _3$$3, _4$$3, _6$$5, *_7$$5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL, *_5 = NULL, *_8 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&info);
	ZVAL_UNDEF(&headers);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_6$$5);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("otherArray", 10, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&headers);
	array_init(&headers);
	while (1) {
		if (!(1)) {
			break;
		}
		ZEPHIR_OBS_NVAR(&_0$$3);
		_1$$3 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_0$$3);
		ZEPHIR_MAKE_WRITE_REF(_1$$3);
		ZEPHIR_CALL_FUNCTION(&info, "array_shift", &_2, 3, _1$$3);
		ZEPHIR_UNREF_WRITE(_1$$3);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(&_3$$3);
		ZVAL_STRING(&_3$$3, "header");
		ZEPHIR_CALL_FUNCTION(&_4$$3, "stripos", &_5, 68, &info, &_3$$3);
		zephir_check_call_status();
		if (!ZEPHIR_IS_FALSE_IDENTICAL(&_4$$3)) {
			zephir_array_append(&headers, &info, PH_SEPARATE, "stub/properties/propertyarray.zep", 51);
			if (UNEXPECTED(EG(exception))) {
				ZEPHIR_MM_RESTORE();
				return;
			}
			continue;
		} else {
			ZEPHIR_OBS_NVAR(&_6$$5);
			_7$$5 = zephir_fetch_property_write(this_ptr, _zephir_prop_0, &_6$$5);
			ZEPHIR_MAKE_WRITE_REF(_7$$5);
			ZEPHIR_CALL_FUNCTION(NULL, "array_unshift", &_8, 69, _7$$5, &info);
			ZEPHIR_UNREF_WRITE(_7$$5);
			zephir_check_call_status();
			break;
		}
	}
	RETURN_CCTOR(&headers);
}

