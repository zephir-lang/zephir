
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


ZEPHIR_INIT_CLASS(Stub_Oo_PropertyAccess)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, PropertyAccess, stub, oo_propertyaccess, stub_oo_propertyaccess_method_entry, 0);

	zend_declare_property_null(stub_oo_propertyaccess_ce, SL("a"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_oo_propertyaccess_ce, SL("b"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_oo_propertyaccess_ce, SL("privateVariable"), ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_Oo_PropertyAccess, __construct)
{
	zval _0, _6;
	zval test, test1, _1, _2, _3, _4, _5, _7, _8, _9, _10;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&test);
	ZVAL_UNDEF(&test1);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_6);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	static zend_string *_zephir_prop_2 = NULL;
	static zend_string *_zephir_prop_3 = NULL;
	static zend_string *_zephir_prop_4 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("test", 4, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("test2", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_2)) {
		_zephir_prop_2 = zend_string_init("test3", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_3)) {
		_zephir_prop_3 = zend_string_init("a", 1, 1);
	}
	if (UNEXPECTED(!_zephir_prop_4)) {
		_zephir_prop_4 = zend_string_init("b", 1, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&test);
	object_init(&test);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 5, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 6.00);
	zephir_array_fast_append(&_0, &_1);
	zephir_update_property_zval_cached(&test, _zephir_prop_0, 0, &_0);
	zephir_read_property_cached(&_2, &test, _zephir_prop_0, 0, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_3);
	zephir_array_fetch_long(&_3, &_2, 0, PH_NOISY, "stub/oo/propertyaccess.zep", 17);
	zephir_update_property_zval_cached(&test, _zephir_prop_1, 0, &_3);
	zephir_read_property_cached(&_4, &test, _zephir_prop_0, 0, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_5);
	zephir_array_fetch_long(&_5, &_4, 1, PH_NOISY, "stub/oo/propertyaccess.zep", 18);
	zephir_update_property_zval_cached(&test, _zephir_prop_2, 0, &_5);
	ZEPHIR_INIT_VAR(&_6);
	zephir_create_array(&_6, 4, 0);
	zephir_read_property_cached(&_7, &test, _zephir_prop_0, 0, PH_NOISY_CC | PH_READONLY);
	zephir_memory_observe(&_8);
	zephir_array_fetch_long(&_8, &_7, 1, PH_NOISY, "stub/oo/propertyaccess.zep", 19);
	zephir_array_fast_append(&_6, &_8);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_6, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_6, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_6, &_1);
	zephir_update_property_zval_cached(&test, _zephir_prop_2, 0, &_6);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_3, 77, &test);
	zephir_read_property_cached(&_9, this_ptr, _zephir_prop_3, 77, PH_NOISY_CC | PH_READONLY);
	zephir_read_property_cached(&_10, &_9, _zephir_prop_1, 0, PH_NOISY_CC | PH_READONLY);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_4, 78, &_10);
	zephir_memory_observe(&test1);
	zephir_read_property_cached(&test1, this_ptr, _zephir_prop_3, 77, PH_NOISY_CC);
	ZEPHIR_MM_RESTORE();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1851
 */
PHP_METHOD(Stub_Oo_PropertyAccess, setPrivatevariable)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("privateVariable", 15, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 79, value);
	RETURN_THISW();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1851
 */
PHP_METHOD(Stub_Oo_PropertyAccess, getPrivatevariable)
{

	RETURN_MEMBER(getThis(), "privateVariable");
}

