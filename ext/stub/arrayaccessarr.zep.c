
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
#include "kernel/array.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_ArrayAccessArr)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayAccessArr, stub, arrayaccessarr, stub_arrayaccessarr_method_entry, 0);

	zend_declare_property_null(stub_arrayaccessarr_ce, SL("test"), ZEND_ACC_PROTECTED);
	zend_class_implements(stub_arrayaccessarr_ce, 1, zend_ce_arrayaccess);
	return SUCCESS;
}

PHP_METHOD(Stub_ArrayAccessArr, __construct)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	add_assoc_long_ex(&_0, SL("one"), 1);
	add_assoc_long_ex(&_0, SL("two"), 2);
	add_assoc_long_ex(&_0, SL("three"), 3);
	zephir_update_property_zval(this_ptr, ZEND_STRL("test"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_ArrayAccessArr, offsetSet)
{
	zval offset_sub, value_sub;
	zval *offset, *value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(offset)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(2, 0, &offset, &value);


	if (Z_TYPE_P(offset) == IS_NULL) {
		zephir_update_property_array_append(this_ptr, SL("test"), value);
	} else {
		zephir_update_property_array(this_ptr, SL("test"), offset, value);
	}
}

PHP_METHOD(Stub_ArrayAccessArr, offsetExists)
{
	zval offset_sub, _0;
	zval *offset;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &offset);


	zephir_read_property(&_0, this_ptr, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
	RETURN_BOOL(zephir_array_isset(&_0, offset));
}

PHP_METHOD(Stub_ArrayAccessArr, offsetUnset)
{
	zval offset_sub, _0;
	zval *offset;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &offset);


	zephir_unset_property_array(this_ptr, ZEND_STRL("test"), offset);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
	zephir_array_unset(&_0, offset, PH_SEPARATE);
}

PHP_METHOD(Stub_ArrayAccessArr, offsetGet)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval offset_sub, _0, _1, _2;
	zval *offset;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&offset_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(offset)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &offset);


	ZEPHIR_INIT_VAR(&_0);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
	if (zephir_array_isset(&_1, offset)) {
		zephir_read_property(&_2, this_ptr, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
		zephir_array_fetch(&_0, &_2, offset, PH_NOISY, "stub/arrayaccessarr.zep", 37);
	} else {
		ZVAL_NULL(&_0);
	}
	RETURN_CCTOR(&_0);
}

