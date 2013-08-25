
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
 * Function calls
 */
ZEPHIR_INIT_CLASS(Test_Mcall) {

	ZEPHIR_REGISTER_CLASS(Test, Mcall, mcall, test_mcall_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Mcall, testMethod1) {

	RETURN_STRING("hello public", 1);


}

PHP_METHOD(Test_Mcall, testMethod2) {

	RETURN_STRING("hello protected", 1);


}

PHP_METHOD(Test_Mcall, testMethod3) {

	RETURN_STRING("hello private", 1);


}

PHP_METHOD(Test_Mcall, testMethod4) {

	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_add_function(_0, a, b TSRMLS_CC);
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testMethod5) {

	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_add_function(_0, a, b TSRMLS_CC);
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testMethod6) {

	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_add_function(_0, a, b TSRMLS_CC);
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testCall1) {

	zval *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method(_0, this_ptr, "testmethod1");
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testCall2) {

	zval *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method(_0, this_ptr, "testmethod2");
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testCall3) {

	zval *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(_0);
	zephir_call_method(_0, this_ptr, "testmethod3");
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testCall4) {

	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p2(_0, this_ptr, "testmethod4", a, b);
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testCall5) {

	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p2(_0, this_ptr, "testmethod5", a, b);
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Mcall, testCall6) {

	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p2(_0, this_ptr, "testmethod6", a, b);
	RETURN_CCTOR(_0);


}

