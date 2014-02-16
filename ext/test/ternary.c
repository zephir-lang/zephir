
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

	zval *a, *y, *_0 = NULL, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	ZEPHIR_INIT_VAR(_0);
	if (100) {
		ZVAL_LONG(_0, (1 + 100));
	} else {
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_INIT_VAR(_2);
		zephir_call_method(_2, a, "y");
		if (zephir_is_true(_2)) {
			zephir_call_method(_1, a, "x");
		} else {
			ZVAL_BOOL(_1, zephir_array_isset(a, y));
		}
		ZEPHIR_CPY_WRT(_0, _1);
	}
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Ternary, testTernaryComplex2) {

	zval *a, *y, *_0 = NULL, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	ZEPHIR_INIT_VAR(_0);
	if (100) {
		ZVAL_LONG(_0, (1 + 100));
	} else {
		ZEPHIR_INIT_VAR(_1);
		ZEPHIR_INIT_VAR(_2);
		zephir_call_method(_2, a, "y");
		if (zephir_is_true(_2)) {
			zephir_call_method(_1, a, "x");
		} else {
			ZVAL_BOOL(_1, zephir_array_isset(a, y));
		}
		ZEPHIR_CPY_WRT(_0, _1);
	}
	RETURN_MM_LONG((5 + zephir_get_numberval(_0)));

}

PHP_METHOD(Test_Ternary, testTernaryComplex3) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);



	ZEPHIR_INIT_VAR(_0);
	if ((Z_TYPE_P(a) == IS_RESOURCE)) {
		ZVAL_STRING(_0, "unknown", 1);
	} else {
		ZVAL_BOOL(_0, 0);
	}
	zephir_gettype(return_value, _0 TSRMLS_CC);
	RETURN_MM();

}

