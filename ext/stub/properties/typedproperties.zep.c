
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


/**
 * Typed class properties (issue #2608).
 */
ZEPHIR_INIT_CLASS(Stub_Properties_TypedProperties)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, TypedProperties, stub, properties_typedproperties, stub_properties_typedproperties_method_entry, 0);

	{
		zval _zc0;
		array_init_size(&_zc0, 1);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("orgs"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_ARRAY, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_BOOL(&_zc0, 0);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("valid"), &_zc0, ZEND_ACC_PROTECTED, MAY_BE_BOOL, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_LONG(&_zc0, 7);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("counter"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_DOUBLE(&_zc0, 1.5);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("ratio"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_DOUBLE, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "hi", 2);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("label"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_STRING, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "", 0);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("val"), &_zc0, ZEND_ACC_PRIVATE, MAY_BE_STRING|MAY_BE_NULL, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_NULL(&_zc0);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("maybeNum"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_LONG|MAY_BE_NULL, NULL, 0);
	}

	{
		zval _zc0;
		ZVAL_UNDEF(&_zc0);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("friend"), &_zc0, ZEND_ACC_PUBLIC, 0, SL("Stub\\Properties\\PublicProperties"));
	}

	{
		zval _zc0;
		ZVAL_NULL(&_zc0);
		zephir_declare_typed_property(stub_properties_typedproperties_ce, SL("maybeFriend"), &_zc0, ZEND_ACC_PUBLIC, MAY_BE_NULL, SL("Stub\\Properties\\PublicProperties"));
	}

	return SUCCESS;
}

PHP_METHOD(Stub_Properties_TypedProperties, getVal)
{

	RETURN_MEMBER(getThis(), "val");
}

PHP_METHOD(Stub_Properties_TypedProperties, getMaybeNum)
{

	RETURN_MEMBER(getThis(), "maybeNum");
}

