
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


ZEPHIR_INIT_CLASS(Stub_Issue2324)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2324, stub, issue2324, stub_issue2324_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2324, __construct)
{

}

PHP_METHOD(Stub_Issue2324, makeStatic)
{
	zval obj;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_NULL(&obj);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	object_init_ex(&obj, zend_get_called_scope(execute_data));
	ZEPHIR_CALL_METHOD(NULL, &obj, "__construct", NULL, 1);
	zephir_check_call_status();
	zephir_get_class(return_value, &obj, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2324, makeSelf)
{
	zval obj;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_NULL(&obj);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	object_init_ex(&obj, stub_issue2324_ce);
	ZEPHIR_CALL_METHOD(NULL, &obj, "__construct", NULL, 1);
	zephir_check_call_status();
	zephir_get_class(return_value, &obj, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2324, makeStaticInstance)
{
	zval obj;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_NULL(&obj);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	object_init_ex(&obj, zend_get_called_scope(execute_data));
	ZEPHIR_CALL_METHOD(NULL, &obj, "__construct", NULL, 1);
	zephir_check_call_status();
	zephir_get_class(return_value, &obj, 0);
	RETURN_MM();
}

