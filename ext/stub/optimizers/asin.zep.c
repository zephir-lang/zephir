
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
#include "kernel/memory.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Optimizers_ASin)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Optimizers, ASin, stub, optimizers_asin, stub_optimizers_asin_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Optimizers_ASin, testInt)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	a = 1;
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("asin", NULL, 13, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Optimizers_ASin, testVar)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	a = 1;
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("asin", NULL, 13, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Optimizers_ASin, testIntValue1)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZVAL_LONG(&_0, 0);
	ZEPHIR_RETURN_CALL_FUNCTION("asin", NULL, 13, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Optimizers_ASin, testIntParameter)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zend_long a, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);
	a = zephir_get_intval(a_param);


	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("asin", NULL, 13, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Optimizers_ASin, testVarParameter)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a);


	ZEPHIR_RETURN_CALL_FUNCTION("asin", NULL, 13, a);
	zephir_check_call_status();
	RETURN_MM();
}

