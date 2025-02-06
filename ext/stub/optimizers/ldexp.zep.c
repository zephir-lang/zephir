
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
#include "kernel/math.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_Ldexp)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, Ldexp, stub, optimizers_ldexp, stub_optimizers_ldexp_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Optimizers_Ldexp, testInt)
{
	zval _0, _1;
	zend_long x, exponent;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	x = 2;
	exponent = 3;
	ZVAL_LONG(&_0, x);
	ZVAL_LONG(&_1, exponent);
	RETURN_DOUBLE(zephir_ldexp(&_0, &_1));
}

PHP_METHOD(Stub_Optimizers_Ldexp, testDoubleInt)
{
	zval _0, _1;
	zend_long exponent;
	double x;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	x =  (2.0);
	exponent = 3;
	ZVAL_DOUBLE(&_0, x);
	ZVAL_LONG(&_1, exponent);
	RETURN_DOUBLE(zephir_ldexp(&_0, &_1));
}

PHP_METHOD(Stub_Optimizers_Ldexp, testDouble)
{
	zval _0, _1;
	double x, exponent;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	x =  (2.0);
	exponent =  (3.0);
	ZVAL_DOUBLE(&_0, x);
	ZVAL_DOUBLE(&_1, exponent);
	RETURN_DOUBLE(zephir_ldexp(&_0, &_1));
}

PHP_METHOD(Stub_Optimizers_Ldexp, testVar)
{
	zval _0, _1;
	zend_long x, exponent;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	x = 2;
	exponent = 3;
	ZVAL_LONG(&_0, x);
	ZVAL_LONG(&_1, exponent);
	RETURN_DOUBLE(zephir_ldexp(&_0, &_1));
}

PHP_METHOD(Stub_Optimizers_Ldexp, testIntValue1)
{
	zval _0, _1;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_LONG(&_0, 2);
	ZVAL_LONG(&_1, 3);
	RETURN_DOUBLE(zephir_ldexp(&_0, &_1));
}

PHP_METHOD(Stub_Optimizers_Ldexp, testIntParameter)
{
	zval *x_param = NULL, *exponent_param = NULL, _0, _1;
	zend_long x, exponent;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(x)
		Z_PARAM_LONG(exponent)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &x_param, &exponent_param);
	ZVAL_LONG(&_0, x);
	ZVAL_LONG(&_1, exponent);
	RETURN_DOUBLE(zephir_ldexp(&_0, &_1));
}

PHP_METHOD(Stub_Optimizers_Ldexp, testVarParameter)
{
	zval *x, x_sub, *exponent, exponent_sub;

	ZVAL_UNDEF(&x_sub);
	ZVAL_UNDEF(&exponent_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(x)
		Z_PARAM_ZVAL(exponent)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &x, &exponent);
	RETURN_DOUBLE(zephir_ldexp(x, exponent));
}

