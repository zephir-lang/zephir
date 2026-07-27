
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Types_Unions)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Types, Unions, stub, types_unions, stub_types_unions_method_entry, 0);

	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 1);
		zephir_declare_typed_property(stub_types_unions_ce, SL("num"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG|MAY_BE_DOUBLE, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_NULL(&_zc0);
		zephir_declare_typed_property(stub_types_unions_ce, SL("note"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG|MAY_BE_STRING|MAY_BE_NULL, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		const char *_zut[] = { "Stub\\Types\\MayBe" };
		zephir_declare_typed_property_union(stub_types_unions_ce, SL("tag"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_STRING, _zut, 1);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Types_Unions, kind)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	ZEPHIR_INIT_VAR(&_0);
	zephir_gettype(&_0, value);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Types_Unions, pick)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	ZEPHIR_INIT_VAR(&_0);
	zephir_gettype(&_0, value);
	RETURN_CCTOR(&_0);
}

