
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


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_Scall) {

	ZEPHIR_REGISTER_CLASS_EX(Test, Scall, test, scall, test_scallparent_ce, test_scall_method_entry, 0);

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

PHP_METHOD(Test_Scall, testMethod7) {


	object_init(return_value);
	return;

}

PHP_METHOD(Test_Scall, testCall1) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_self(return_value, this_ptr, "testmethod1");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall2) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_self(return_value, this_ptr, "testmethod2");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall3) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_self(return_value, this_ptr, "testmethod3");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall4) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_self_p2(return_value, this_ptr, "testmethod4", a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall5) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_self_p2(return_value, this_ptr, "testmethod5", a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall6) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_self_p2(return_value, this_ptr, "testmethod6", a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall7) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_self(return_value, this_ptr, "testmethod1");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall8) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_self(return_value, this_ptr, "testmethod2");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall9) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_self(return_value, this_ptr, "testmethod3");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall10) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_self_p2(return_value, this_ptr, "testmethod4", a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall11) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_self_p2(return_value, this_ptr, "testmethod5", a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall12) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_self_p2(return_value, this_ptr, "testmethod6", a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall13) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_parent(return_value, this_ptr, test_scall_ce, "testmethod1");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall14) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_parent(return_value, this_ptr, test_scall_ce, "testmethod2");
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Scall, testCall15) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	zephir_call_self(return_value, this_ptr, "testmethod7");
	zephir_check_call_status();
	RETURN_MM();

}

