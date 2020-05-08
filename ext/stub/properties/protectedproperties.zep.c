
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


ZEPHIR_INIT_CLASS(Stub_Properties_ProtectedProperties) {

	ZEPHIR_REGISTER_CLASS(Stub\\Properties, ProtectedProperties, stub, properties_protectedproperties, stub_properties_protectedproperties_method_entry, 0);

	/**
	 * This is a protected property with no initial value
	 */
	zend_declare_property_null(stub_properties_protectedproperties_ce, SL("someNull"), ZEND_ACC_PROTECTED);

	/**
	 * This is a protected property with initial null value
	 */
	zend_declare_property_null(stub_properties_protectedproperties_ce, SL("someNullInitial"), ZEND_ACC_PROTECTED);

	/**
	 * This is a protected property with initial boolean false
	 */
	zend_declare_property_bool(stub_properties_protectedproperties_ce, SL("someFalse"), 0, ZEND_ACC_PROTECTED);

	/**
	 * This is a protected property with initial boolean true
	 */
	zend_declare_property_bool(stub_properties_protectedproperties_ce, SL("someTrue"), 1, ZEND_ACC_PROTECTED);

	/**
	 * This is a protected property with an initial integer value
	 */
	zend_declare_property_long(stub_properties_protectedproperties_ce, SL("someInteger"), 10, ZEND_ACC_PROTECTED);

	/**
	 * This is a protected property with an initial double value
	 */
	zend_declare_property_double(stub_properties_protectedproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PROTECTED);

	/**
	 * This is a protected property with an initial string value
	 */
	zend_declare_property_string(stub_properties_protectedproperties_ce, SL("someString"), "test", ZEND_ACC_PROTECTED);

	zend_declare_property_null(stub_properties_protectedproperties_ce, SL("someVar"), ZEND_ACC_PROTECTED);

	return SUCCESS;

}

PHP_METHOD(Stub_Properties_ProtectedProperties, setSomeVar) {

	zval *someVar, someVar_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&someVar_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &someVar);



	zephir_update_property_zval(this_ptr, ZEND_STRL("someVar"), someVar);
	RETURN_THISW();

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeVar) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someVar");

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeNull) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someNull");

}

PHP_METHOD(Stub_Properties_ProtectedProperties, setSomeNull) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &param);



	zephir_update_property_zval(this_ptr, ZEND_STRL("someNull"), param);

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeNullInitial) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someNullInitial");

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeFalse) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someFalse");

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeTrue) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someTrue");

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeInteger) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someInteger");

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeDouble) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someDouble");

}

PHP_METHOD(Stub_Properties_ProtectedProperties, getSomeString) {

	zval *this_ptr = getThis();


	RETURN_MEMBER(getThis(), "someString");

}

