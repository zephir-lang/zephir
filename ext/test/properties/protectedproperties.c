
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


ZEPHIR_INIT_CLASS(Test_Properties_ProtectedProperties) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, ProtectedProperties, test, properties_protectedproperties, test_properties_protectedproperties_method_entry, 0);

	/**
	 * This is a protected property with no initial value
	 */
	zend_declare_property_null(test_properties_protectedproperties_ce, SL("someNull"), ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * This is a protected property with initial null value
	 */
	zend_declare_property_null(test_properties_protectedproperties_ce, SL("someNullInitial"), ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * This is a protected property with initial boolean false
	 */
	zend_declare_property_bool(test_properties_protectedproperties_ce, SL("someFalse"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * This is a protected property with initial boolean true
	 */
	zend_declare_property_bool(test_properties_protectedproperties_ce, SL("someTrue"), 1, ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * This is a protected property with an initial integer value
	 */
	zend_declare_property_long(test_properties_protectedproperties_ce, SL("someInteger"), 10, ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * This is a protected property with an initial double value
	 */
	zend_declare_property_double(test_properties_protectedproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * This is a protected property with an initial string value
	 */
	zend_declare_property_string(test_properties_protectedproperties_ce, SL("someString"), "test", ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_properties_protectedproperties_ce, SL("someVar"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_ProtectedProperties, setSomeVar) {

	zval *someVar;

	zephir_fetch_params(0, 1, 0, &someVar);



	zephir_update_property_this(this_ptr, SL("someVar"), someVar TSRMLS_CC);

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeVar) {


	RETURN_MEMBER(this_ptr, "someVar");

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeNull) {


	RETURN_MEMBER(this_ptr, "someNull");

}

PHP_METHOD(Test_Properties_ProtectedProperties, setSomeNull) {

	zval *param;

	zephir_fetch_params(0, 1, 0, &param);



	zephir_update_property_this(this_ptr, SL("someNull"), param TSRMLS_CC);

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeNullInitial) {


	RETURN_MEMBER(this_ptr, "someNullInitial");

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeFalse) {


	RETURN_MEMBER(this_ptr, "someFalse");

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeTrue) {


	RETURN_MEMBER(this_ptr, "someTrue");

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeInteger) {


	RETURN_MEMBER(this_ptr, "someInteger");

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeDouble) {


	RETURN_MEMBER(this_ptr, "someDouble");

}

PHP_METHOD(Test_Properties_ProtectedProperties, getSomeString) {


	RETURN_MEMBER(this_ptr, "someString");

}

