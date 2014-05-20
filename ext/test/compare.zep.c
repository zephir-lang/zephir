
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

