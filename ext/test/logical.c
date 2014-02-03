
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Logical) {

	ZEPHIR_REGISTER_CLASS(Test, Logical, test, logical, test_logical_method_entry, 0);


	return SUCCESS;

}

/* ---------- And ----------------*/
PHP_METHOD(Test_Logical, testAnd1) {

	zend_bool _0;


	_0 = (0) ? 1 : 0;
	if (_0) {
		_0 = (0) ? 1 : 0;
	}
	RETURN_BOOL(_0);

}

PHP_METHOD(Test_Logical, testAnd2) {

	zend_bool _0;


	_0 = (0) ? 1 : 0;
	if (_0) {
		_0 = (1.5) ? 1 : 0;
	}
	RETURN_BOOL(_0);

}

PHP_METHOD(Test_Logical, testAnd3) {

	zend_bool _0, _1;


	_0 = (0) ? 1 : 0;
	if (_0) {
		_1 = (1.5) ? 1 : 0;
		if (_1) {
			_1 = ('A') ? 1 : 0;
		}
		_0 = _1;
	}
	RETURN_BOOL(_0);

}

PHP_METHOD(Test_Logical, testAnd4) {

	zend_bool _0, _1;


	_0 = (1) ? 1 : 0;
	if (_0) {
		_1 = (0) ? 1 : 0;
		if (_1) {
			_1 = 0;
		}
		_0 = _1;
	}
	RETURN_BOOL(_0);

}

PHP_METHOD(Test_Logical, testAnd9) {

	zend_bool _0;
	int b;
	zval *a, *b_param = NULL, *_1;

	zephir_fetch_params(0, 2, 0, &a, &b_param);

		b = zephir_get_intval(b_param);


	_0 = (Z_TYPE_P(a) == IS_ARRAY);
	if (_0) {
		_0 = zephir_array_isset_long(a, b);
	}
	if (_0) {
		zephir_array_fetch_long(&_1, a, b, PH_NOISY | PH_READONLY TSRMLS_CC);
		RETURN_CTORW(_1);
	}
	RETURN_NULL();

}

PHP_METHOD(Test_Logical, testOr1) {

	zend_bool _0;


	_0 = (0) ? 1 : 0;
	if (!(_0)) {
		_0 = (1) ? 1 : 0;
	}
	RETURN_BOOL(_0);

}

PHP_METHOD(Test_Logical, testOr2) {

	zend_bool _0;


	_0 = (0) ? 1 : 0;
	if (!(_0)) {
		_0 = 1;
	}
	RETURN_BOOL(_0);

}

PHP_METHOD(Test_Logical, testMixed) {

	zend_bool _0, _1;
	int first, second;


	first = 0;
	second = 1;
	_0 = (first) ? 1 : 0;
	if (_0) {
		_1 = (second) ? 1 : 0;
		if (!(_1)) {
			_1 = (second) ? 1 : 0;
		}
		_0 = _1;
	}
	if (_0) {
		php_printf("ok");
	} else {
		php_printf("wrong");
	}

}

