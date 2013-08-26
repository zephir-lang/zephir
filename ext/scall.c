
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_Scall) {

	ZEPHIR_REGISTER_CLASS(Test, Scall, scall, test_scall_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Scall, testMethod1) {

	RETURN_STRING("hello public", 1);


}

PHP_METHOD(Test_Scall, testMethod2) {

	RETURN_STRING("hello protected", 1);


}

PHP_METHOD(Test_Scall, testMethod3) {

	RETURN_STRING("hello private", 1);


}

PHP_METHOD(Test_Scall, testMethod4) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b TSRMLS_CC);
	return;


}

PHP_METHOD(Test_Scall, testMethod5) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b TSRMLS_CC);
	return;


}

PHP_METHOD(Test_Scall, testMethod6) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b TSRMLS_CC);
	return;


}

PHP_METHOD(Test_Scall, testCall1) {

	ZEPHIR_CALL_SELF(return_value, this_ptr, test_scall_ce, "testmethod1");
	return;


}

PHP_METHOD(Test_Scall, testCall2) {

	ZEPHIR_CALL_SELF(return_value, this_ptr, test_scall_ce, "testmethod2");
	return;


}

PHP_METHOD(Test_Scall, testCall3) {

	ZEPHIR_CALL_SELF(return_value, this_ptr, test_scall_ce, "testmethod3");
	return;


}

PHP_METHOD(Test_Scall, testCall4) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	ZEPHIR_CALL_SELF_PARAMS_2(return_value, this_ptr, "testmethod4", a, b);
	return;


}

PHP_METHOD(Test_Scall, testCall5) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	ZEPHIR_CALL_SELF_PARAMS_2(return_value, this_ptr, "testmethod5", a, b);
	return;


}

PHP_METHOD(Test_Scall, testCall6) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	ZEPHIR_CALL_SELF_PARAMS_2(return_value, this_ptr, "testmethod6", a, b);
	return;


}

