
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 * Trait exposed to PHP userland (zephir#504).
 */
ZEPHIR_INIT_CLASS(Stub_Traits_Nameable)
{
	ZEPHIR_REGISTER_TRAIT(Stub\\Traits, Nameable, stub, traits_nameable, stub_traits_nameable_method_entry);

	zend_declare_property_string(stub_traits_nameable_ce, SL("name"), "unnamed", ZEND_ACC_PROTECTED);
	{
		zval _zc0;
		array_init_size(&_zc0, 4);
		add_next_index_long(&_zc0, 1);
		add_next_index_long(&_zc0, 2);
		add_next_index_long(&_zc0, 3);
		zephir_declare_property_array(stub_traits_nameable_ce, SL("tags"), &_zc0, ZEND_ACC_PROTECTED);
	}

	{
		zval _zc0;
		array_init_size(&_zc0, 3);
		add_assoc_long_ex(&_zc0, "a", 1, 1);
		zval _zc1;
		array_init_size(&_zc1, 3);
		add_next_index_long(&_zc1, 2);
		add_next_index_long(&_zc1, 3);
		add_assoc_zval_ex(&_zc0, "b", 1, &_zc1);
		zephir_declare_property_array(stub_traits_nameable_ce, SL("meta"), &_zc0, ZEND_ACC_PROTECTED);
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Traits_Nameable, setName)
{
	zval name_zv;
	zend_string *name = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&name_zv);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("name", 4, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(name)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&name_zv, name);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 123, &name_zv);
}

PHP_METHOD(Stub_Traits_Nameable, getName)
{

	RETURN_MEMBER_TYPED(getThis(), "name", IS_STRING);
}

PHP_METHOD(Stub_Traits_Nameable, getTags)
{

	RETURN_MEMBER_TYPED(getThis(), "tags", IS_ARRAY);
}

PHP_METHOD(Stub_Traits_Nameable, addTag)
{
	zval *tag, tag_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&tag_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(tag)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &tag);
	zephir_update_property_array_append(this_ptr, SL("tags"), tag);
}

PHP_METHOD(Stub_Traits_Nameable, getMeta)
{

	RETURN_MEMBER_TYPED(getThis(), "meta", IS_ARRAY);
}

