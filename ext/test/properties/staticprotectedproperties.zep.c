
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


ZEPHIR_INIT_CLASS(Test_Properties_StaticProtectedProperties) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, StaticProtectedProperties, test, properties_staticprotectedproperties, test_properties_staticprotectedproperties_method_entry, 0);

	/**
	 * This is a protected property with no initial value
	 */
	zend_declare_property_null(test_properties_staticprotectedproperties_ce, SL("someNull"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	/**
	 * This is a protected property with initial null value
	 */
	zend_declare_property_null(test_properties_staticprotectedproperties_ce, SL("someNullInitial"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	/**
	 * This is a protected property with initial boolean false
	 */
	zend_declare_property_bool(test_properties_staticprotectedproperties_ce, SL("someFalse"), 0, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	/**
	 * This is a protected property with initial boolean true
	 */
	zend_declare_property_bool(test_properties_staticprotectedproperties_ce, SL("someTrue"), 1, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	/**
	 * This is a protected property with an initial integer value
	 */
	zend_declare_property_long(test_properties_staticprotectedproperties_ce, SL("someInteger"), 10, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	/**
	 * This is a protected property with an initial double value
	 */
	zend_declare_property_double(test_properties_staticprotectedproperties_ce, SL("someDouble"), 10.25, ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	/**
	 * This is a protected property with an initial string value
	 */
	zend_declare_property_string(test_properties_staticprotectedproperties_ce, SL("someString"), "test", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	zend_declare_property_null(test_properties_staticprotectedproperties_ce, SL("someVar"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, setSomeVar) {

	zval *someVar;

	zephir_fetch_params(0, 1, 0, &someVar);



	zephir_update_property_this(this_ptr, SL("someVar"), someVar TSRMLS_CC);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeVar) {


	RETURN_MEMBER(this_ptr, "someVar");

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeNull) {

	zval *_0;


	_0 = zephir_fetch_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someNull") TSRMLS_CC);
	RETURN_CTORW(_0);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, setSomeNull) {

	zval *param;

	zephir_fetch_params(0, 1, 0, &param);



	zephir_update_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someNull"), &param TSRMLS_CC);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeNullInitial) {

	zval *_0;


	_0 = zephir_fetch_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someNullInitial") TSRMLS_CC);
	RETURN_CTORW(_0);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeFalse) {

	zval *_0;


	_0 = zephir_fetch_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someFalse") TSRMLS_CC);
	RETURN_CTORW(_0);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeTrue) {

	zval *_0;


	_0 = zephir_fetch_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someTrue") TSRMLS_CC);
	RETURN_CTORW(_0);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeInteger) {

	zval *_0;


	_0 = zephir_fetch_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someInteger") TSRMLS_CC);
	RETURN_CTORW(_0);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeDouble) {

	zval *_0;


	_0 = zephir_fetch_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someDouble") TSRMLS_CC);
	RETURN_CTORW(_0);

}

PHP_METHOD(Test_Properties_StaticProtectedProperties, getSomeString) {

	zval *_0;


	_0 = zephir_fetch_static_property_ce(test_properties_staticprotectedproperties_ce, SL("someString") TSRMLS_CC);
	RETURN_CTORW(_0);

}

