
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
 * A persistent array default (typed property or class constant) must behave like
 * a PHP one: shared until written, copied on first write. Before #2651 the
 * persistent table was refcount 1, so copy-on-write never separated it and a
 * userland write landed in the single table every instance points at.
 *
 * The string keys and string values are deliberate: they are what makes
 * zend_array_dup()'s immutable branch (a raw bucket memcpy) observable.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2651
 */
ZEPHIR_INIT_CLASS(Stub_Issue2651)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2651, stub, issue2651, stub_issue2651_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_typed_property(stub_issue2651_ce, SL("headers"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_stringl_ex(&_zc0, SL("x"), SL("one"));
		add_assoc_stringl_ex(&_zc0, SL("y"), SL("two"));
		zephir_declare_typed_property(stub_issue2651_ce, SL("preset"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		zval _zc1;
		array_init_size(&_zc1, 2);
		add_assoc_stringl_ex(&_zc1, SL("inner"), SL("deep"));
		add_assoc_zval_ex(&_zc0, SL("outer"), &_zc1);
		zephir_declare_typed_property(stub_issue2651_ce, SL("nested"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_stringl_ex(&_zc0, SL("a"), SL("first"));
		add_assoc_stringl_ex(&_zc0, SL("b"), SL("second"));
		zephir_declare_class_constant_array(stub_issue2651_ce, SL("MAP"), &_zc0);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2651, toArray)
{

	RETURN_MEMBER_TYPED(getThis(), "headers", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2651, getPreset)
{

	RETURN_MEMBER_TYPED(getThis(), "preset", IS_ARRAY);
}

PHP_METHOD(Stub_Issue2651, getNested)
{

	RETURN_MEMBER_TYPED(getThis(), "nested", IS_ARRAY);
}

/**
 * An internal write must separate too, and must not disturb other instances.
 */
PHP_METHOD(Stub_Issue2651, addHeader)
{
	zval key_zv, value_zv;
	zend_string *key = NULL, *value = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_zv);
	ZVAL_UNDEF(&value_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(key)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&key_zv, key);
	ZVAL_STR(&value_zv, value);
	zephir_update_property_array(this_ptr, SL("headers"), &key_zv, &value_zv);
}

