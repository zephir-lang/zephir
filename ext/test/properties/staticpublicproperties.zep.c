
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


ZEPHIR_INIT_CLASS(Test_Properties_StaticPublicProperties) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, StaticPublicProperties, test, properties_staticpublicproperties, test_properties_staticpublicproperties_method_entry, 0);

	/**
	 * This is a public property with no initial value
	 */
	zend_declare_property_null(test_properties_staticpublicproperties_ce, SL("someNull"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	/**
	 * This is a public property with initial null value
	 */
	zend_declare_property_null(test_properties_staticpublicproperties_ce, SL("someNullInitial"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	/**
	 * This is a public property with initial boolean false
	 */
	zend_declare_property_bool(test_properties_staticpublicproperties_ce, SL("someFalse"), 0, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	/**
	 * This is a public property with initial boolean true
	 */
	zend_declare_property_bool(test_properties_staticpublicproperties_ce, SL("someTrue"), 1, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	/**
	 * This is a public property with an initial integer value
	 */
	zend_declare_property_long(test_properties_staticpublicproperties_ce, SL("someInteger"), 10, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	/**
	 * This is a public property with an initial double value
	 */
	zend_declare_property_double(test_properties_staticpublicproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	/**
	 * This is a public property with an initial string value
	 */
	zend_declare_property_string(test_properties_staticpublicproperties_ce, SL("someString"), "test", ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_StaticPublicProperties, setSomeString) {

	zval *val, val_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&val_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &val);



	zephir_update_static_property_ce(test_properties_staticpublicproperties_ce, ZEND_STRL("someString"), val);

}

