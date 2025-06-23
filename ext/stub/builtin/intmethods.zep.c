
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
#include "kernel/object.h"
#include "math.h"


ZEPHIR_INIT_CLASS(Stub_BuiltIn_IntMethods)
{
	ZEPHIR_REGISTER_CLASS(Stub\\BuiltIn, IntMethods, stub, builtin_intmethods, stub_builtin_intmethods_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getAbs)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "abs", NULL, 9, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getAbs1)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, -5);
	ZEPHIR_CALL_FUNCTION(&_1, "abs", NULL, 9, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getBinary)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "decbin", NULL, 10, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getHex)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "dechex", NULL, 11, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getOctal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "decoct", NULL, 12, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getPow)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, *exp_param = NULL, _0, _1, _2;
	zend_long num, exp, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(num)
		Z_PARAM_LONG(exp)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &num_param, &exp_param);
	ZVAL_LONG(&_0, num);
	ZVAL_LONG(&_1, exp);
	ZEPHIR_CALL_FUNCTION(&_2, "pow", NULL, 13, &_0, &_1);
	zephir_check_call_status();
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getSqrt)
{
	zval *num_param = NULL, _0;
	zend_long num;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	RETURN_DOUBLE(sqrt(num));
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getExp)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "exp", NULL, 2, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getSin)
{
	zval *num_param = NULL, _0;
	zend_long num;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	RETURN_DOUBLE(sin(num));
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getCos)
{
	zval *num_param = NULL, _0;
	zend_long num;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	RETURN_DOUBLE(cos(num));
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getTan)
{
	zval *num_param = NULL, _0;
	zend_long num;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	RETURN_DOUBLE(tan(num));
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getAsin)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "asin", NULL, 14, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getAcos)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "acos", NULL, 15, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getAtan)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _1;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZVAL_LONG(&_0, num);
	ZEPHIR_CALL_FUNCTION(&_1, "atan", NULL, 16, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_IntMethods, getLog)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, *base_param = NULL, _0$$3, _1$$3, _2, _3, _4;
	zend_long num, base, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_LONG(num)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(base)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 1, &num_param, &base_param);
	if (!base_param) {
		base = -1;
	} else {
		}
	if (base == -1) {
		ZVAL_LONG(&_0$$3, num);
		ZEPHIR_CALL_FUNCTION(&_1$$3, "log", NULL, 17, &_0$$3);
		zephir_check_call_status();
		RETURN_CCTOR(&_1$$3);
	}
	ZVAL_LONG(&_2, num);
	ZVAL_LONG(&_3, base);
	ZEPHIR_CALL_FUNCTION(&_4, "log", NULL, 17, &_2, &_3);
	zephir_check_call_status();
	RETURN_CCTOR(&_4);
}

