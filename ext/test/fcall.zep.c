
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/file.h"


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

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL;
	zval _0 = zval_used_for_init, _1 = zval_used_for_init;

	ZEPHIR_MM_GROW();

	while (1) {
		ZEPHIR_SINIT_NVAR(_0);
		ZVAL_LONG(&_0, 0);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_LONG(&_1, 100);
		ZEPHIR_RETURN_CALL_FUNCTION("mt_rand", &_2, &_0, &_1);
		zephir_check_call_status();
		RETURN_MM();
	}

}

PHP_METHOD(Test_Fcall, testCall3) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_3 = NULL;
	zval *handle = NULL, *handle2 = NULL, *buffer = NULL, _0 = zval_used_for_init, _1 = zval_used_for_init;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "inputfile.txt", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "r", 0);
	ZEPHIR_CALL_FUNCTION(&handle, "fopen", &_2, &_0, &_1);
	zephir_check_call_status();
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_STRING(&_0, "outputfile.txt", 0);
	ZEPHIR_SINIT_NVAR(_1);
	ZVAL_STRING(&_1, "w", 0);
	ZEPHIR_CALL_FUNCTION(&handle2, "fopen", &_2, &_0, &_1);
	zephir_check_call_status();
	if (zephir_is_true(handle)) {
		while (1) {
			ZEPHIR_SINIT_NVAR(_0);
			ZVAL_LONG(&_0, 4096);
			ZEPHIR_CALL_FUNCTION(&buffer, "fgets", &_3, handle, &_0);
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
	zephir_nts_static zephir_fcall_cache_entry *_2 = NULL, *_3 = NULL;
	zval *handle = NULL, *handle2 = NULL, *buffer = NULL, _0 = zval_used_for_init, _1 = zval_used_for_init;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "r", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "inputfile.txt", 0);
	ZEPHIR_CALL_FUNCTION(&handle, "fopen", &_2, &_0, &_1);
	zephir_check_call_status();
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_STRING(&_0, "outputfile.txt", 0);
	ZEPHIR_SINIT_NVAR(_1);
	ZVAL_STRING(&_1, "w", 0);
	ZEPHIR_CALL_FUNCTION(&handle2, "fopen", &_2, &_0, &_1);
	zephir_check_call_status();
	if (zephir_is_true(handle)) {
		while (1) {
			ZEPHIR_SINIT_NVAR(_0);
			ZVAL_LONG(&_0, 4096);
			ZEPHIR_CALL_FUNCTION(&buffer, "fgets", &_3, handle, &_0);
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
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testCall6) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_FUNCTION("rand", &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testCall7) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_FUNCTION(NULL, "memory_get_usage", &_0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, testCall8) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *a, *b, *x = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_CALL_FUNCTION(&x, "str_repeat", &_0, a, b);
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
	ZVAL_STRING(_0, "hello", 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "l", 0);
	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(funcName, NULL, _0, _1);
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
	zephir_nts_static zephir_fcall_cache_entry *_1 = NULL;
	zval *value, _0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value);



	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "/", 0);
	ZEPHIR_RETURN_CALL_FUNCTION("strtok", &_1, value, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testFunctionGetArgs) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zephir_fcall_cache_entry *_0 = NULL;
	zval *param1, *param2;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &param1, &param2);



	ZEPHIR_RETURN_CALL_FUNCTION("func_get_args", &_0);
	zephir_check_call_status();
	RETURN_MM();

}

