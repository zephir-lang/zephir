
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
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_ACos)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, ACos, stub, optimizers_acos, stub_optimizers_acos_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Optimizers_ACos, testInt)
{
	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, a;

	ZVAL_UNDEF(&_0);
	a = 1;
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 15, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Optimizers_ACos, testVar)
{
	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, a;

	ZVAL_UNDEF(&_0);
	a = 1;
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 15, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Optimizers_ACos, testIntValue1)
{
	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 15, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Optimizers_ACos, testIntParameter)
{
	zval *a_param = NULL, _0;
	zend_long a, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &a_param);
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 15, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Optimizers_ACos, testVarParameter)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_RETURN_CALL_FUNCTION("acos", NULL, 15, a);
	zephir_check_call_status();
	return;
}

