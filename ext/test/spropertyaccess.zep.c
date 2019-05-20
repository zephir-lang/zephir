
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
#include "kernel/string.h"
#include "kernel/operators.h"


/**
 * Class with constructor + params
 */
ZEPHIR_INIT_CLASS(Test_SPropertyAccess) {

	ZEPHIR_REGISTER_CLASS(Test, SPropertyAccess, test, spropertyaccess, test_spropertyaccess_method_entry, 0);

	zend_declare_property_null(test_spropertyaccess_ce, SL("a"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(test_spropertyaccess_ce, SL("b"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_string(test_spropertyaccess_ce, SL("delimiter"), ".", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_string(test_spropertyaccess_ce, SL("_delimiterWithUnderscore"), ".", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_SPropertyAccess, __construct) {

	zval _0, _1, _2, _3, _4;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZEPHIR_MM_ZVAL_STRING(&_0, "abc");
	zephir_update_static_property(test_spropertyaccess_ce, ZEND_STRL("a"), &_0);
	zephir_read_static_property_ce(&_0, test_spropertyaccess_ce, SL("a"), PH_NOISY_CC | PH_READONLY);
	ZVAL_LONG(&_1, 0);
	ZVAL_LONG(&_2, 1);
	zephir_substr(&_3, &_0, 0 , 1 , 0);
	zephir_update_static_property(test_spropertyaccess_ce, ZEND_STRL("b"), &_3);
	zephir_read_static_property_ce(&_4, test_spropertyaccess_ce, SL("b"), PH_NOISY_CC);
	zephir_update_static_property(test_scallexternal_ce, ZEND_STRL("sproperty"), &_4);
	zephir_read_static_property_ce(&_4, test_scallexternal_ce, SL("sproperty"), PH_NOISY_CC);
	zephir_update_static_property(test_spropertyaccess_ce, ZEND_STRL("b"), &_4);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SPropertyAccess, testArgumentWithUnderscore) {

	zval *delimiter = NULL, delimiter_sub, __$null;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);

	zephir_fetch_params(1, 0, 1, &delimiter);

	if (!delimiter) {
		delimiter = &delimiter_sub;
		ZEPHIR_CPY_WRT(delimiter, &__$null);
	} else {
		ZEPHIR_SEPARATE_PARAM(delimiter);
	}


	if (ZEPHIR_IS_EMPTY(delimiter)) {
		zephir_read_static_property_ce(delimiter, test_spropertyaccess_ce, SL("_delimiterWithUnderscore"), PH_NOISY_CC);
	}
	RETURN_MM_CTOR(delimiter);

}

PHP_METHOD(Test_SPropertyAccess, testArgument) {

	zval *delimiter = NULL, delimiter_sub, __$null, _0$$3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0$$3);

	zephir_fetch_params(1, 0, 1, &delimiter);

	if (!delimiter) {
		delimiter = &delimiter_sub;
		ZEPHIR_CPY_WRT(delimiter, &__$null);
	} else {
		ZEPHIR_SEPARATE_PARAM(delimiter);
	}


	if (ZEPHIR_IS_EMPTY(delimiter)) {
		zephir_read_static_property_ce(&_0$$3, test_spropertyaccess_ce, SL("delimiter"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(delimiter, &_0$$3);
	}
	RETURN_MM_CTOR(delimiter);

}

