
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/object.h"


/**
 * Cast tests
 */
ZEPHIR_INIT_CLASS(Stub_TypeInstances)
{
	ZEPHIR_REGISTER_CLASS(Stub, TypeInstances, stub, typeinstances, stub_typeinstances_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_TypeInstances, testInstanceOfString1)
{
	zval _2;
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 24);
	ZEPHIR_CALL_FUNCTION(&_1, "create_string", NULL, 0, &_0);
	zephir_check_call_status();
	zephir_cast_to_string(&_2, &_1);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_TypeInstances, testInstanceOfString2)
{
	zval _2;
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, -24);
	ZEPHIR_CALL_FUNCTION(&_1, "create_string", NULL, 0, &_0);
	zephir_check_call_status();
	zephir_cast_to_string(&_2, &_1);
	RETURN_CTOR(&_2);
}

PHP_METHOD(Stub_TypeInstances, testInstanceOfString3)
{
	zval _2;
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZEPHIR_CALL_FUNCTION(&_1, "create_string", NULL, 0, &_0);
	zephir_check_call_status();
	zephir_cast_to_string(&_2, &_1);
	RETURN_CTOR(&_2);
}

