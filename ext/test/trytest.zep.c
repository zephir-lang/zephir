
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
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_TryTest) {

	ZEPHIR_REGISTER_CLASS(Test, TryTest, test, trytest, test_trytest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_TryTest, testThrow1) {


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_exception_get_default(TSRMLS_C), "error", "test/trytest.zep", 10);
	return;

}

PHP_METHOD(Test_TryTest, testThrow2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *message, *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(message);
	ZVAL_STRING(message, "error", 1);

	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
	ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, message);
	zephir_check_call_status();
	zephir_throw_exception_debug(_0, "test/trytest.zep", 16 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();
	return;

}

PHP_METHOD(Test_TryTest, testTry1) {



	/* try_start_1: */


	try_end_1:

	zend_clear_exception(TSRMLS_C);

}

PHP_METHOD(Test_TryTest, testTry2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, _1);
		zephir_check_temp_parameter(_1);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(_0, "test/trytest.zep", 27 TSRMLS_CC);
		goto try_end_1;


	try_end_1:

	zend_clear_exception(TSRMLS_C);

}

PHP_METHOD(Test_TryTest, testTry3) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL, *_1;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, _1);
		zephir_check_temp_parameter(_1);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(_0, "test/trytest.zep", 34 TSRMLS_CC);
		goto try_end_1;


	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_NVAR(_0);
		ZEPHIR_CPY_WRT(_0, EG(exception));
		if (zephir_instance_of_ev(_0, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("error", 1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry4) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, *_0 = NULL, *_1 = NULL;
	zend_bool a;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(_0);
			object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 46 TSRMLS_CC);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_NVAR(_0);
			object_init_ex(_0, spl_ce_RuntimeException);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 77, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 48 TSRMLS_CC);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_NVAR(_0);
		ZEPHIR_CPY_WRT(_0, EG(exception));
		if (zephir_instance_of_ev(_0, spl_ce_RuntimeException TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("domain error", 1);
		}
		ZEPHIR_INIT_NVAR(_0);
		ZEPHIR_CPY_WRT(_0, EG(exception));
		if (zephir_instance_of_ev(_0, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("error", 1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry5) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, *_0 = NULL, *_1 = NULL;
	zend_bool a;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(_0);
			object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 63 TSRMLS_CC);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_NVAR(_0);
			object_init_ex(_0, spl_ce_RuntimeException);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 77, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 65 TSRMLS_CC);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_INIT_NVAR(_0);
		ZEPHIR_CPY_WRT(_0, EG(exception));
		if (zephir_instance_of_ev(_0, spl_ce_RuntimeException TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("any error", 1);
		}
		if (zephir_instance_of_ev(_0, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("any error", 1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry6) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, *e = NULL, *_0 = NULL, *_1 = NULL;
	zend_bool a;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(_0);
			object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 80 TSRMLS_CC);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_NVAR(_0);
			object_init_ex(_0, spl_ce_RuntimeException);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 77, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 82 TSRMLS_CC);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_instance_of_ev(e, spl_ce_RuntimeException TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("domain error", 1);
		}
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_instance_of_ev(e, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("error", 1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry7) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, *e = NULL, *_0 = NULL, *_1 = NULL;
	zend_bool a;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	a = zephir_get_boolval(a_param);



	/* try_start_1: */

		if (a) {
			ZEPHIR_INIT_VAR(_0);
			object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
			ZEPHIR_INIT_VAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 99 TSRMLS_CC);
			goto try_end_1;

		} else {
			ZEPHIR_INIT_NVAR(_0);
			object_init_ex(_0, spl_ce_RuntimeException);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_STRING(_1, "error!", ZEPHIR_TEMP_PARAM_COPY);
			ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 77, _1);
			zephir_check_temp_parameter(_1);
			zephir_check_call_status_or_jump(try_end_1);
			zephir_throw_exception_debug(_0, "test/trytest.zep", 101 TSRMLS_CC);
			goto try_end_1;

		}

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_instance_of_ev(e, spl_ce_RuntimeException TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("any error", 1);
		}
		if (zephir_instance_of_ev(e, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("any error", 1);
		}
	}

}

PHP_METHOD(Test_TryTest, testTry8) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_INIT_VAR(_0);
		object_init_ex(_0, zend_exception_get_default(TSRMLS_C));
		ZEPHIR_INIT_VAR(_1);
		ZVAL_STRING(_1, "error 1!", ZEPHIR_TEMP_PARAM_COPY);
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 28, _1);
		zephir_check_temp_parameter(_1);
		zephir_check_call_status_or_jump(try_end_1);
		zephir_throw_exception_debug(_0, "test/trytest.zep", 111 TSRMLS_CC);
		goto try_end_1;


	try_end_1:

	zend_clear_exception(TSRMLS_C);
	ZEPHIR_THROW_EXCEPTION_DEBUG_STR(zend_exception_get_default(TSRMLS_C), "error 2!", "test/trytest.zep", 113);
	return;

}

PHP_METHOD(Test_TryTest, someMethod1) {


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_RuntimeException, "some external exception", "test/trytest.zep", 118);
	return;

}

PHP_METHOD(Test_TryTest, someMethod2) {


	ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(spl_ce_DomainException, "some external exception", "test/trytest.zep", 123);
	return;

}

PHP_METHOD(Test_TryTest, testTry9) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *e = NULL;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "somemethod1", NULL, 78);
		zephir_check_call_status_or_jump(try_end_1);
		RETURN_MM_STRING("not catched", 1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_instance_of_ev(e, spl_ce_RuntimeException TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("domain error", 1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry10) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *e = NULL;

	ZEPHIR_MM_GROW();


	/* try_start_1: */

		ZEPHIR_CALL_METHOD(NULL, this_ptr, "somemethod2", NULL, 79);
		zephir_check_call_status_or_jump(try_end_1);
		RETURN_MM_STRING("not catched", 1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(e, EG(exception));
		if (zephir_instance_of_ev(e, spl_ce_RuntimeException TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
			RETURN_MM_STRING("domain error", 1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_TryTest, testTry11) {

	zval *ex = NULL;



	/* try_start_1: */

		RETURN_STRING("test", 1);

	try_end_1:

	if (EG(exception)) {
		ZEPHIR_CPY_WRT(ex, EG(exception));
		if (zephir_instance_of_ev(ex, zend_exception_get_default(TSRMLS_C) TSRMLS_CC)) {
			zend_clear_exception(TSRMLS_C);
		}
	}

}

