
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


ZEPHIR_INIT_CLASS(Test_TryTest) {

	ZEPHIR_REGISTER_CLASS(Test, TryTest, test, trytest, test_trytest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_TryTest, testTry1) {


	//missing try-catch

}

PHP_METHOD(Test_TryTest, testTry2) {


	//missing try-catch

}

PHP_METHOD(Test_TryTest, testTry3) {


	//missing try-catch

}

PHP_METHOD(Test_TryTest, testTry4) {

	zval *a_param = NULL;
	zend_bool a;

	zephir_fetch_params(0, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);


	//missing try-catch

}

PHP_METHOD(Test_TryTest, testTry5) {

	zval *a_param = NULL;
	zend_bool a;

	zephir_fetch_params(0, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);


	//missing try-catch

}

PHP_METHOD(Test_TryTest, testTry6) {

	zval *a_param = NULL;
	zend_bool a;

	zephir_fetch_params(0, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);


	//missing try-catch

}

PHP_METHOD(Test_TryTest, testTry7) {

	zval *a_param = NULL;
	zend_bool a;

	zephir_fetch_params(0, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);


	//missing try-catch

}

