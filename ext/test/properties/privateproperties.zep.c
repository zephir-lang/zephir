
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


ZEPHIR_INIT_CLASS(Test_Properties_PrivateProperties) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, PrivateProperties, test, properties_privateproperties, test_properties_privateproperties_method_entry, 0);

	/**
	 * This is a private property with no initial value
	 */
	zend_declare_property_null(test_properties_privateproperties_ce, SL("someNull"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/**
	 * This is a private property with initial null value
	 */
	zend_declare_property_null(test_properties_privateproperties_ce, SL("someNullInitial"), ZEND_ACC_PRIVATE TSRMLS_CC);

	/**
	 * This is a private property with initial boolean false
	 */
	zend_declare_property_bool(test_properties_privateproperties_ce, SL("someFalse"), 0, ZEND_ACC_PRIVATE TSRMLS_CC);

	/**
	 * This is a private property with initial boolean true
	 */
	zend_declare_property_bool(test_properties_privateproperties_ce, SL("someTrue"), 1, ZEND_ACC_PRIVATE TSRMLS_CC);

	/**
	 * This is a private property with an initial integer value
	 */
	zend_declare_property_long(test_properties_privateproperties_ce, SL("someInteger"), 10, ZEND_ACC_PRIVATE TSRMLS_CC);

	/**
	 * This is a private property with an initial double value
	 */
	zend_declare_property_double(test_properties_privateproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PRIVATE TSRMLS_CC);

	/**
	 * This is a private property with an initial string value
	 */
	zend_declare_property_string(test_properties_privateproperties_ce, SL("someString"), "test", ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_PrivateProperties, getSomeNull) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someNull");

}

PHP_METHOD(Test_Properties_PrivateProperties, getSomeNullInitial) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someNullInitial");

}

PHP_METHOD(Test_Properties_PrivateProperties, getSomeFalse) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someFalse");

}

PHP_METHOD(Test_Properties_PrivateProperties, getSomeTrue) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someTrue");

}

PHP_METHOD(Test_Properties_PrivateProperties, getSomeInteger) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someInteger");

}

PHP_METHOD(Test_Properties_PrivateProperties, getSomeDouble) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someDouble");

}

PHP_METHOD(Test_Properties_PrivateProperties, getSomeString) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someString");

}

