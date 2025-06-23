
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


ZEPHIR_INIT_CLASS(Stub_TryTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, TryTest, stub, trytest, stub_trytest_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_TryTest, testThrow1)
{

	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_ce_exception, "error", "stub/trytest.zep", 10);
	return;
}

PHP_METHOD(Stub_TryTest, testThrow2)
{
	zval message, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&message);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&message);
	ZVAL_STRING(&message, "error");
	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, zend_ce_exception);
	ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 31, &message);
	zephir_check_call_status();
	zephir_throw_exception_debug(&_0, "stub/trytest.zep", 16);
	ZEPHIR_MM_RESTORE();
	return;
}

PHP_METHOD(Stub_TryTest, testTry1)
{

	/* try_start_1: */
	try_end_1:

	zend_clear_exception();
}

PHP_METHOD(Stub_TryTest, testTry2)
{
	zval _0$$3, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	/* try_start_1: */

		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, zend_ce_exception);
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "error!");
		ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 31, &_1$$3);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(&_0$$3, "stub/trytest.zep", 27);
		goto try_end_1;
	try_end_1:

	zend_clear_exception();
}

PHP_METHOD(Stub_TryTest, testTry3)
{
	zval _0$$3, _1$$3, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	/* try_start_1: */

		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, zend_ce_exception);
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "error!");
		ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 31, &_1$$3);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(&_0$$3, "stub/trytest.zep", 34);
		goto try_end_1;
	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_2);
		ZVAL_OBJ(&_2, EG(exception));
		Z_ADDREF_P(&_2);
		ZEPHIR_INIT_VAR(&_3);
		if (zephir_is_instance_of(&_2, SL("Exception"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&_3, &_2);
			RETURN_MM_STRING("error");
		}
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_TryTest, testTry4)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, _0$$4, _1$$4, _2$$5, _3$$5, _4, _5, _6;
	zend_bool a;

	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_ce_exception);
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 31, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "stub/trytest.zep", 46);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 107, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "stub/trytest.zep", 48);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		ZEPHIR_INIT_VAR(&_5);
		ZEPHIR_INIT_VAR(&_6);
		if (zephir_is_instance_of(&_4, SL("RuntimeException"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&_5, &_4);
			RETURN_MM_STRING("domain error");
		} else {
			if (zephir_is_instance_of(&_4, SL("Exception"))) {
				zend_clear_exception();
				ZEPHIR_CPY_WRT(&_6, &_4);
				RETURN_MM_STRING("error");
			}
		}
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_TryTest, testTry5)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, _0$$4, _1$$4, _2$$5, _3$$5, _4, _5;
	zend_bool a;

	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_ce_exception);
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 31, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "stub/trytest.zep", 63);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 107, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "stub/trytest.zep", 65);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		ZEPHIR_INIT_VAR(&_5);
		if (zephir_is_instance_of(&_4, SL("RuntimeException"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&_5, &_4);
			RETURN_MM_STRING("any error");
		} else {
			if (zephir_is_instance_of(&_4, SL("Exception"))) {
				zend_clear_exception();
				ZEPHIR_CPY_WRT(&_5, &_4);
				RETURN_MM_STRING("any error");
			}
		}
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_TryTest, testTry6)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, e, _4, _0$$4, _1$$4, _2$$5, _3$$5;
	zend_bool a;

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_ce_exception);
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 31, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "stub/trytest.zep", 80);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 107, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "stub/trytest.zep", 82);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		if (zephir_is_instance_of(&_4, SL("RuntimeException"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&e, &_4);
			RETURN_MM_STRING("domain error");
		} else {
			if (zephir_is_instance_of(&_4, SL("Exception"))) {
				zend_clear_exception();
				ZEPHIR_CPY_WRT(&e, &_4);
				RETURN_MM_STRING("error");
			}
		}
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_TryTest, testTry7)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, e, _4, _0$$4, _1$$4, _2$$5, _3$$5;
	zend_bool a;

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0$$4);
	ZVAL_UNDEF(&_1$$4);
	ZVAL_UNDEF(&_2$$5);
	ZVAL_UNDEF(&_3$$5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(&_0$$4);
			object_init_ex(&_0$$4, zend_ce_exception);
			ZEPHIR_INIT_VAR(&_1$$4);
			ZVAL_STRING(&_1$$4, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_0$$4, "__construct", NULL, 31, &_1$$4);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_0$$4, "stub/trytest.zep", 99);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_VAR(&_2$$5);
			object_init_ex(&_2$$5, spl_ce_RuntimeException);
			ZEPHIR_INIT_VAR(&_3$$5);
			ZVAL_STRING(&_3$$5, "error!");
			ZEPHIR_CALL_METHOD(NULL, &_2$$5, "__construct", NULL, 107, &_3$$5);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(&_2$$5, "stub/trytest.zep", 101);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_4);
		ZVAL_OBJ(&_4, EG(exception));
		Z_ADDREF_P(&_4);
		if (zephir_is_instance_of(&_4, SL("RuntimeException"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&e, &_4);
			RETURN_MM_STRING("any error");
		} else {
			if (zephir_is_instance_of(&_4, SL("Exception"))) {
				zend_clear_exception();
				ZEPHIR_CPY_WRT(&e, &_4);
				RETURN_MM_STRING("any error");
			}
		}
	}
}

PHP_METHOD(Stub_TryTest, testTry8)
{
	zval _0$$3, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	/* try_start_1: */

		ZEPHIR_INIT_VAR(&_0$$3);
		object_init_ex(&_0$$3, zend_ce_exception);
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_STRING(&_1$$3, "error 1!");
		ZEPHIR_CALL_METHOD(NULL, &_0$$3, "__construct", NULL, 31, &_1$$3);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(&_0$$3, "stub/trytest.zep", 111);
		goto try_end_1;
	try_end_1:

	zend_clear_exception();
	ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_ce_exception, "error 2!", "stub/trytest.zep", 113);
	return;
}

PHP_METHOD(Stub_TryTest, someMethod1)
{

	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_RuntimeException, "some external exception", "stub/trytest.zep", 118);
	return;
}

PHP_METHOD(Stub_TryTest, someMethod2)
{

	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_DomainException, "some external exception", "stub/trytest.zep", 123);
	return;
}

PHP_METHOD(Stub_TryTest, testTry9)
{
	zval e, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "somemethod1", NULL, 108);
		zephir_check_call_status_or_jump(try_end_1);
		RETURN_MM_STRING("not catched");

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_is_instance_of(&_0, SL("RuntimeException"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&e, &_0);
			RETURN_MM_STRING("domain error");
		}
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_TryTest, testTry10)
{
	zval e, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&e);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "somemethod2", NULL, 109);
		zephir_check_call_status_or_jump(try_end_1);
		RETURN_MM_STRING("not catched");

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_is_instance_of(&_0, SL("RuntimeException"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&e, &_0);
			RETURN_MM_STRING("domain error");
		}
	}
	RETURN_MM_BOOL(0);
}

PHP_METHOD(Stub_TryTest, testTry11)
{
	zval ex, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&ex);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	/* try_start_1: */

		RETURN_MM_STRING("test");

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_VAR(&_0);
		ZVAL_OBJ(&_0, EG(exception));
		Z_ADDREF_P(&_0);
		if (zephir_is_instance_of(&_0, SL("Exception"))) {
			zend_clear_exception();
			ZEPHIR_CPY_WRT(&ex, &_0);
		}
	}
}

