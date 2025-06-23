
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


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Stub_Range)
{
	ZEPHIR_REGISTER_CLASS(Stub, Range, stub, range, stub_range_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Range, inclusive1)
{
	zval _3;
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 10);
	ZEPHIR_CALL_FUNCTION(&_2, "range", NULL, 83, &_0, &_1);
	zephir_check_call_status();
	zephir_get_arrval(&_3, &_2);
	RETURN_CTOR(&_3);
}

PHP_METHOD(Stub_Range, exclusive1)
{
	zval _3;
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 10);
	ZEPHIR_CALL_FUNCTION(&_2, "range", NULL, 83, &_0, &_1);
	zephir_check_call_status();
	zephir_get_arrval(&_3, &_2);
	RETURN_CTOR(&_3);
}

