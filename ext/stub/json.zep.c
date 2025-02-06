
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/object.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Json)
{
	ZEPHIR_REGISTER_CLASS(Stub, Json, stub, json, stub_json_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Json, testEncodeObject)
{
	zval obj;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&obj);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	zephir_create_array(&obj, 3, 0);
	add_assoc_stringl_ex(&obj, SL("a"), SL("hello"));
	add_assoc_stringl_ex(&obj, SL("b"), SL("world"));
	add_assoc_long_ex(&obj, SL("c"), 128);
	zephir_json_encode(return_value, &obj, 0 );
	RETURN_MM();
}

PHP_METHOD(Stub_Json, testEncodeArray)
{
	zval arr, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&arr, &_0);
	zephir_json_encode(return_value, &arr, 0 );
	RETURN_MM();
}

PHP_METHOD(Stub_Json, testEncodeOptions)
{
	zval arr, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 4, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "<foo>");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "'bar'");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "&blong&");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "\xc3\xa9");
	zephir_array_fast_append(&arr, &_0);
	ZVAL_LONG(&_1, 1);
	zephir_json_encode(return_value, &arr, zephir_get_intval(&_1) );
	RETURN_MM();
}

PHP_METHOD(Stub_Json, testDecodeObject)
{
	zval obj;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&obj);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	ZVAL_STRING(&obj, "{\"a\":\"hello\",\"b\":\"world\",\"c\":128}");
	zephir_json_decode(return_value, &obj, 0 );
	RETURN_MM();
}

PHP_METHOD(Stub_Json, testDecodeObject2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$true, obj;

	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&obj);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&obj);
	ZVAL_STRING(&obj, "{\"a\":\"hello\",\"b\":\"world\",\"c\":128}");
	zephir_json_decode(return_value, &obj, zephir_get_intval(&__$true) );
	RETURN_MM();
}

PHP_METHOD(Stub_Json, testDecodeArray)
{
	zval arr;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	ZVAL_STRING(&arr, "[1,2,3]");
	zephir_json_decode(return_value, &arr, 0 );
	RETURN_MM();
}

