
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
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_Exceptions) {

	ZEPHIR_REGISTER_CLASS(Test, Exceptions, test, exceptions, test_exceptions_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Exceptions, testException1) {

	ZEPHIR_INIT_THIS();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_exception_ce, "hello1", "test/exceptions.zep", 8);
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionStringEscape) {

	ZEPHIR_INIT_THIS();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_exception_ce, "hello \"simple code\" test", "test/exceptions.zep", 13);
	return;

}

PHP_METHOD(Test_Exceptions, testException2) {

	zval msg, _0;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&msg);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&msg);
	ZVAL_STRING(&msg, "hello2");
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 24, &msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_0, "test/exceptions.zep", 20 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testException3) {

	zval ex, msg;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&ex);
	ZVAL_UNDEF(&msg);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&msg);
	ZVAL_STRING(&msg, "hello3");
	ZEPHIR_INIT_VAR(&ex);
	object_init_ex(&ex, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, &ex, "__construct", NULL, 24, &msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(&ex, "test/exceptions.zep", 28 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, getException) {

	zval _0;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, test_exception_ce);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello4");
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 24, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Exceptions, testException4) {

	zval _0;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getexception", NULL, 0);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_0, "test/exceptions.zep", 38 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testException5) {

	zval exception, _0;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&exception);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&exception);
	object_init_ex(&exception, test_exception_ce);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello5");
	ZEPHIR_CALL_METHOD(NULL, &exception, "__construct", NULL, 24, &_0);
	zephir_check_call_status();
	zephir_throw_exception_debug(&exception, "test/exceptions.zep", 46 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionLiteral) {

	zval *type_param = NULL;
	zval type;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&type);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type_param);

	zephir_get_strval(&type, type_param);


	do {
		if (ZEPHIR_IS_STRING(&type, "string")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "Test", "test/exceptions.zep", 53);
			return;
		}
		if (ZEPHIR_IS_STRING(&type, "char")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "t", "test/exceptions.zep", 55);
			return;
		}
		if (ZEPHIR_IS_STRING(&type, "int")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "123", "test/exceptions.zep", 57);
			return;
		}
		if (ZEPHIR_IS_STRING(&type, "double")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "123.123", "test/exceptions.zep", 59);
			return;
		}
	} while(0);

	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Exceptions, testExceptionSprintf) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *name_param = NULL, _0, _1;
	zval name;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(&name, name_param);


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "Hello, %s");
	ZEPHIR_CALL_FUNCTION(&_1, "sprintf", NULL, 4, &_0, &name);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_1, "test/exceptions.zep", 65 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionConcat) {

	zval *framework_param = NULL, *language_param = NULL;
	zval framework, language, _0;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&framework);
	ZVAL_UNDEF(&language);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &framework_param, &language_param);

	zephir_get_strval(&framework, framework_param);
	zephir_get_strval(&language, language_param);


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SVSV(&_0, "Framework ", &framework, " written using ", &language);
	zephir_throw_exception_debug(&_0, "test/exceptions.zep", 70 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionRethrow) {

	zval e;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&e);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "testexception1", NULL, 0);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZVAL_OBJ(&e, EG(exception));
		Z_ADDREF_P(&e);
		if (zephir_instance_of_ev(&e, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			zephir_throw_exception_debug(&e, "test/exceptions.zep", 80 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

