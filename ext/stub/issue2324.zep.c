
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
	zval obj, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, zephir_get_called_scope(execute_data));
	ZEPHIR_LAST_CALL_STATUS = zephir_check_constructor_access(&_0);
	zephir_check_call_status();
	zephir_constructor_start(&_0);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0]);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&obj, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_get_class(return_value, &obj, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2324, makeSelf)
{
	zval obj, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, stub_issue2324_ce);
	zephir_constructor_start(&_0);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0]);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&obj, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_get_class(return_value, &obj, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2324, makeStaticInstance)
{
	zval obj, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_0);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	object_init_ex(&_0, zephir_get_called_scope(execute_data));
	ZEPHIR_LAST_CALL_STATUS = zephir_check_constructor_access(&_0);
	zephir_check_call_status();
	zephir_constructor_start(&_0);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &_0, "__construct", &_zephir_call_cache[0]);
	zephir_check_call_status();
	zephir_constructor_finish(&_0);
	ZEPHIR_CPY_WRT(&obj, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_get_class(return_value, &obj, 0);
	RETURN_MM();
}

