
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_FunctionExists)
{
	ZEPHIR_REGISTER_CLASS(Stub, FunctionExists, stub, functionexists, stub_functionexists_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_FunctionExists, testWithPassedName)
{
	zval func_zv;
	zend_string *func = NULL;

	ZVAL_UNDEF(&func_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(func)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&func_zv, func);
	RETURN_BOOL((zephir_function_exists(&func_zv) == SUCCESS));
}

PHP_METHOD(Stub_FunctionExists, testBuiltInFunctions)
{
	zval result, functions;
	zval func, _0, *_1, _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&func);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&functions);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&functions);
	zephir_create_array(&functions, 8, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "substr");
	zephir_array_fast_append(&functions, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "cubstr");
	zephir_array_fast_append(&functions, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "ucfirst");
	zephir_array_fast_append(&functions, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "bcfirst");
	zephir_array_fast_append(&functions, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "stripos");
	zephir_array_fast_append(&functions, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "ktripos");
	zephir_array_fast_append(&functions, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "trim");
	zephir_array_fast_append(&functions, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "prim");
	zephir_array_fast_append(&functions, &_0);
	zephir_is_iterable(&functions, 0, "stub/functionexists.zep", 31);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&functions), _1)
	{
		ZEPHIR_INIT_NVAR(&func);
		ZVAL_COPY(&func, _1);
		ZEPHIR_INIT_NVAR(&_2$$3);
		ZVAL_BOOL(&_2$$3, (zephir_function_exists(&func) == SUCCESS));
		zephir_array_update_zval(&result, &func, &_2$$3, PH_COPY | PH_SEPARATE);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&func);
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_FunctionExists, testWithString)
{

	RETURN_BOOL((zephir_function_exists_ex(ZEND_STRL("substr")) == SUCCESS));
}

