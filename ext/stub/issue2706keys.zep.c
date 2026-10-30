
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


/**
 * Array keys in declarations: class constants and property defaults. PHP
 * converts a float, bool or null key and a canonical numeric string key once,
 * when the declaration is evaluated, so these are folded at compile time.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2706
 */
ZEPHIR_INIT_CLASS(Stub_Issue2706Keys)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2706Keys, stub, issue2706keys, stub_issue2706keys_method_entry, 0);

	zend_declare_property_null(stub_issue2706keys_ce, SL("keys"), ZEND_ACC_PUBLIC);
	zend_declare_property_null(stub_issue2706keys_ce, SL("staticKeys"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	{
		zval _zc0;
		array_init_size(&_zc0, 9);
		add_index_stringl(&_zc0, 1, SL("a"));
		add_index_stringl(&_zc0, 1, SL("b"));
		add_index_stringl(&_zc0, 0, SL("c"));
		add_assoc_stringl_ex(&_zc0, SL(""), SL("d"));
		add_index_stringl(&_zc0, 2, SL("e"));
		add_index_stringl(&_zc0, 3, SL("f"));
		add_assoc_stringl_ex(&_zc0, SL("03"), SL("g"));
		add_index_stringl(&_zc0, -1, SL("h"));
		zephir_declare_typed_property(stub_issue2706keys_ce, SL("typedKeys"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	stub_issue2706keys_ce->create_object = zephir_init_properties_Stub_Issue2706Keys;
	{
		zval _zc0;
		array_init_size(&_zc0, 9);
		add_index_stringl(&_zc0, 1, SL("a"));
		add_index_stringl(&_zc0, 1, SL("b"));
		add_index_stringl(&_zc0, 0, SL("c"));
		add_assoc_stringl_ex(&_zc0, SL(""), SL("d"));
		add_index_stringl(&_zc0, 2, SL("e"));
		add_index_stringl(&_zc0, 3, SL("f"));
		add_assoc_stringl_ex(&_zc0, SL("03"), SL("g"));
		add_index_stringl(&_zc0, -1, SL("h"));
		zephir_declare_class_constant_array(stub_issue2706keys_ce, SL("KEYS"), &_zc0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		zval _zc1;
		array_init_size(&_zc1, 4);
		add_index_stringl(&_zc1, 1, SL("a"));
		add_index_stringl(&_zc1, 1, SL("b"));
		add_index_stringl(&_zc1, 4, SL("c"));
		add_assoc_zval_ex(&_zc0, SL("x"), &_zc1);
		zephir_declare_class_constant_array(stub_issue2706keys_ce, SL("NESTED"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2706Keys, getTypedKeys)
{

	RETURN_MEMBER_TYPED(getThis(), "typedKeys", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2706Keys, getStaticKeys)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_issue2706keys_ce, SL("staticKeys"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

zend_object *zephir_init_properties_Stub_Issue2706Keys(zend_class_entry *class_type)
{
		zval _1$$3;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("keys"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 8, 0);
			add_index_stringl(&_1$$3, 1, SL("a"));
			add_index_stringl(&_1$$3, 1, SL("b"));
			add_index_stringl(&_1$$3, 0, SL("c"));
			add_assoc_stringl_ex(&_1$$3, SL(""), SL("d"));
			add_index_stringl(&_1$$3, 2, SL("e"));
			add_assoc_stringl_ex(&_1$$3, SL("3"), SL("f"));
			add_assoc_stringl_ex(&_1$$3, SL("03"), SL("g"));
			add_index_stringl(&_1$$3, -1, SL("h"));
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("keys"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

void zephir_init_static_properties_Stub_Issue2706Keys()
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 8, 0);
	add_index_stringl(&_0, 1, SL("a"));
	add_index_stringl(&_0, 1, SL("b"));
	add_index_stringl(&_0, 0, SL("c"));
	add_assoc_stringl_ex(&_0, SL(""), SL("d"));
	add_index_stringl(&_0, 2, SL("e"));
	add_assoc_stringl_ex(&_0, SL("3"), SL("f"));
	add_assoc_stringl_ex(&_0, SL("03"), SL("g"));
	add_index_stringl(&_0, -1, SL("h"));
	zephir_update_static_property_ce(stub_issue2706keys_ce, ZEND_STRL("staticKeys"), &_0);
	ZEPHIR_MM_RESTORE();
}

