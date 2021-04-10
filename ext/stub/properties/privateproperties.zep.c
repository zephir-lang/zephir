
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


ZEPHIR_INIT_CLASS(Stub_Properties_PrivateProperties)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, PrivateProperties, stub, properties_privateproperties, stub_properties_privateproperties_method_entry, 0);

	/**
	 * This is a private property with no initial value
	 */
	zend_declare_property_null(stub_properties_privateproperties_ce, SL("someNull"), ZEND_ACC_PRIVATE);
	/**
	 * This is a private property with initial null value
	 */
	zend_declare_property_null(stub_properties_privateproperties_ce, SL("someNullInitial"), ZEND_ACC_PRIVATE);
	/**
	 * This is a private property with initial boolean false
	 */
	zend_declare_property_bool(stub_properties_privateproperties_ce, SL("someFalse"), 0, ZEND_ACC_PRIVATE);
	/**
	 * This is a private property with initial boolean true
	 */
	zend_declare_property_bool(stub_properties_privateproperties_ce, SL("someTrue"), 1, ZEND_ACC_PRIVATE);
	/**
	 * This is a private property with an initial integer value
	 */
	zend_declare_property_long(stub_properties_privateproperties_ce, SL("someInteger"), 10, ZEND_ACC_PRIVATE);
	/**
	 * This is a private property with an initial double value
	 */
	zend_declare_property_double(stub_properties_privateproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PRIVATE);
	/**
	 * This is a private property with an initial string value
	 */
	zend_declare_property_string(stub_properties_privateproperties_ce, SL("someString"), "test", ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_Properties_PrivateProperties, getSomeNull)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someNull");
}

PHP_METHOD(Stub_Properties_PrivateProperties, getSomeNullInitial)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someNullInitial");
}

PHP_METHOD(Stub_Properties_PrivateProperties, getSomeFalse)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someFalse");
}

PHP_METHOD(Stub_Properties_PrivateProperties, getSomeTrue)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someTrue");
}

PHP_METHOD(Stub_Properties_PrivateProperties, getSomeInteger)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someInteger");
}

PHP_METHOD(Stub_Properties_PrivateProperties, getSomeDouble)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someDouble");
}

PHP_METHOD(Stub_Properties_PrivateProperties, getSomeString)
{
	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "someString");
}

