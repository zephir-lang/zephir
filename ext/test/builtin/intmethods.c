
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

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "abs", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAbs1) {

	zval _0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, -5);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "abs", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getBinary) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "decbin", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getHex) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "dechex", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getOctal) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "decoct", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getPow) {

	zval *num_param = NULL, *exp_param = NULL, _0, _1, *_2;
	int num, exp;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &num_param, &exp_param);

	num = zephir_get_intval(num_param);
	exp = zephir_get_intval(exp_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_LONG(&_1, exp);
	ZEPHIR_INIT_VAR(_2);
	zephir_call_func_p2(_2, "pow", &_0, &_1);
	RETURN_CCTOR(_2);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getSqrt) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "sqrt", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getExp) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "exp", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getSin) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "sin", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getCos) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "cos", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getTan) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "tan", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAsin) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "asin", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAcos) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "acos", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getAtan) {

	zval *num_param = NULL, _0, *_1;
	int num;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_func_p1(_1, "atan", &_0);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_IntMethods, getLog) {

	zval *num_param = NULL, *base_param = NULL, _0 = zval_used_for_init, *_1 = NULL, _2;
	int num, base;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 1, &num_param, &base_param);

	num = zephir_get_intval(num_param);
	if (!base_param) {
		base = -1;
	} else {
		base = zephir_get_intval(base_param);
	}


	if ((base == -1)) {
		ZEPHIR_SINIT_VAR(_0);
		ZVAL_LONG(&_0, num);
		ZEPHIR_INIT_VAR(_1);
		zephir_call_func_p1(_1, "log", &_0);
		RETURN_CCTOR(_1);
	}
	ZEPHIR_SINIT_NVAR(_0);
	ZVAL_LONG(&_0, num);
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_LONG(&_2, base);
	ZEPHIR_INIT_NVAR(_1);
	zephir_call_func_p2(_1, "log", &_0, &_2);
	RETURN_CCTOR(_1);

}

