
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
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Exceptions) {

	ZEPHIR_REGISTER_CLASS(Test, Exceptions, test, exceptions, test_exceptions_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Exceptions, testException1) {


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_exception_ce, "hello1", "test/exceptions.zep", 8);
	return;

}

PHP_METHOD(Test_Exceptions, testException2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *msg, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(msg);
	ZVAL_STRING(msg, "hello2", 1);
	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(_0, "test/exceptions.zep", 15 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testException3) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *ex, *msg;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(msg);
	ZVAL_STRING(msg, "hello3", 1);
	ZEPHIR_INIT_VAR(ex);
	object_init_ex(ex, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, ex, "__construct", NULL, msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(ex, "test/exceptions.zep", 23 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, getException) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, test_exception_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "hello4", 0);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Exceptions, testException4) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getexception",  NULL);
	zephir_check_call_status();
	zephir_throw_exception_debug(_0, "test/exceptions.zep", 33 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

