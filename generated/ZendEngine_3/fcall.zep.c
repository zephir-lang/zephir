
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
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "h");
	zephir_fast_strpos(return_value, &_0, &_1, 0 );
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testCall2) {

	zval _0$$3, _1$$3;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);


	while (1) {
		ZVAL_LONG(&_0$$3, 0);
		ZVAL_LONG(&_1$$3, 100);
		RETURN_LONG(zephir_mt_rand(zephir_get_intval(&_0$$3), zephir_get_intval(&_1$$3) TSRMLS_CC));
	}

}

PHP_METHOD(Test_Fcall, testCall3) {

	zval handle, handle2, buffer, _0, _1, _3$$4;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL, *_4 = NULL;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&handle);
	ZVAL_UNDEF(&handle2);
	ZVAL_UNDEF(&buffer);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3$$4);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "inputfile.txt");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "r");
	ZEPHIR_CALL_FUNCTION(&handle, "fopen", &_2, 26, &_0, &_1);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "outputfile.txt");
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "w");
	ZEPHIR_CALL_FUNCTION(&handle2, "fopen", &_2, 26, &_0, &_1);
	zephir_check_call_status();
	if (zephir_is_true(&handle)) {
		while (1) {
			ZVAL_LONG(&_3$$4, 4096);
			ZEPHIR_CALL_FUNCTION(&buffer, "fgets", &_4, 27, &handle, &_3$$4);
			zephir_check_call_status();
			if (ZEPHIR_IS_FALSE_IDENTICAL(&buffer)) {
				break;
			}
			zephir_fwrite(NULL, &handle2, &buffer TSRMLS_CC);
		}
		zephir_fclose(&handle TSRMLS_CC);
		zephir_fclose(&handle2 TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, testCall4) {

	zval handle, handle2, buffer, _0, _1, _3$$4;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL, *_4 = NULL;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&handle);
	ZVAL_UNDEF(&handle2);
	ZVAL_UNDEF(&buffer);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3$$4);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "r");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "inputfile.txt");
	ZEPHIR_CALL_FUNCTION(&handle, "fopen", &_2, 26, &_0, &_1);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "outputfile.txt");
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "w");
	ZEPHIR_CALL_FUNCTION(&handle2, "fopen", &_2, 26, &_0, &_1);
	zephir_check_call_status();
	if (zephir_is_true(&handle)) {
		while (1) {
			ZVAL_LONG(&_3$$4, 4096);
			ZEPHIR_CALL_FUNCTION(&buffer, "fgets", &_4, 27, &handle, &_3$$4);
			zephir_check_call_status();
			if (ZEPHIR_IS_FALSE_IDENTICAL(&buffer)) {
				break;
			}
			zephir_fwrite(NULL, &handle2, &buffer TSRMLS_CC);
		}
		zephir_fclose(&handle TSRMLS_CC);
		zephir_fclose(&handle2 TSRMLS_CC);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, testCall5) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", NULL, 17, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testCall6) {

	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_FUNCTION("rand", NULL, 28);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testCall7) {

	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();


	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_FUNCTION(NULL, "memory_get_usage", NULL, 29);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, zvalFcallWith1Parameter) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *callback, callback_sub, *param1 = NULL, param1_sub, __$null;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&callback_sub);
	ZVAL_UNDEF(&param1_sub);
	ZVAL_NULL(&__$null);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &callback, &param1);

	if (!param1) {
		param1 = &param1_sub;
		param1 = &__$null;
	}


	ZEPHIR_CALL_ZVAL_FUNCTION(NULL, callback, NULL, 0, param1);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Fcall, testCall8) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, x;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&x);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_CALL_FUNCTION(&x, "str_repeat", NULL, 17, a, b);
	zephir_check_call_status();
	RETURN_CCTOR(x);

}

PHP_METHOD(Test_Fcall, testCall1FromVar) {

	zval funcName, _0, _1;
	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&funcName);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&funcName);
	ZVAL_STRING(&funcName, "strpos");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "l");
	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(&funcName, NULL, 0, &_0, &_1);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testStrtokFalse) {

	ZEPHIR_INIT_THIS();


	RETURN_BOOL(0);

}

PHP_METHOD(Test_Fcall, testStrtokVarBySlash) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *value, value_sub, _0;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value);



	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "/");
	ZEPHIR_RETURN_CALL_FUNCTION("strtok", NULL, 30, value, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testFunctionGetArgs) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *param1, param1_sub, *param2, param2_sub;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&param1_sub);
	ZVAL_UNDEF(&param2_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &param1, &param2);



	ZEPHIR_RETURN_CALL_FUNCTION("func_get_args", NULL, 31);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Fcall, testArrayFill) {

	zval v1, v2, _0, _1, _2;
	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_3 = NULL;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&v1);
	ZVAL_UNDEF(&v2);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 5);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "?");
	ZEPHIR_CALL_FUNCTION(&v1, "array_fill", &_3, 32, &_0, &_1, &_2);
	zephir_check_call_status();
	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 6);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "?");
	ZEPHIR_CALL_FUNCTION(&v2, "array_fill", &_3, 32, &_0, &_1, &_2);
	zephir_check_call_status();
	zephir_create_array(return_value, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(return_value, &v1);
	zephir_array_fast_append(return_value, &v2);
	RETURN_MM();

}

PHP_FUNCTION(g_test_zephir_global_method_test) {
	int ZEPHIR_LAST_CALL_STATUS;
	zval *str, str_sub, _0;
		ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str);



	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, test_fcall_ce);
	if (zephir_has_constructor(&_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZEPHIR_RETURN_CALL_METHOD(&_0, "teststrtokvarbyslash", NULL, 33, str);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_FUNCTION(f_Test_zephir_namespaced_method_test) {
	int ZEPHIR_LAST_CALL_STATUS;
	zval *str, str_sub, _0, _1;
		ZVAL_UNDEF(&str_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &str);



	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, test_fcall_ce);
	if (zephir_has_constructor(&_0 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &_0, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZVAL_LONG(&_1, 5);
	ZEPHIR_RETURN_CALL_METHOD(&_0, "testcall5", NULL, 34, str, &_1);
	zephir_check_call_status();
	RETURN_MM();

}

