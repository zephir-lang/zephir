
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_BuiltIn_IntMethods) {

	ZEPHIR_REGISTER_CLASS(Test\\BuiltIn, IntMethods, test, builtin_intmethods, test_builtin_intmethods_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAbs) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "abs", &_2, 5, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAbs1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;
	zval _0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, -5);
	ZEPHIR_CALL_FUNCTION(&_1, "abs", &_2, 5, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getBinary) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "decbin", &_2, 6, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getHex) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "dechex", &_2, 7, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getOctal) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "decoct", &_2, 8, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getPow) {

	zval *num_param = NULL, *exp_param = NULL, *_0, _1, _2;
	int num, exp;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &num_param, &exp_param);

	num = zephir_get_intval(num_param);
	exp = zephir_get_intval(exp_param);


	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_LONG(&_1, num);
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_LONG(&_2, exp);
	zephir_pow_function(_0, &_1, &_2);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getSqrt) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "sqrt", &_2, 9, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getExp) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", &_2, 1, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getSin) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "sin", &_2, 10, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getCos) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "cos", &_2, 11, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getTan) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "tan", &_2, 12, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAsin) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "asin", &_2, 13, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAcos) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "acos", &_2, 14, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAtan) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, _0, *_1 = NULL;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "atan", &_2, 15, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getLog) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *num_param = NULL, *base_param = NULL, _0 = zval_used_for_init, *_1 = NULL, _3;
	int num, base, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &num_param, &base_param);

	num = zephir_get_intval(num_param);
	if (!base_param) {
		base = -1;
	} else {
		base = zephir_get_intval(base_param);
	}


	if (base == -1) {
		ZEPHIR_SINIT_VAR(_0);
		ZVAL_LONG(&_0, num);
		ZEPHIR_CALL_FUNCTION(&_1, "log", &_2, 16, &_0);
		zephir_check_call_status();
		RETURN_CCTOR(_1);
	}
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_SINIT_VAR(_3);
	ZVAL_LONG(&_3, base);
	ZEPHIR_CALL_FUNCTION(&_1, "log", &_2, 16, &_0, &_3);
	zephir_check_call_status();
	RETURN_CCTOR(_1);

}

