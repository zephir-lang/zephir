
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
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Functions)
{
	ZEPHIR_REGISTER_CLASS(Stub, Functions, stub, functions, stub_functions_method_entry, 0);

	return SUCCESS;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/658
 */
PHP_METHOD(Stub_Functions, filterVar1)
{
	zval ret, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&ret);
	ZVAL_STRING(&ret, "0");
	ZVAL_LONG(&_0, 259);
	ZVAL_LONG(&_1, 20480);
	ZEPHIR_CALL_FUNCTION(&_2, "filter_var", NULL, 44, &ret, &_0, &_1);
	zephir_check_call_status();
	RETURN_MM_BOOL(ZEPHIR_IS_FALSE_IDENTICAL(&_2));
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/658
 */
PHP_METHOD(Stub_Functions, filterVar2)
{
	zval ret, _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&ret);
	ZVAL_STRING(&ret, "0");
	ZVAL_LONG(&_0, 259);
	ZVAL_LONG(&_1, 20480);
	ZEPHIR_CALL_FUNCTION(&_2, "filter_var", NULL, 44, &ret, &_0, &_1);
	zephir_check_call_status();
	RETURN_MM_BOOL(ZEPHIR_IS_FALSE(&_2));
}

