
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
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Invoke)
{
	ZEPHIR_REGISTER_CLASS(Stub, Invoke, stub, invoke, stub_invoke_method_entry, 0);

	zend_declare_property_null(stub_invoke_ce, SL("a"), ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_Invoke, __construct)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "aaa");
	zephir_update_property_zval(this_ptr, ZEND_STRL("a"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Invoke, __invoke)
{
	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("a"), PH_NOISY_CC | PH_READONLY);
	zephir_gettype(return_value, &_0);
	return;
}

PHP_METHOD(Stub_Invoke, test)
{
	zval func;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&func);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&func);
	object_init_ex(&func, stub_invoke_ce);
	ZEPHIR_CALL_METHOD(NULL, &func, "__construct", NULL, 47);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(&func, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

