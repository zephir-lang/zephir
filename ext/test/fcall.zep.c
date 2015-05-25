
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
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/math.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/file.h"
#include "kernel/array.h"


/**
 * Function calls
 */
ZEPHIR_INIT_CLASS(Test_Fcall) {

	ZEPHIR_REGISTER_CLASS(Test, Fcall, test, fcall, test_fcall_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Fcall, testCall1) {

	zval _0, _1;


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "hello", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "h", 0);
	zephir_fast_strpos(return_value, &_0, &_1, 0 );
	return;

}

PHP_METHOD(Test_Fcall, testCall2) {

	zval _0 = zval_used_for_init, _1 = zval_used_for_init;


	while (1) {
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_LONG(&_0, 0);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_LONG(&_1, 100);
		RETURN_LONG(zephir_mt_rand(zephir_get_intval(&_0), zephir_get_intval(&_1) TSRMLS_CC));
	}

}

PHP_METHOD(Test_Fcall, testCall3) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL, *_3 = NULL;
	zval *handle = NULL, *handle2 = NULL, *buffer = NULL, _0 = zval_used_for_init, _1 = zval_used_for_init;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "inputfile.txt", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "r", 0);
	ZEPHIR_CALL_FUNCTION(&handle, "fopen", &_2, 30, &_0, &_1);
	zephir_check_call_status();
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_STRING(&_0, "outputfile.txt", 0);
	ZEPHIR_SINIT_NVAR(_1);
	ZVAL_STRING(&_1, "w", 0);
	ZEPHIR_CALL_FUNCTION(&handle2, "fopen", &_2, 30, &_0, &_1);
	zephir_check_call_status();
	if (zephir_is_true(handle)) {
		while (1) {
			ZEPHIR_SINIT_NVAR(_0);
			ZVAL_LONG(&_0, 4096);
			ZEPHIR_CALL_FUNCTION(&buffer, "fgets", &_3, 31, handle, &_0);
			zephir_check_call_status();
			if (ZEPHIR_IS_FALSE_IDENTICAL(buffer)) {
				break;
			}
			zephir_fwrite(NULL, handle2, buffer TSRMLS_CC);
		}
		zephir_fclose(handle TSRMLS_CC);
		zephir_fclose(handle2 TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, testCall4) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL, *_3 = NULL;
	zval *handle = NULL, *handle2 = NULL, *buffer = NULL, _0 = zval_used_for_init, _1 = zval_used_for_init;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "r", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "inputfile.txt", 0);
	ZEPHIR_CALL_FUNCTION(&handle, "fopen", &_2, 30, &_0, &_1);
	zephir_check_call_status();
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_STRING(&_0, "outputfile.txt", 0);
	ZEPHIR_SINIT_NVAR(_1);
	ZVAL_STRING(&_1, "w", 0);
	ZEPHIR_CALL_FUNCTION(&handle2, "fopen", &_2, 30, &_0, &_1);
	zephir_check_call_status();
	if (zephir_is_true(handle)) {
		while (1) {
			ZEPHIR_SINIT_NVAR(_0);
			ZVAL_LONG(&_0, 4096);
			ZEPHIR_CALL_FUNCTION(&buffer, "fgets", &_3, 31, handle, &_0);
			zephir_check_call_status();
			if (ZEPHIR_IS_FALSE_IDENTICAL(buffer)) {
				break;
			}
			zephir_fwrite(NULL, handle2, buffer TSRMLS_CC);
		}
		zephir_fclose(handle TSRMLS_CC);
		zephir_fclose(handle2 TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, testCall5) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", NULL, 21, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testCall6) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_FUNCTION("rand", NULL, 32);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testCall7) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_FUNCTION(NULL, "memory_get_usage", NULL, 33);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, zvalFcallWith1Parameter) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *callback, *param1 = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &callback, &param1);

	if (!param1) {
		param1 = ZEPHIR_GLOBAL(global_null);
	}


	ZEPHIR_CALL_ZVAL_FUNCTION(NULL, callback, NULL, 0, param1);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, testCall8) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, *b, *x = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_CALL_FUNCTION(&x, "str_repeat", NULL, 21, a, b);
	zephir_check_call_status();
	RETURN_CCTOR(x);

}

PHP_METHOD(Test_Fcall, testCall1FromVar) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *funcName, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(funcName);
	ZVAL_STRING(funcName, "strpos", 1);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "hello", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "l", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(funcName, NULL, 0, _0, _1);
	zephir_check_temp_parameter(_0);
	zephir_check_temp_parameter(_1);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testStrtokFalse) {


	RETURN_BOOL(0);

}

PHP_METHOD(Test_Fcall, testStrtokVarBySlash) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *value, _0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value);



	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "/", 0);
	ZEPHIR_RETURN_CALL_FUNCTION("strtok", NULL, 34, value, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testFunctionGetArgs) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *param1, *param2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &param1, &param2);



	ZEPHIR_RETURN_CALL_FUNCTION("func_get_args", NULL, 35);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testArrayFill) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_3 = NULL;
	zval *v1 = NULL, *v2 = NULL, *_0 = NULL, *_1 = NULL, *_2 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 5);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "?", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_FUNCTION(&v1, "array_fill", &_3, 36, _0, _1, _2);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 0);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 6);
	ZEPHIR_INIT_NVAR(_2);
	ZVAL_STRING(_2, "?", ZEPHIR_TEMP_PARAM_COPY);
	ZEPHIR_CALL_FUNCTION(&v2, "array_fill", &_3, 36, _0, _1, _2);
	zephir_check_temp_parameter(_2);
	zephir_check_call_status();
	zephir_create_array(return_value, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(return_value, v1);
	zephir_array_fast_append(return_value, v2);
	RETURN_MM();

}

PHP_FUNCTION(g_test_zephir_global_method_test) {
	int ZEPHIR_LAST_CALL_STATUS;
	zval *str, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str);



	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, test_fcall_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZEPHIR_RETURN_CALL_METHOD(_0, "teststrtokvarbyslash", NULL, 37, str);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_FUNCTION(f_Test_zephir_namespaced_method_test) {
	int ZEPHIR_LAST_CALL_STATUS;
	zval *str, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str);



	ZEPHIR_INIT_VAR(_0);
	object_init_ex(_0, test_fcall_ce);
	if (zephir_has_constructor(_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, _0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 5);
	ZEPHIR_RETURN_CALL_METHOD(_0, "testcall5", NULL, 38, str, _1);
	zephir_check_call_status();
	RETURN_MM();

}

