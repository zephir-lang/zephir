
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


ZEPHIR_INIT_CLASS(Stub_Oo_PropertyAccess) {

	ZEPHIR_REGISTER_CLASS(Stub\\Oo, PropertyAccess, stub, oo_propertyaccess, stub_oo_propertyaccess_method_entry, 0);

	zend_declare_property_null(stub_oo_propertyaccess_ce, SL("a"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_oo_propertyaccess_ce, SL("b"), ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_oo_propertyaccess_ce, SL("privateVariable"), ZEND_ACC_PRIVATE);

	return SUCCESS;

}

PHP_METHOD(Stub_Oo_PropertyAccess, __construct) {

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


	ZEPHIR_MM_GROW();

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
	zephir_update_property_zval(&test, ZEND_STRL("test"), &_0);
	zephir_read_property(&_2, &test, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
	zephir_array_fetch_long(&_3, &_2, 0, PH_NOISY | PH_READONLY, "stub/oo/propertyaccess.zep", 17);
	zephir_update_property_zval(&test, ZEND_STRL("test2"), &_3);
	zephir_read_property(&_4, &test, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
	zephir_array_fetch_long(&_5, &_4, 1, PH_NOISY | PH_READONLY, "stub/oo/propertyaccess.zep", 18);
	zephir_update_property_zval(&test, ZEND_STRL("test3"), &_5);
	ZEPHIR_INIT_VAR(&_6);
	zephir_create_array(&_6, 4, 0);
	zephir_read_property(&_7, &test, ZEND_STRL("test"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_OBS_VAR(&_8);
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
	zephir_update_property_zval(&test, ZEND_STRL("test3"), &_6);
	zephir_update_property_zval(this_ptr, ZEND_STRL("a"), &test);
	zephir_read_property(&_9, this_ptr, ZEND_STRL("a"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_10, &_9, ZEND_STRL("test2"), PH_NOISY_CC | PH_READONLY);
	zephir_update_property_zval(this_ptr, ZEND_STRL("b"), &_10);
	zephir_read_property(&test1, this_ptr, ZEND_STRL("a"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_MM_RESTORE();

}

/**
 * @issue https://github.com/phalcon/zephir/issues/1851
 */
PHP_METHOD(Stub_Oo_PropertyAccess, setPrivatevariable) {

	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &value);



	zephir_update_property_zval(this_ptr, ZEND_STRL("privateVariable"), value);
	RETURN_THISW();

}

/**
 * @issue https://github.com/phalcon/zephir/issues/1851
 */
PHP_METHOD(Stub_Oo_PropertyAccess, getPrivatevariable) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "privateVariable");

}

