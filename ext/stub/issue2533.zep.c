
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Issue2533)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2533, stub, issue2533, stub_issue2533_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 4);
		add_next_index_long(&_zc0, 1);
		add_next_index_long(&_zc0, 2);
		add_next_index_long(&_zc0, 3);
		zephir_declare_class_constant_array(stub_issue2533_ce, SL("BAR"), &_zc0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_long_ex(&_zc0, "a", 1, 1);
		add_assoc_long_ex(&_zc0, "b", 1, 2);
		zephir_declare_class_constant_array(stub_issue2533_ce, SL("MAP"), &_zc0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		zval _zc1;
		array_init_size(&_zc1, 3);
		add_next_index_long(&_zc1, 1);
		add_next_index_long(&_zc1, 2);
		add_next_index_zval(&_zc0, &_zc1);
		zval _zc2;
		array_init_size(&_zc2, 2);
		add_assoc_stringl_ex(&_zc2, "k", 1, "v", 1);
		add_next_index_zval(&_zc0, &_zc2);
		zephir_declare_class_constant_array(stub_issue2533_ce, SL("NESTED"), &_zc0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_class_constant_array(stub_issue2533_ce, SL("EMPTY_ARR"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2533, bar)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_create_array(return_value, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2533, map)
{

	zephir_create_array(return_value, 2, 0);
	add_assoc_long_ex(return_value, SL("a"), 1);
	add_assoc_long_ex(return_value, SL("b"), 2);
	return;
}

PHP_METHOD(Stub_Issue2533, nested)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0);
	add_assoc_stringl_ex(&_0, SL("k"), SL("v"));
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

