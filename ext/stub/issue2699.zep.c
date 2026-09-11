
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


/**
 * The keys of a persistent array default belong to the module, not to any copy
 * of it. zephir_persist_constant_zval() (kernel/main.c) marks the shared table
 * HASH_FLAG_STATIC_KEYS, and zend_array_dup()'s immutable branch is a raw
 * bucket memcpy that inherits the flag without addref'ing anything. A copy that
 * then loses the flag releases every key, including the ones it only borrowed.
 *
 * A copy loses the flag on the first insert of a NON-interned key. A PHP
 * literal offset is interned at compile time, so only a key built at runtime
 * reaches the defect. Every test against this fixture must build its keys.
 *
 * The shape is Phalcon\Acl\Adapter\Memory's: a "*" key in the default and a
 * runtime component name written over it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2699
 */
ZEPHIR_INIT_CLASS(Stub_Issue2699)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2699, stub, issue2699, stub_issue2699_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		add_assoc_bool_ex(&_zc0, SL("*"), 1);
		zephir_declare_typed_property(stub_issue2699_ce, SL("components"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		zval _zc1;
		array_init_size(&_zc1, 2);
		add_assoc_bool_ex(&_zc1, SL("inner"), 1);
		add_assoc_zval_ex(&_zc0, SL("outer"), &_zc1);
		zephir_declare_typed_property(stub_issue2699_ce, SL("nested"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_bool_ex(&_zc0, SL("*"), 1);
		add_assoc_long_ex(&_zc0, SL("a"), 1);
		zephir_declare_class_constant_array(stub_issue2699_ce, SL("MAP"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2699, add)
{
	zval key_zv, __$true;
	zend_string *key = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_BOOL(&__$true, 1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&key_zv, key);
	zephir_update_property_array(this_ptr, SL("components"), &key_zv, &__$true);
}

PHP_METHOD(Stub_Issue2699, getComponents)
{

	RETURN_MEMBER_TYPED(getThis(), "components", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2699, getNested)
{

	RETURN_MEMBER_TYPED(getThis(), "nested", IS_ARRAY);
}

