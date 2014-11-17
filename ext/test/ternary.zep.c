
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
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Ternary) {

	ZEPHIR_REGISTER_CLASS(Test, Ternary, test, ternary, test_ternary_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Ternary, testTernary1) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	if (100) {
		ZVAL_LONG(_0, (1 + 100));
	} else {
		ZVAL_BOOL(_0, 0);
	}
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Ternary, testTernary2) {

	zval *b_param = NULL, *_0;
	zend_bool b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &b_param);

	b = zephir_get_boolval(b_param);


	ZEPHIR_INIT_VAR(_0);
	if (b) {
		ZVAL_STRING(_0, "foo", 1);
	} else {
		ZVAL_STRING(_0, "bar", 1);
	}
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Ternary, testTernaryComplex1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *y, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	ZEPHIR_INIT_VAR(_0);
	if (100) {
		ZVAL_LONG(_0, (1 + 100));
	} else {
		ZEPHIR_CALL_METHOD(&_1, a, "y", NULL);
		zephir_check_call_status();
		if (zephir_is_true(_1)) {
			ZEPHIR_CALL_METHOD(&_0, a, "x", NULL);
			zephir_check_call_status();
		} else {
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_BOOL(_0, zephir_array_isset(a, y));
		}
	}
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Ternary, testTernaryComplex2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *y, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	ZEPHIR_INIT_VAR(_0);
	if (100) {
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_LONG(_0, (1 + 100));
	} else {
		ZEPHIR_CALL_METHOD(&_1, a, "y", NULL);
		zephir_check_call_status();
		if (zephir_is_true(_1)) {
			ZEPHIR_CALL_METHOD(&_0, a, "x", NULL);
			zephir_check_call_status();
		} else {
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_BOOL(_0, zephir_array_isset(a, y));
		}
	}
	RETURN_MM_LONG((5 + zephir_get_numberval(_0)));

}

PHP_METHOD(Test_Ternary, testTernaryComplex3) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);




}

/**
 * @link https://github.com/phalcon/zephir/issues/297
 */
PHP_METHOD(Test_Ternary, testTernaryAfterLetVariable) {

	zval *s = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(s);
	ZVAL_LONG(s, 23);

	if (1 == 1) {
		ZEPHIR_INIT_NVAR(s);
		ZVAL_LONG(s, 3);
	} else {
		ZEPHIR_INIT_NVAR(s);
		ZVAL_LONG(s, 10);
	}
	RETURN_CCTOR(s);

}

