
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
#include "kernel/object.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_ResourceTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, ResourceTest, stub, resourcetest, stub_resourcetest_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_ResourceTest, testLetStatementSTDIN)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZEPHIR_GET_CONSTANT(&a, "STDIN");
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_ResourceTest, testLetStatementSTDOUT)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZEPHIR_GET_CONSTANT(&a, "STDOUT");
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_ResourceTest, testLetStatementSTDERR)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZEPHIR_GET_CONSTANT(&a, "STDERR");
	RETURN_CCTOR(&a);
}

PHP_METHOD(Stub_ResourceTest, testTypeOffResource)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZEPHIR_GET_CONSTANT(&a, "STDIN");
	ZEPHIR_INIT_VAR(&_0);
	zephir_gettype(&_0, &a);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_ResourceTest, testIsResource)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZEPHIR_GET_CONSTANT(&a, "STDIN");
	RETURN_MM_BOOL(Z_TYPE_P(&a) == IS_RESOURCE);
}

PHP_METHOD(Stub_ResourceTest, testFunctionsForSTDIN)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZEPHIR_GET_CONSTANT(&a, "STDIN");
	ZVAL_LONG(&_0, 1);
	ZEPHIR_CALL_FUNCTION(NULL, "stream_set_blocking", NULL, 86, &a, &_0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

