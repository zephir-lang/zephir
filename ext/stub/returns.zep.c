
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Returns)
{
	ZEPHIR_REGISTER_CLASS(Stub, Returns, stub, returns, stub_returns_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Returns, testReturnCast1)
{

	RETURN_LONG((int) 5.0);
}

PHP_METHOD(Stub_Returns, testReturnCast2)
{

	RETURN_LONG(1);
}

PHP_METHOD(Stub_Returns, testReturnCast3)
{

	RETURN_LONG(0);
}

PHP_METHOD(Stub_Returns, testReturnCast4)
{

	if (1) {
		RETURN_LONG(1);
	} else {
		RETURN_LONG(0);
	}
}

PHP_METHOD(Stub_Returns, returnWithParameter)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *parameter_param = NULL;
	zval parameter;

	ZVAL_UNDEF(&parameter);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(parameter)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &parameter_param);
	zephir_get_strval(&parameter, parameter_param);
	RETURN_MM_STRING("Return back");
}

PHP_METHOD(Stub_Returns, returnWithoutParameter)
{

	RETURN_STRING("Return back");
}

