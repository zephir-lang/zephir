
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_Properties_PublicProperties) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, PublicProperties, properties_publicproperties, NULL, 0);

/**
 * This is a public property with no initial value
 */
	zend_declare_property_null(test_properties_publicproperties_ce, SL("someNull"), ZEND_ACC_PUBLIC TSRMLS_CC);
/**
 * This is a public property with initial null value
 */
	zend_declare_property_null(test_properties_publicproperties_ce, SL("someNullInitial"), ZEND_ACC_PUBLIC TSRMLS_CC);
/**
 * This is a public property with initial boolean false
 */
	zend_declare_property_bool(test_properties_publicproperties_ce, SL("someFalse"), 0, ZEND_ACC_PUBLIC TSRMLS_CC);
/**
 * This is a public property with initial boolean true
 */
	zend_declare_property_bool(test_properties_publicproperties_ce, SL("someTrue"), 1, ZEND_ACC_PUBLIC TSRMLS_CC);
/**
 * This is a public property with an initial integer value
 */
	zend_declare_property_long(test_properties_publicproperties_ce, SL("someInteger"), 10, ZEND_ACC_PUBLIC TSRMLS_CC);
/**
 * This is a public property with an initial double value
 */
	zend_declare_property_double(test_properties_publicproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PUBLIC TSRMLS_CC);
/**
 * This is a public property with an initial string value
 */
	zend_declare_property_string(test_properties_publicproperties_ce, SL("someString"), "test", ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

