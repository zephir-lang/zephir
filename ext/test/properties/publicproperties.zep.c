
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
#include "kernel/array.h"

ZEPHIR_INIT_CLASS(Test_Properties_PublicProperties) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, PublicProperties, test, properties_publicproperties, test_properties_publicproperties_method_entry, 0);

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

	/**
	 * This is a public property with an initial empty-array value
	 */
	zend_declare_property_null(test_properties_publicproperties_ce, SL("someEmptyArray"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * This is a public property with an initial array value
	 */
	zend_declare_property_null(test_properties_publicproperties_ce, SL("someArray"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * It's needed to test ide stubs
	 * @link https://github.com/phalcon/zephir/issues/731
	 */
	zend_declare_property_null(test_properties_publicproperties_ce, SL("someArrayWithSpecifyKeys"), ZEND_ACC_PROTECTED TSRMLS_CC);

	/**
	 * It's needed to test ide stubs
	 * @link https://github.com/phalcon/zephir/issues/731
	 */
	zend_declare_property_null(test_properties_publicproperties_ce, SL("someArrayWithSubArray"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_properties_publicproperties_ce, SL("someGetterSetterArray"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_long(test_properties_publicproperties_ce, SL("testPropertyFromSelfConstantValue"), 123, ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_long(test_properties_publicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_class_constant_long(test_properties_publicproperties_ce, SL("TEST_CONST"), 123 TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_PublicProperties, setSomeGetterSetterArray) {

	zval *someGetterSetterArray;

	zephir_fetch_params(0, 1, 0, &someGetterSetterArray);



	zephir_update_property_this(this_ptr, SL("someGetterSetterArray"), someGetterSetterArray TSRMLS_CC);

}

PHP_METHOD(Test_Properties_PublicProperties, getSomeGetterSetterArray) {


	RETURN_MEMBER(this_ptr, "someGetterSetterArray");

}

PHP_METHOD(Test_Properties_PublicProperties, test394Issue) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(_0);
	zephir_read_property(&_0, this_ptr, SL("someTrue"), PH_NOISY_CC);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Properties_PublicProperties, __construct) {

	zval *_1, *_2 = NULL, *_3;
	zval *_0, *_4 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("someGetterSetterArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_create_array(_1, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	zephir_create_array(_2, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(_2, SS("class"), SL("alert alert-success"), 1);
	zephir_array_update_string(&_1, SL("success"), &_2, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_2);
	zephir_create_array(_2, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(_2, SS("class"), SL("alert alert-info"), 1);
	zephir_array_update_string(&_1, SL("info"), &_2, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_2);
	zephir_create_array(_2, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(_2, SS("class"), SL("alert alert-warning"), 1);
	zephir_array_update_string(&_1, SL("warning"), &_2, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_2);
	zephir_create_array(_2, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(_2, SS("class"), SL("alert alert-danger"), 1);
	zephir_array_update_string(&_1, SL("danger"), &_2, PH_COPY | PH_SEPARATE);
	zephir_update_property_this(this_ptr, SL("someArrayWithSubArray"), _1 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_2);
	zephir_create_array(_2, 3, 0 TSRMLS_CC);
	add_assoc_stringl_ex(_2, SS("a"), SL("A"), 1);
	add_assoc_stringl_ex(_2, SS("b"), SL("B"), 1);
	add_assoc_stringl_ex(_2, SS("c"), SL("C"), 1);
	zephir_update_property_this(this_ptr, SL("someArrayWithSpecifyKeys"), _2 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	zephir_create_array(_3, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_LONG(_4, 1);
	zephir_array_fast_append(_3, _4);
	ZEPHIR_INIT_NVAR(_4);
	ZVAL_LONG(_4, 2);
	zephir_array_fast_append(_3, _4);
	ZEPHIR_INIT_NVAR(_4);
	ZVAL_LONG(_4, 3);
	zephir_array_fast_append(_3, _4);
	ZEPHIR_INIT_NVAR(_4);
	ZVAL_LONG(_4, 4);
	zephir_array_fast_append(_3, _4);
	zephir_update_property_this(this_ptr, SL("someArray"), _3 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_4);
	array_init(_4);
	zephir_update_property_this(this_ptr, SL("someEmptyArray"), _4 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

