
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


ZEPHIR_INIT_CLASS(Test_Operator) {

	ZEPHIR_REGISTER_CLASS(Test, Operator, test, operator, test_operator_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Operator, testIdentical) {

	zval *param1, *param2;

	zephir_fetch_params(0, 2, 0, &param1, &param2);



	RETURN_BOOL(ZEPHIR_IS_IDENTICAL(param1, param2));

}

PHP_METHOD(Test_Operator, testExponential) {

	zval *a, *b, *c, _0;

	zephir_fetch_params(0, 3, 0, &a, &b, &c);



	ZEPHIR_SINIT_VAR(_0);
	zephir_exp_function(&_0, b, c TSRMLS_CC);
	zephir_exp_function(return_value, a, &_0 TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Operator, testExponential2) {


	RETURN_LONG(81);

}

PHP_METHOD(Test_Operator, testExponential3) {

	zval *a_param = NULL, *b_param = NULL;
	int a, b;

	zephir_fetch_params(0, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_LONG((a ** b));

}

