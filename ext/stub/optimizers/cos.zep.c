
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
#include "math.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/math.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_Cos)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, Cos, stub, optimizers_cos, stub_optimizers_cos_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Optimizers_Cos, testInt)
{
	zval _0;
	zend_long a;

	ZVAL_UNDEF(&_0);
	a = 4;
	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(cos(a));
}

PHP_METHOD(Stub_Optimizers_Cos, testVar)
{
	zval _0;
	zend_long a;

	ZVAL_UNDEF(&_0);
	a = 4;
	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(cos(a));
}

PHP_METHOD(Stub_Optimizers_Cos, testIntValue1)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 4);
	RETURN_DOUBLE(cos(4));
}

PHP_METHOD(Stub_Optimizers_Cos, testIntValue2)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 16);
	RETURN_DOUBLE(cos(16));
}

PHP_METHOD(Stub_Optimizers_Cos, testIntParameter)
{
	zval *a_param = NULL, _0;
	zend_long a;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(cos(a));
}

PHP_METHOD(Stub_Optimizers_Cos, testVarParameter)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_DOUBLE(zephir_cos(a));
}

