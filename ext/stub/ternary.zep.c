
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/string.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Stub_Ternary) {

	ZEPHIR_REGISTER_CLASS(Stub, Ternary, stub, ternary, stub_ternary_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Ternary, testTernary1) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	if (100) {
		ZVAL_LONG(&_0, (1 + 100));
	} else {
		ZVAL_BOOL(&_0, 0);
	}
	RETURN_CCTOR(&_0);

}

PHP_METHOD(Stub_Ternary, testTernary2) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0;
	zend_bool b;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &b_param);

	b = zephir_get_boolval(b_param);


	ZEPHIR_INIT_VAR(&_0);
	if (b) {
		ZVAL_STRING(&_0, "foo");
	} else {
		ZVAL_STRING(&_0, "bar");
	}
	RETURN_CCTOR(&_0);

}

PHP_METHOD(Stub_Ternary, testTernaryComplex1) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *y, y_sub, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&y_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(y)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	ZEPHIR_INIT_VAR(&_0);
	if (100) {
		ZVAL_LONG(&_0, (1 + 100));
	} else {
		ZEPHIR_CALL_METHOD(&_1, a, "y", NULL, 0);
		zephir_check_call_status();
		if (zephir_is_true(&_1)) {
			ZEPHIR_CALL_METHOD(&_0, a, "x", NULL, 0);
			zephir_check_call_status();
		} else {
			ZEPHIR_INIT_NVAR(&_0);
			ZVAL_BOOL(&_0, zephir_array_isset(a, y));
		}
	}
	RETURN_CCTOR(&_0);

}

PHP_METHOD(Stub_Ternary, testTernaryComplex2) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *y, y_sub, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&y_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(y)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &y);



	ZEPHIR_INIT_VAR(&_0);
	if (100) {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_LONG(&_0, (1 + 100));
	} else {
		ZEPHIR_CALL_METHOD(&_1, a, "y", NULL, 0);
		zephir_check_call_status();
		if (zephir_is_true(&_1)) {
			ZEPHIR_CALL_METHOD(&_0, a, "x", NULL, 0);
			zephir_check_call_status();
		} else {
			ZEPHIR_INIT_NVAR(&_0);
			ZVAL_BOOL(&_0, zephir_array_isset(a, y));
		}
	}
	RETURN_MM_LONG((5 + zephir_get_numberval(&_0)));

}

PHP_METHOD(Stub_Ternary, testTernaryComplex3) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);



	ZEPHIR_INIT_VAR(&_0);
	if (Z_TYPE_P(a) == IS_RESOURCE) {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_STRING(&_0, "unknown");
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_BOOL(&_0, 0);
	}
	zephir_gettype(return_value, &_0);
	RETURN_MM();

}

/**
 * @link https://github.com/zephir-lang/zephir/issues/665
 */
PHP_METHOD(Stub_Ternary, testTernaryWithPromotedTemporaryVariable) {

	zval var2, var3, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&var2);
	ZVAL_UNDEF(&var3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&var2);
	zephir_create_array(&var2, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "_b_");
	zephir_array_fast_append(&var2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "_c_");
	zephir_array_fast_append(&var2, &_0);
	ZEPHIR_INIT_VAR(&_1);
	if (zephir_array_isset_long(&var2, 1)) {
		ZEPHIR_OBS_NVAR(&_1);
		zephir_array_fetch_long(&_1, &var2, 1, PH_NOISY, "stub/ternary.zep", 44);
	} else {
		ZEPHIR_INIT_NVAR(&_1);
		ZVAL_STRING(&_1, "");
	}
	ZEPHIR_INIT_VAR(&var3);
	zephir_fast_explode_str(&var3, SL("_"), &_1, LONG_MAX);
	RETURN_CCTOR(&var3);

}

/**
 * @link https://github.com/zephir-lang/zephir/issues/297
 */
PHP_METHOD(Stub_Ternary, testTernaryAfterLetVariable) {

	zval s;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&s);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&s);
	ZVAL_LONG(&s, 23);
	if (1 == 1) {
		ZEPHIR_INIT_NVAR(&s);
		ZVAL_LONG(&s, 3);
	} else {
		ZEPHIR_INIT_NVAR(&s);
		ZVAL_LONG(&s, 10);
	}
	RETURN_CCTOR(&s);

}

PHP_METHOD(Stub_Ternary, testShortTernary) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);



	ZEPHIR_INIT_VAR(&_0);
	if (!(zephir_is_true(a))) {
		ZVAL_BOOL(&_0, 0);
	} else {
		ZEPHIR_CPY_WRT(&_0, a);
	}
	RETURN_CCTOR(&_0);

}

PHP_METHOD(Stub_Ternary, testShortTernaryComplex) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *left, left_sub, *value, value_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&left_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(left)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &left, &value);



	ZEPHIR_INIT_VAR(&_0);
	if (!(zephir_is_true(left))) {
		ZEPHIR_CPY_WRT(&_0, value);
	} else {
		ZEPHIR_CPY_WRT(&_0, left);
	}
	RETURN_CCTOR(&_0);

}

