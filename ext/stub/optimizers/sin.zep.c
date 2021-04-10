
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


ZEPHIR_INIT_CLASS(Stub_Optimizers_Sin)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, Sin, stub, optimizers_sin, stub_optimizers_sin_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Optimizers_Sin, testInt)
{
	zval _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	a = 4;
	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(sin(a));
}

PHP_METHOD(Stub_Optimizers_Sin, testVar)
{
	zval _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	a = 4;
	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(sin(a));
}

PHP_METHOD(Stub_Optimizers_Sin, testIntValue1)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	ZVAL_LONG(&_0, 4);
	RETURN_DOUBLE(sin(4));
}

PHP_METHOD(Stub_Optimizers_Sin, testIntValue2)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);



	ZVAL_LONG(&_0, 16);
	RETURN_DOUBLE(sin(16));
}

PHP_METHOD(Stub_Optimizers_Sin, testIntParameter)
{
	zval *a_param = NULL, _0;
	zend_long a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	a = zephir_get_intval(a_param);


	ZVAL_LONG(&_0, a);
	RETURN_DOUBLE(sin(a));
}

PHP_METHOD(Stub_Optimizers_Sin, testVarParameter)
{
	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &a);


	RETURN_DOUBLE(zephir_sin(a));
}

