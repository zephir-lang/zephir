
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


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&obj);
	object_init(&obj);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "val1");
	zephir_update_property_zval(&obj, ZEND_STRL("key1"), &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "val2");
	zephir_update_property_zval(&obj, ZEND_STRL("key2"), &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "val3");
	zephir_update_property_zval(&obj, ZEND_STRL("key3"), &_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("test"), &obj);
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
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(offset)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &offset, &value);


	ZEPHIR_OBS_VAR(&obj);
	zephir_read_property(&obj, this_ptr, ZEND_STRL("test"), PH_NOISY_CC);
	if (!(Z_TYPE_P(offset) == IS_NULL)) {
		zephir_update_property_zval_zval(&obj, offset, value);
	}
	zephir_update_property_zval(this_ptr, ZEND_STRL("test"), &obj);
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
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);


	ZEPHIR_OBS_VAR(&obj);
	zephir_read_property(&obj, this_ptr, ZEND_STRL("test"), PH_NOISY_CC);
	RETURN_MM_BOOL(zephir_isset_property_zval(&obj, offset));
}

PHP_METHOD(Stub_ArrayAccessObj, offsetUnset)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval offset_sub, obj;
	zval *offset;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);


	ZEPHIR_OBS_VAR(&obj);
	zephir_read_property(&obj, this_ptr, ZEND_STRL("test"), PH_NOISY_CC);
	zephir_get_arrval(&_0, &obj);
	ZEPHIR_CPY_WRT(&obj, &_0);
	zephir_array_unset(&obj, offset, PH_SEPARATE);
	zephir_convert_to_object(&obj);
	zephir_update_property_zval(this_ptr, ZEND_STRL("test"), &obj);
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
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);


	ZEPHIR_INIT_VAR(&_0);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
	if (zephir_isset_property_zval(&_1, offset)) {
		ZEPHIR_OBS_VAR(&_2);
		zephir_read_property(&_2, this_ptr, ZEND_STRL("test"), PH_NOISY_CC);
		zephir_read_property_zval(&_0, &_2, offset, PH_NOISY_CC);
	} else {
		ZVAL_NULL(&_0);
	}
	RETURN_CCTOR(&_0);
}

