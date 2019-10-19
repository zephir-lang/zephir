
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_TryTest) {

	ZEPHIR_REGISTER_CLASS(Test, TryTest, test, trytest, test_trytest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_TryTest, testThrow1) {

	zval *this_ptr = getThis();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_exception_get_default(TSRMLS_C), "error", "test/trytest.zep", 10);
	return;

}

PHP_METHOD(Test_TryTest, testThrow2) {

	zval message, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&message);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&message);
	ZVAL_STRING(&message, "error");
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, zend_exception_get_default(TSRMLS_C));
	ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 28, &message);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_0, "test/trytest.zep", 16);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_TryTest, testTry1) {

	zval *this_ptr = getThis();



	/* try_start_1: */


	try_end_1:

	zend_clear_exception(TSRMLS_C);

}

PHP_METHOD(Test_TryTest, testTry2) {

	zval _0$$3, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, zend_exception_get_default(TSRMLS_C));
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "error!");
		ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 28, &_1$$3);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(&_0$$3, "test/trytest.zep", 27);
		goto try_end_1;


	try_end_1:

	zend_clear_exception(TSRMLS_C);

}

PHP_METHOD(Test_TryTest, testTry3) {

	zval _0$$3, _1$$3, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, zend_exception_get_default(TSRMLS_C));
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "error!");
		ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 28, &_1$$3);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(&_0$$3, "test/trytest.zep", 34);
		goto try_end_1;


	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_2);
		ZVAL_OBJ(&_2, EG(exception));
		Z_ADDREF_P(&_2);
		ZEPHIR_INIT_VAR(&_3);
		if (zephir_instance_of_ev(&_2, zend_exception_get_default(TSRMLS_C))) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&_3, &_2);
			RETURN_MM_STRING("error");
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry4) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, _0$$4, _1$$4, _2$$5, _3$$5, _4, _5, _6;
	zend_bool a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 28, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "test/trytest.zep", 46);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 99, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "test/trytest.zep", 48);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		ZEPHIR_INIT_VAR(&_5);
		ZEPHIR_INIT_VAR(&_6);
		if (zephir_instance_of_ev(&_4, spl_ce_RuntimeException)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&_5, &_4);
			RETURN_MM_STRING("domain error");
		} else {
			if (zephir_instance_of_ev(&_4, zend_exception_get_default(TSRMLS_C))) {
				zend_clear_exception(TSRMLS_C);
				ZEPHIR_CPY_WRT(&_6, &_4);
				RETURN_MM_STRING("error");
			}
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry5) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, _0$$4, _1$$4, _2$$5, _3$$5, _4, _5;
	zend_bool a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 28, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "test/trytest.zep", 63);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 99, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "test/trytest.zep", 65);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		ZEPHIR_INIT_VAR(&_5);
		if (zephir_instance_of_ev(&_4, spl_ce_RuntimeException)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&_5, &_4);
			RETURN_MM_STRING("any error");
		} else {
			if (zephir_instance_of_ev(&_4, zend_exception_get_default(TSRMLS_C))) {
				zend_clear_exception(TSRMLS_C);
				ZEPHIR_CPY_WRT(&_5, &_4);
				RETURN_MM_STRING("any error");
			}
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry6) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, e, _4, _0$$4, _1$$4, _2$$5, _3$$5;
	zend_bool a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 28, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "test/trytest.zep", 80);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 99, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "test/trytest.zep", 82);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		if (zephir_instance_of_ev(&_4, spl_ce_RuntimeException)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&e, &_4);
			RETURN_MM_STRING("domain error");
		} else {
			if (zephir_instance_of_ev(&_4, zend_exception_get_default(TSRMLS_C))) {
				zend_clear_exception(TSRMLS_C);
				ZEPHIR_CPY_WRT(&e, &_4);
				RETURN_MM_STRING("error");
			}
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry7) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, e, _4, _0$$4, _1$$4, _2$$5, _3$$5;
	zend_bool a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 28, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "test/trytest.zep", 99);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 99, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "test/trytest.zep", 101);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		if (zephir_instance_of_ev(&_4, spl_ce_RuntimeException)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&e, &_4);
			RETURN_MM_STRING("any error");
		} else {
			if (zephir_instance_of_ev(&_4, zend_exception_get_default(TSRMLS_C))) {
				zend_clear_exception(TSRMLS_C);
				ZEPHIR_CPY_WRT(&e, &_4);
				RETURN_MM_STRING("any error");
			}
		}
	}

}

PHP_METHOD(Test_TryTest, testTry8) {

	zval _0$$3, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, zend_exception_get_default(TSRMLS_C));
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "error 1!");
		ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 28, &_1$$3);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(&_0$$3, "test/trytest.zep", 111);
		goto try_end_1;


	try_end_1:

	zend_clear_exception(TSRMLS_C);
	ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "error 2!", "test/trytest.zep", 113);
	return;

}

PHP_METHOD(Test_TryTest, someMethod1) {

	zval *this_ptr = getThis();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_RuntimeException, "some external exception", "test/trytest.zep", 118);
	return;

}

PHP_METHOD(Test_TryTest, someMethod2) {

	zval *this_ptr = getThis();


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_DomainException, "some external exception", "test/trytest.zep", 123);
	return;

}

PHP_METHOD(Test_TryTest, testTry9) {

	zval e, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "somemethod1", NULL, 100);
		zephir_check_call_status_or_jump(try_end_1);
		RETURN_MM_STRING("not catched");

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_instance_of_ev(&_0, spl_ce_RuntimeException)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&e, &_0);
			RETURN_MM_STRING("domain error");
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry10) {

	zval e, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "somemethod2", NULL, 101);
		zephir_check_call_status_or_jump(try_end_1);
		RETURN_MM_STRING("not catched");

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_instance_of_ev(&_0, spl_ce_RuntimeException)) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&e, &_0);
			RETURN_MM_STRING("domain error");
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry11) {

	zval ex, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ex);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		RETURN_MM_STRING("test");

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_instance_of_ev(&_0, zend_exception_get_default(TSRMLS_C))) {
			zend_clear_exception(TSRMLS_C);
			ZEPHIR_CPY_WRT(&ex, &_0);
		}
	}

}

