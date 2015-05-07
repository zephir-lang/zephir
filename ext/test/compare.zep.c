
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


ZEPHIR_INIT_CLASS(Test_Compare) {

	ZEPHIR_REGISTER_CLASS(Test, Compare, test, compare, test_compare_method_entry, 0);

	zend_declare_class_constant_double(test_compare_ce, SL("PI"), 3.14 TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Compare, isLessInt) {

	zval *a_param = NULL, *b_param = NULL;
	int a, b;

	zephir_fetch_params(0, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_BOOL(a < b);

}

PHP_METHOD(Test_Compare, isGreaterEqual) {

	zval *a_param = NULL, *b_param = NULL;
	int a, b;

	zephir_fetch_params(0, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_BOOL(a >= b);

}

PHP_METHOD(Test_Compare, isLessDouble) {

	zval *a_param = NULL, *b_param = NULL;
	double a, b;

	zephir_fetch_params(0, 2, 0, &a_param, &b_param);

	a = zephir_get_doubleval(a_param);
	b = zephir_get_doubleval(b_param);


	RETURN_BOOL(a < b);

}

PHP_METHOD(Test_Compare, isLessThenPi) {

	zval *a_param = NULL;
	double a;

	zephir_fetch_params(0, 1, 0, &a_param);

	a = zephir_get_doubleval(a_param);


	RETURN_BOOL(a < 3.14);

}

PHP_METHOD(Test_Compare, isMoreThenPi) {

	zval *a_param = NULL;
	double a;

	zephir_fetch_params(0, 1, 0, &a_param);

	a = zephir_get_doubleval(a_param);


	RETURN_BOOL(3.14 < a);

}

/**
 * @link https://github.com/phalcon/zephir/issues/411
 */
PHP_METHOD(Test_Compare, testVarWithStringEquals) {

	zval *str_param = NULL;
	zval *str = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str_param);

	if (unlikely(Z_TYPE_P(str_param) != IS_STRING && Z_TYPE_P(str_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'str' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(str_param) == IS_STRING)) {
		zephir_get_strval(str, str_param);
	} else {
		ZEPHIR_INIT_VAR(str);
		ZVAL_EMPTY_STRING(str);
	}


	if (ZEPHIR_IS_STRING_IDENTICAL(str, "wrong testing")) {
		ZEPHIR_INIT_NVAR(str);
		ZVAL_STRING(str, "NOK", 1);
	} else {
		if (ZEPHIR_IS_STRING_IDENTICAL(str, "another testing")) {
			ZEPHIR_INIT_NVAR(str);
			ZVAL_STRING(str, "NOK", 1);
		} else {
			if (ZEPHIR_IS_STRING_IDENTICAL(str, "testing")) {
				ZEPHIR_INIT_NVAR(str);
				ZVAL_STRING(str, "OK", 1);
			} else {
				ZEPHIR_INIT_NVAR(str);
				ZVAL_STRING(str, "NOK", 1);
			}
		}
	}
	RETURN_CTOR(str);

}

PHP_METHOD(Test_Compare, testVarEqualsNull) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	if (Z_TYPE_P(a) == IS_NULL) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);

}

PHP_METHOD(Test_Compare, testNullEqualsVar) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	if (Z_TYPE_P(a) == IS_NULL) {
		RETURN_BOOL(1);
	}
	RETURN_BOOL(0);

}

PHP_METHOD(Test_Compare, testNotIdenticalZeroVar) {

	int a = 5;


	RETURN_BOOL(0 != a);

}

PHP_METHOD(Test_Compare, testNotIdenticalZeroInt) {

	int a = 5;


	RETURN_BOOL(0 != a);

}

PHP_METHOD(Test_Compare, testNotIdenticalZeroLong) {

	long a = 5;


	RETURN_BOOL(0 != a);

}

