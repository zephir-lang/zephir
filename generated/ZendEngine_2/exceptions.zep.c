
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

	zend_declare_property_null(test_exceptions_ce, SL("internalExceptionCallable"), ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(test_exceptions_ce, SL("exceptionCallable"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Exceptions, testException1) {

	

	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_exception_ce, "hello1", "test/exceptions.zep", 11);
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionStringEscape) {

	

	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(test_exception_ce, "hello \"simple code\" test", "test/exceptions.zep", 16);
	return;

}

PHP_METHOD(Test_Exceptions, testException2) {

	zval *msg = NULL, *_0;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(msg);
	ZVAL_STRING(msg, "hello2", 1);
	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 24, msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(_0, "test/exceptions.zep", 23 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testException3) {

	zval *ex = NULL, *msg = NULL;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(msg);
	ZVAL_STRING(msg, "hello3", 1);
	ZEPHIR_INIT_VAR(ex);
	object_init_ex(ex, test_exception_ce);
	ZEPHIR_CALL_METHOD(NULL, ex, "__construct", NULL, 24, msg);
	zephir_check_call_status();
	zephir_throw_exception_debug(ex, "test/exceptions.zep", 31 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, getException) {

	zval *_0;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	object_init_ex(return_value, test_exception_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "hello4", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 24, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Exceptions, testException4) {

	zval *_0 = NULL;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "getexception", NULL, 0);
	zephir_check_call_status();
	zephir_throw_exception_debug(_0, "test/exceptions.zep", 41 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testException5) {

	zval *exception = NULL, *_0;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(exception);
	object_init_ex(exception, test_exception_ce);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "hello5", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_METHOD(NULL, exception, "__construct", NULL, 24, _0);
	zephir_check_temp_parameter(_0);
	zephir_check_call_status();
	zephir_throw_exception_debug(exception, "test/exceptions.zep", 49 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionLiteral) {

	zval *type_param = NULL;
	zval *type = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &type_param);

	zephir_get_strval(type, type_param);


	do {
		if (ZEPHIR_IS_STRING(type, "string")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "Test", "test/exceptions.zep", 56);
			return;
		}
		if (ZEPHIR_IS_STRING(type, "char")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "t", "test/exceptions.zep", 58);
			return;
		}
		if (ZEPHIR_IS_STRING(type, "int")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "123", "test/exceptions.zep", 60);
			return;
		}
		if (ZEPHIR_IS_STRING(type, "double")) {
			ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "123.123", "test/exceptions.zep", 62);
			return;
		}
	} while(0);

	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Exceptions, testExceptionSprintf) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *name_param = NULL, _0, *_1 = NULL;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(name, name_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "Hello, %s", 0);
	ZEPHIR_CALL_FUNCTION(&_1, "sprintf", NULL, 13, &_0, name);
	zephir_check_call_status();
	zephir_throw_exception_debug(_1, "test/exceptions.zep", 68 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionConcat) {

	zval *framework_param = NULL, *language_param = NULL;
	zval *framework = NULL, *language = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &framework_param, &language_param);

	zephir_get_strval(framework, framework_param);
	zephir_get_strval(language, language_param);


	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CONCAT_SVSV(_0, "Framework ", framework, " written using ", language);
	zephir_throw_exception_debug(_0, "test/exceptions.zep", 73 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_Exceptions, testExceptionRethrow) {

	zval *e = NULL, *_0;
	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "testexception1", NULL, 0);
		zephir_check_call_status_or_jump(try_end_1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_CPY_WRT(_0, EG(exception));
		if (zephir_instance_of_ev(_0, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(e, _0);
			zephir_throw_exception_debug(e, "test/exceptions.zep", 83 TSRMLS_CC);
			ZEPHIR_MM_RESTORE();
			return;
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Exceptions, testMultiException) {

	zend_bool _1$$4, _3$$7, _5$$10, _7$$13;
	int ZEPHIR_LAST_CALL_STATUS;
	zval *returnValue, *exception, *e = NULL, *f = NULL, *iexc = NULL, *exc = NULL, *_0, *_2$$4 = NULL, *_4$$7 = NULL, *_6$$10 = NULL, *_8$$13 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &returnValue, &exception);



	ZEPHIR_OBS_VAR(iexc);
	zephir_read_property_this(&iexc, this_ptr, SL("internalExceptionCallable"), PH_NOISY_CC);
	ZEPHIR_OBS_VAR(exc);
	zephir_read_property_this(&exc, this_ptr, SL("exceptionCallable"), PH_NOISY_CC);

	/* try_start_1: */

		zephir_throw_exception_debug(exception, "test/exceptions.zep", 95 TSRMLS_CC);
		goto try_end_1;


	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(_0);
		ZEPHIR_CPY_WRT(_0, EG(exception));
		if (zephir_instance_of_ev(_0, test_exception_ce TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(e, _0);
			_1$$4 = zephir_is_callable(iexc TSRMLS_CC);
			if (_1$$4) {
				ZEPHIR_CALL_ZVAL_FUNCTION(&_2$$4, iexc, NULL, 0);
				zephir_check_call_status();
				_1$$4 = ZEPHIR_IS_FALSE_IDENTICAL(_2$$4);
			}
			if (_1$$4) {
			} else {
				zephir_throw_exception_debug(e, "test/exceptions.zep", 101 TSRMLS_CC);
				ZEPHIR_MM_RESTORE();
				return;
			}
		} else {
			if (zephir_instance_of_ev(_0, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
				zend_clear_exception(TSRMLS_C);
				ZEPHIR_CPY_WRT(e, _0);
				_3$$7 = zephir_is_callable(exc TSRMLS_CC);
				if (_3$$7) {
					ZEPHIR_CALL_ZVAL_FUNCTION(&_4$$7, exc, NULL, 0);
					zephir_check_call_status();
					_3$$7 = ZEPHIR_IS_FALSE_IDENTICAL(_4$$7);
				}
				if (_3$$7) {
				} else {
					zephir_throw_exception_debug(e, "test/exceptions.zep", 108 TSRMLS_CC);
					ZEPHIR_MM_RESTORE();
					return;
				}
			} else {
				if (zephir_is_instance_of(_0, SL("RuntimeError") TSRMLS_CC)) {
					zend_clear_exception(TSRMLS_C);
					ZEPHIR_CPY_WRT(f, _0);
					_5$$10 = zephir_is_callable(exc TSRMLS_CC);
					if (_5$$10) {
						ZEPHIR_CALL_ZVAL_FUNCTION(&_6$$10, exc, NULL, 0);
						zephir_check_call_status();
						_5$$10 = ZEPHIR_IS_FALSE_IDENTICAL(_6$$10);
					}
					if (_5$$10) {
					} else {
						zephir_throw_exception_debug(f, "test/exceptions.zep", 115 TSRMLS_CC);
						ZEPHIR_MM_RESTORE();
						return;
					}
				} else {
					if (zephir_is_instance_of(_0, SL("LogicError") TSRMLS_CC)) {
						zend_clear_exception(TSRMLS_C);
						ZEPHIR_CPY_WRT(f, _0);
						_7$$13 = zephir_is_callable(exc TSRMLS_CC);
						if (_7$$13) {
							ZEPHIR_CALL_ZVAL_FUNCTION(&_8$$13, exc, NULL, 0);
							zephir_check_call_status();
							_7$$13 = ZEPHIR_IS_FALSE_IDENTICAL(_8$$13);
						}
						if (_7$$13) {
						} else {
							zephir_throw_exception_debug(f, "test/exceptions.zep", 115 TSRMLS_CC);
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

