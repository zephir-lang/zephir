
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
#include "kernel/concat.h"
#include "kernel/array.h"


/**
 * Class with constructor + params
 */
ZEPHIR_INIT_CLASS(Test_SPropertyAccess) {

	ZEPHIR_REGISTER_CLASS(Test, SPropertyAccess, test, spropertyaccess, test_spropertyaccess_method_entry, 0);

	zend_declare_property_null(test_spropertyaccess_ce, SL("a"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_property_null(test_spropertyaccess_ce, SL("b"), ZEND_ACC_PRIVATE|ZEND_ACC_STATIC);

	zend_declare_property_string(test_spropertyaccess_ce, SL("delimiter"), ".", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_property_string(test_spropertyaccess_ce, SL("_delimiterWithUnderscore"), ".", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_property_string(test_spropertyaccess_ce, SL("stringVar"), "", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_property_long(test_spropertyaccess_ce, SL("intVar"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_property_double(test_spropertyaccess_ce, SL("doubleVar"), 0.0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	zend_declare_property_null(test_spropertyaccess_ce, SL("arrayVar"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);

	return SUCCESS;

}

PHP_METHOD(Test_SPropertyAccess, __construct) {

	zval _0, _1, _2, _3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abc");
	zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("a"), &_0);
	zephir_read_static_property_ce(&_0, test_spropertyaccess_ce, SL("a"), PH_NOISY_CC | PH_READONLY);
	ZVAL_LONG(&_1, 0);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_INIT_VAR(&_3);
	zephir_substr(&_3, &_0, 0 , 1 , 0);
	zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("b"), &_3);
	ZEPHIR_OBS_VAR(&_4);
	zephir_read_static_property_ce(&_4, test_spropertyaccess_ce, SL("b"), PH_NOISY_CC);
	zephir_update_static_property_ce(test_scallexternal_ce, ZEND_STRL("sproperty"), &_4);
	ZEPHIR_OBS_NVAR(&_4);
	zephir_read_static_property_ce(&_4, test_scallexternal_ce, SL("sproperty"), PH_NOISY_CC);
	zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("b"), &_4);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SPropertyAccess, testArgumentWithUnderscore) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *delimiter = NULL, delimiter_sub, __$null;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &delimiter);

	if (!delimiter) {
		delimiter = &delimiter_sub;
		ZEPHIR_CPY_WRT(delimiter, &__$null);
	} else {
		ZEPHIR_SEPARATE_PARAM(delimiter);
	}


	if (ZEPHIR_IS_EMPTY(delimiter)) {
		ZEPHIR_OBS_NVAR(delimiter);
		zephir_read_static_property_ce(delimiter, test_spropertyaccess_ce, SL("_delimiterWithUnderscore"), PH_NOISY_CC);
	}
	RETVAL_ZVAL(delimiter, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Test_SPropertyAccess, testArgument) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *delimiter = NULL, delimiter_sub, __$null, _0$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0$$3);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &delimiter);

	if (!delimiter) {
		delimiter = &delimiter_sub;
		ZEPHIR_CPY_WRT(delimiter, &__$null);
	} else {
		ZEPHIR_SEPARATE_PARAM(delimiter);
	}


	if (ZEPHIR_IS_EMPTY(delimiter)) {
		ZEPHIR_OBS_VAR(&_0$$3);
		zephir_read_static_property_ce(&_0$$3, test_spropertyaccess_ce, SL("delimiter"), PH_NOISY_CC);
		ZEPHIR_CPY_WRT(delimiter, &_0$$3);
	}
	RETVAL_ZVAL(delimiter, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Test_SPropertyAccess, mutateStringVarInsideCycle) {

	zend_long _1, _2;
	zend_bool _0;
	zval i, _5, _3$$3, _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);

	ZEPHIR_MM_GROW();

	_2 = 3;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			_1 = _1;
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_STRING(&i, " + ");
			ZEPHIR_OBS_NVAR(&_3$$3);
			zephir_read_static_property_ce(&_3$$3, test_spropertyaccess_ce, SL("stringVar"), PH_NOISY_CC);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZEPHIR_CONCAT_VV(&_4$$3, &_3$$3, &i);
			zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("stringVar"), &_4$$3);
		}
	}
	zephir_read_static_property_ce(&_5, test_spropertyaccess_ce, SL("stringVar"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_5);

}

/**
 * @see https://github.com/phalcon/zephir/issues/1494
 */
PHP_METHOD(Test_SPropertyAccess, mutateIntVarInsideCycle) {

	zval _4, _3$$3;
	zend_bool _0;
	zend_long i = 0, _1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_3$$3);


	_2 = 3;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			_1 = _1;
			i = 42;
			ZEPHIR_INIT_ZVAL_NREF(_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("intVar"), &_3$$3);
		}
	}
	zephir_read_static_property_ce(&_4, test_spropertyaccess_ce, SL("intVar"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_4);

}

/**
 * @see https://github.com/phalcon/zephir/issues/1494
 */
PHP_METHOD(Test_SPropertyAccess, mutateDoubleVarInsideCycle) {

	zval _4, _3$$3;
	zend_long _1, _2;
	zend_bool _0;
	double i = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_3$$3);


	_2 = 3;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			_1 = _1;
			i = 3.14;
			ZEPHIR_INIT_ZVAL_NREF(_3$$3);
			ZVAL_DOUBLE(&_3$$3, i);
			zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("doubleVar"), &_3$$3);
		}
	}
	zephir_read_static_property_ce(&_4, test_spropertyaccess_ce, SL("doubleVar"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_4);

}

PHP_METHOD(Test_SPropertyAccess, mutateArrayVarInsideCycle) {

	zend_long _1, _2;
	zend_bool _0;
	zval i, j, _5, _3$$3, _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&j);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);

	ZEPHIR_MM_GROW();

	_2 = 3;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_INIT_NVAR(&j);
			zephir_create_array(&j, 1, 0);
			zephir_array_update_zval(&j, &i, &i, PH_COPY);
			zephir_read_static_property_ce(&_3$$3, test_spropertyaccess_ce, SL("arrayVar"), PH_NOISY_CC | PH_READONLY);
			ZEPHIR_INIT_NVAR(&_4$$3);
			zephir_add_function(&_4$$3, &_3$$3, &j);
			zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("arrayVar"), &_4$$3);
		}
	}
	zephir_read_static_property_ce(&_5, test_spropertyaccess_ce, SL("arrayVar"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_5);

}

void zephir_init_static_properties_Test_SPropertyAccess(TSRMLS_D) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_spropertyaccess_ce, ZEND_STRL("arrayVar"), &_0);
	ZEPHIR_MM_RESTORE();

}

