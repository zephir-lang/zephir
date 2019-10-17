
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"


ZEPHIR_INIT_CLASS(Test_Exceptions) {

	ZEPHIR_REGISTER_CLASS(Test, Exceptions, test, exceptions, test_exceptions_method_entry, 0);

	zend_declare_property_null(test_exceptions_ce, SL("internalExceptionCallable"), ZEND_ACC_PUBLIC);

	zend_declare_property_null(test_exceptions_ce, SL("exceptionCallable"), ZEND_ACC_PUBLIC);

	return SUCCESS;

}

PHP_METHOD(Test_Exceptions, testException1) {

	zval *this_ptr = getThis();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_exception_ce, "hello1", "test/exceptions.zep", 11);
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionStringEscape) {

	zval *this_ptr = getThis();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_exception_ce, "hello \"simple code\" test", "test/exceptions.zep", 16);
	return;

}

PHP_METHOD(Test_Exceptions, testException2) {

	zval msg, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&msg);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&msg);
	ZVAL_STRING(&msg, "hello2");
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 28, &msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_0, "test/exceptions.zep", 23);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testException3) {

	zval ex, msg;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ex);
	ZVAL_UNDEF(&msg);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&msg);
	ZVAL_STRING(&msg, "hello3");
	ZEPHIR_INIT_VAR(&ex);
	object_init_ex(&ex, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, &ex, "__construct", NULL, 28, &msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(&ex, "test/exceptions.zep", 31);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, getException) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, test_exception_ce);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello4");
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 28, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Exceptions, testException4) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getexception", NULL, 0);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_0, "test/exceptions.zep", 41);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testException5) {

	zval exception, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&exception);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&exception);
	object_init_ex(&exception, test_exception_ce);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello5");
	ZEPHIR_CALL_METHOD(NULL, &exception, "__construct", NULL, 28, &_0);
	zephir_check_call_status();
	zephir_throw_exception_debug(&exception, "test/exceptions.zep", 49);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionLiteral) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *type_param = NULL;
	zval type;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&type);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type_param);

	zephir_get_strval(&type, type_param);


	do {
		if (ZEPHIR_IS_STRING(&type, "string")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "Test", "test/exceptions.zep", 56);
			return;
		}
		if (ZEPHIR_IS_STRING(&type, "char")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "t", "test/exceptions.zep", 58);
			return;
		}
		if (ZEPHIR_IS_STRING(&type, "int")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "123", "test/exceptions.zep", 60);
			return;
		}
		if (ZEPHIR_IS_STRING(&type, "double")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "123.123", "test/exceptions.zep", 62);
			return;
		}
	} while(0);

	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Exceptions, testExceptionSprintf) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *name_param = NULL, _0, _1;
	zval name;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(&name, name_param);


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "Hello, %s");
	ZEPHIR_CALL_FUNCTION(&_1, "sprintf", NULL, 17, &_0, &name);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_1, "test/exceptions.zep", 68);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionConcat) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *framework_param = NULL, *language_param = NULL;
	zval framework, language, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&framework);
	ZVAL_UNDEF(&language);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &framework_param, &language_param);

	zephir_get_strval(&framework, framework_param);
	zephir_get_strval(&language, language_param);


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SVSV(&_0, "Framework ", &framework, " written using ", &language);
	zephir_throw_exception_debug(&_0, "test/exceptions.zep", 73);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionRethrow) {

	zval e, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "testexception1", NULL, 0);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_instance_of_ev(&_0, zend_exception_get_default(TSRMLS_C))) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&e, &_0);
			zephir_throw_exception_debug(&e, "test/exceptions.zep", 83);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Exceptions, testMultiException) {

	zend_bool _1$$4, _3$$7, _5$$10, _7$$13;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *returnValue, returnValue_sub, *exception, exception_sub, e, f, iexc, exc, _0, _2$$4, _4$$7, _6$$10, _8$$13;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&returnValue_sub);
	ZVAL_UNDEF(&exception_sub);
	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&f);
	ZVAL_UNDEF(&iexc);
	ZVAL_UNDEF(&exc);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2$$4);
	ZVAL_UNDEF(&_4$$7);
	ZVAL_UNDEF(&_6$$10);
	ZVAL_UNDEF(&_8$$13);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &returnValue, &exception);



	ZEPHIR_OBS_VAR(&iexc);
	zephir_read_property(&iexc, this_ptr, SL("internalExceptionCallable"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(&exc);
	zephir_read_property(&exc, this_ptr, SL("exceptionCallable"), PH_NOISY_CC);

	/* try_start_1: */

		zephir_throw_exception_debug(exception, "test/exceptions.zep", 95);
		goto try_end_1;


	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_instance_of_ev(&_0, test_exception_ce)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&e, &_0);
			_1$$4 = zephir_is_callable(&iexc);
			if (_1$$4) {
				ZEPHIR_CALL_ZVAL_FUNCTION(&_2$$4, &iexc, NULL, 0);
				zephir_check_call_status();
				_1$$4 = ZEPHIR_IS_FALSE_IDENTICAL(&_2$$4);
			}
			if (_1$$4) {
			} else {
				zephir_throw_exception_debug(&e, "test/exceptions.zep", 101);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			if (zephir_instance_of_ev(&_0, zend_exception_get_default(TSRMLS_C))) {
				zend_clear_exception(TSRMLS_C);
				ZEPHIR_CPY_WRT(&e, &_0);
				_3$$7 = zephir_is_callable(&exc);
				if (_3$$7) {
					ZEPHIR_CALL_ZVAL_FUNCTION(&_4$$7, &exc, NULL, 0);
					zephir_check_call_status();
					_3$$7 = ZEPHIR_IS_FALSE_IDENTICAL(&_4$$7);
				}
				if (_3$$7) {
				} else {
					zephir_throw_exception_debug(&e, "test/exceptions.zep", 108);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				if (zephir_is_instance_of(&_0, SL("RuntimeError"))) {
					zend_clear_exception(TSRMLS_C);
					ZEPHIR_CPY_WRT(&f, &_0);
					_5$$10 = zephir_is_callable(&exc);
					if (_5$$10) {
						ZEPHIR_CALL_ZVAL_FUNCTION(&_6$$10, &exc, NULL, 0);
						zephir_check_call_status();
						_5$$10 = ZEPHIR_IS_FALSE_IDENTICAL(&_6$$10);
					}
					if (_5$$10) {
					} else {
						zephir_throw_exception_debug(&f, "test/exceptions.zep", 115);
						ZEPHIR_MM_RESTORE();
						return;
					}
				} else {
					if (zephir_is_instance_of(&_0, SL("LogicError"))) {
						zend_clear_exception(TSRMLS_C);
						ZEPHIR_CPY_WRT(&f, &_0);
						_7$$13 = zephir_is_callable(&exc);
						if (_7$$13) {
							ZEPHIR_CALL_ZVAL_FUNCTION(&_8$$13, &exc, NULL, 0);
							zephir_check_call_status();
							_7$$13 = ZEPHIR_IS_FALSE_IDENTICAL(&_8$$13);
						}
						if (_7$$13) {
						} else {
							zephir_throw_exception_debug(&f, "test/exceptions.zep", 115);
							ZEPHIR_MM_RESTORE();
							return;
						}
					}
				}
			}
		}
	}
	RETVAL_ZVAL(returnValue, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Test_Exceptions, issue1325) {

	zval e, status, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&status);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(&status, this_ptr, "donoopexception", NULL, 29);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_instance_of_ev(&_0, zend_exception_get_default(TSRMLS_C))) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&e, &_0);
			ZEPHIR_INIT_NVAR(&status);
			ZVAL_STRING(&status, "woop");
		}
	}
	RETURN_MM_LONG(1);

}

PHP_METHOD(Test_Exceptions, doNoopException) {

	zval *this_ptr = getThis();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_exception_get_default(TSRMLS_C), "I am exception", "test/exceptions.zep", 140);
	return;

}

