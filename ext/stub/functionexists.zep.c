
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
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_FunctionExists)
{
	ZEPHIR_REGISTER_CLASS(Stub, FunctionExists, stub, functionexists, stub_functionexists_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_FunctionExists, testWithPassedName)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *func_param = NULL;
	zval func;

	ZVAL_UNDEF(&func);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(func)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &func_param);
	zephir_get_strval(&func, func_param);
	RETURN_MM_BOOL((zephir_function_exists(&func) == SUCCESS));
}

PHP_METHOD(Stub_FunctionExists, testBuiltInFunctions)
{
	zval result, functions;
	zval func, _0, *_1, _2, _3$$3, _4$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&func);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$4);
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
	if (Z_TYPE_P(&functions) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&functions), _1)
		{
			ZEPHIR_INIT_NVAR(&func);
			ZVAL_COPY(&func, _1);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_BOOL(&_3$$3, (zephir_function_exists(&func) == SUCCESS));
			zephir_array_update_zval(&result, &func, &_3$$3, PH_COPY | PH_SEPARATE);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &functions, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_2, &functions, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_2)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&func, &functions, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_INIT_NVAR(&_4$$4);
				ZVAL_BOOL(&_4$$4, (zephir_function_exists(&func) == SUCCESS));
				zephir_array_update_zval(&result, &func, &_4$$4, PH_COPY | PH_SEPARATE);
			ZEPHIR_CALL_METHOD(NULL, &functions, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&func);
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_FunctionExists, testWithString)
{

	RETURN_BOOL((zephir_function_exists_ex(ZEND_STRL("substr")) == SUCCESS));
}

