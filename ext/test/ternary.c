
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
#include "kernel/operators.h"
#include "kernel/memory.h"
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


	if (100) {
		ZVAL_LONG(return_value, (1 + 100));
	} else {
		ZVAL_BOOL(return_value, 0);
	}
	return;

}

PHP_METHOD(Test_Ternary, testTernary2) {

	zval *b_param = NULL;
	zend_bool b;

	zephir_fetch_params(0, 1, 0, &b_param);

	b = zephir_get_boolval(b_param);


	if (b) {
		ZVAL_STRING(return_value, "foo", 1);
	} else {
		ZVAL_STRING(return_value, "bar", 1);
	}
	return;

}

PHP_METHOD(Test_Ternary, testTernaryComplex1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *y, *_0 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	if (100) {
		ZVAL_LONG(return_value, (1 + 100));
	} else {
		ZEPHIR_CALL_METHOD(&_0, a, "y",  NULL);
		zephir_check_call_status();
		if (zephir_is_true(_0)) {
			ZEPHIR_RETURN_CALL_METHOD(a, "x", NULL);
			zephir_check_call_status();
		} else {
			ZVAL_BOOL(return_value, zephir_array_isset(a, y));
		}
	}
	RETURN_MM();

}

PHP_METHOD(Test_Ternary, testTernaryComplex2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *y, *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	ZEPHIR_INIT_VAR(_0);
	if (100) {
		ZEPHIR_INIT_BNVAR(_0);
		ZVAL_LONG(_0, (1 + 100));
	} else {
		ZEPHIR_INIT_BNVAR(_0);
		ZEPHIR_CALL_METHOD(&_1, a, "y",  NULL);
		zephir_check_call_status();
		if (zephir_is_true(_1)) {
			ZEPHIR_CALL_METHOD(&_0, a, "x",  NULL);
			zephir_check_call_status();
		} else {
			ZEPHIR_INIT_BNVAR(_0);
			ZVAL_BOOL(_0, zephir_array_isset(a, y));
		}
	}
	RETURN_MM_LONG((5 + zephir_get_numberval(_0)));

}

PHP_METHOD(Test_Ternary, testTernaryComplex3) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);




}

