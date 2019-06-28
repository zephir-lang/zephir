
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


/**
 * Class with constructor + params
 */
ZEPHIR_INIT_CLASS(Test_Oo_PropertyAccess) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, PropertyAccess, test, oo_propertyaccess, test_oo_propertyaccess_method_entry, 0);

	zend_declare_property_null(test_oo_propertyaccess_ce, SL("a"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_oo_propertyaccess_ce, SL("b"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_oo_propertyaccess_ce, SL("privateVariable"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_PropertyAccess, __construct) {

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
	zephir_create_array(&_0, 5, 0 TSRMLS_CC);
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
	zephir_update_property_zval(&test, SL("test"), &_0);
	zephir_read_property(&_2, &test, SL("test"), PH_NOISY_CC | PH_READONLY);
	zephir_array_fetch_long(&_3, &_2, 0, PH_NOISY | PH_READONLY, "test/oo/propertyaccess.zep", 21 TSRMLS_CC);
	zephir_update_property_zval(&test, SL("test2"), &_3);
	zephir_read_property(&_4, &test, SL("test"), PH_NOISY_CC | PH_READONLY);
	zephir_array_fetch_long(&_5, &_4, 1, PH_NOISY | PH_READONLY, "test/oo/propertyaccess.zep", 22 TSRMLS_CC);
	zephir_update_property_zval(&test, SL("test3"), &_5);
	ZEPHIR_INIT_VAR(&_6);
	zephir_create_array(&_6, 4, 0 TSRMLS_CC);
	zephir_read_property(&_7, &test, SL("test"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_OBS_VAR(&_8);
	zephir_array_fetch_long(&_8, &_7, 1, PH_NOISY, "test/oo/propertyaccess.zep", 23 TSRMLS_CC);
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
	zephir_update_property_zval(&test, SL("test3"), &_6);
	zephir_update_property_zval(this_ptr, SL("a"), &test);
	zephir_read_property(&_9, this_ptr, SL("a"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_10, &_9, SL("test2"), PH_NOISY_CC | PH_READONLY);
	zephir_update_property_zval(this_ptr, SL("b"), &_10);
	zephir_read_property(&test1, this_ptr, SL("a"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_PropertyAccess, setPrivatevariable) {

	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &value);



	zephir_update_property_zval(this_ptr, SL("privateVariable"), value);
	RETURN_THISW();

}

PHP_METHOD(Test_Oo_PropertyAccess, getPrivatevariable) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "privateVariable");

}

