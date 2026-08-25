
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
#include "kernel/operators.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_ArrayAccessObj)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayAccessObj, stub, arrayaccessobj, stub_arrayaccessobj_method_entry, 0);

	zend_declare_property_null(stub_arrayaccessobj_ce, SL("test"), ZEND_ACC_PROTECTED);
	zend_class_implements(stub_arrayaccessobj_ce, 1, zend_ce_arrayaccess);
	return SUCCESS;
}

PHP_METHOD(Stub_ArrayAccessObj, __construct)
{
	zval obj, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	static zend_string *_zephir_prop_2 = NULL;
	static zend_string *_zephir_prop_3 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("key1", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("key2", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_2)) {
		_zephir_prop_2 = zend_string_init("key3", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_3)) {
		_zephir_prop_3 = zend_string_init("test", 4, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "val1");
	zephir_update_property_zval_cached(&obj, _zephir_prop_0, 0, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "val2");
	zephir_update_property_zval_cached(&obj, _zephir_prop_1, 0, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "val3");
	zephir_update_property_zval_cached(&obj, _zephir_prop_2, 0, &_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_3, 10, &obj);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_ArrayAccessObj, offsetSet)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval offset_sub, value_sub, obj;
	zval *offset, *value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&obj);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(offset)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &offset, &value);
	zephir_memory_observe(&obj);
	zephir_read_property_cached(&obj, this_ptr, _zephir_prop_0, 10, PH_NOISY_CC);
	if (!(Z_TYPE_P(offset) == IS_NULL)) {
		zephir_update_property_zval_zval(&obj, offset, value);
	}
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 10, &obj);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_ArrayAccessObj, offsetExists)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval offset_sub, obj;
	zval *offset;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&obj);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &offset);
	zephir_memory_observe(&obj);
	zephir_read_property_cached(&obj, this_ptr, _zephir_prop_0, 10, PH_NOISY_CC);
	RETURN_MM_BOOL(zephir_isset_property_value_zval(&obj, offset));
}

PHP_METHOD(Stub_ArrayAccessObj, offsetUnset)
{
	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval offset_sub, obj, _0, _2;
	zval *offset;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &offset);
	zephir_memory_observe(&obj);
	zephir_read_property_cached(&obj, this_ptr, _zephir_prop_0, 10, PH_NOISY_CC);
	ZEPHIR_CPY_WRT(&_0, &obj);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&obj, &_1);
	zephir_array_unset(&obj, offset, PH_SEPARATE);
	ZEPHIR_CPY_WRT(&_2, &obj);
	zephir_convert_to_object(&_2);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 10, &_2);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_ArrayAccessObj, offsetGet)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval offset_sub, _0, _1, _2;
	zval *offset;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &offset);
	ZEPHIR_INIT_VAR(&_0);
	zephir_read_property_cached(&_1, this_ptr, _zephir_prop_0, 10, PH_NOISY_CC | PH_READONLY);
	if (zephir_isset_property_value_zval(&_1, offset)) {
		zephir_memory_observe(&_2);
		zephir_read_property_cached(&_2, this_ptr, _zephir_prop_0, 10, PH_NOISY_CC);
		ZEPHIR_OBS_NVAR(&_0);
		zephir_read_property_zval(&_0, &_2, offset, PH_NOISY_CC);
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_NULL(&_0);
	}
	RETURN_CCTOR(&_0);
}

