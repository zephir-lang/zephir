
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
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Exceptions) {

	ZEPHIR_REGISTER_CLASS(Test, Exceptions, exceptions, test_exceptions_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Exceptions, testException1) {


	ZEPHIR_THROW_EXCEPTION_STRW(test_exception_ce, "hello1");
	return;

}

PHP_METHOD(Test_Exceptions, testException2) {

	zval *msg, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(msg);
	ZVAL_STRING(msg, "hello2", 1);
	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, test_exception_ce);
	zephir_call_method_p1_noret(_0, "__construct", msg);
	zephir_throw_exception(_0 TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Exceptions, testException3) {

	zval *ex, *msg;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(msg);
	ZVAL_STRING(msg, "hello3", 1);
	ZEPHIR_INIT_VAR(ex);
	object_init_ex(ex, test_exception_ce);
	zephir_call_method_p1_noret(ex, "__construct", msg);
	zephir_throw_exception(ex TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Exceptions, getException) {

	zval *_0;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, test_exception_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "hello4", 1);
	zephir_call_method_p1_noret(return_value, "__construct", _0);
	RETURN_MM();

}

PHP_METHOD(Test_Exceptions, testException4) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_call_method(_0, this_ptr, "getexception");
	zephir_throw_exception(_0 TSRMLS_CC);
	return;

}

