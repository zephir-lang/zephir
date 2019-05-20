
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
#include "kernel/operators.h"
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

	test_properties_publicproperties_ce->create_object = zephir_init_properties_Test_Properties_PublicProperties;
	zephir_declare_class_constant_long(test_properties_publicproperties_ce, SL("TEST_CONST"), 123);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_PublicProperties, setSomeGetterSetterArray) {

	zval *someGetterSetterArray, someGetterSetterArray_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&someGetterSetterArray_sub);

	zephir_fetch_params(0, 1, 0, &someGetterSetterArray);



	zephir_update_property_zval(this_ptr, SL("someGetterSetterArray"), someGetterSetterArray);
	RETURN_MM_THIS();

}

PHP_METHOD(Test_Properties_PublicProperties, getSomeGetterSetterArray) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "someGetterSetterArray");

}

PHP_METHOD(Test_Properties_PublicProperties, test394Issue) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	zephir_read_property(&_0, this_ptr, SL("someTrue"), PH_NOISY_CC);
	RETURN_MM_CTOR(&_0);

}

zend_object *zephir_init_properties_Test_Properties_PublicProperties(zend_class_entry *class_type TSRMLS_DC) {

		zval _3$$4, _4$$4, _6$$5, _8$$6;
	zval _0, _2, _5, _7, _10, _1$$3, _9$$6, _11$$7;
	
	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_9$$6);
	ZVAL_UNDEF(&_11$$7);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_6$$5);
	ZVAL_UNDEF(&_8$$6);

	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property(&_0, this_ptr, SL("someGetterSetterArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			array_init(&_1$$3);
			ZEPHIR_MM_ADD_ENTRY(&_1$$3);
			zephir_update_property_zval(this_ptr, SL("someGetterSetterArray"), &_1$$3);
		}
		zephir_read_property(&_2, this_ptr, SL("someArrayWithSubArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_2) == IS_NULL) {
			zephir_create_array(&_3$$4, 4, 0);
			ZEPHIR_MM_ADD_ENTRY(&_3$$4);
			zephir_create_array(&_4$$4, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_4$$4);
			add_assoc_stringl_ex(&_4$$4, SL("class"), SL("alert alert-success"));
			zephir_array_update_string(&_3$$4, SL("success"), &_4$$4, PH_SEPARATE);
			zephir_create_array(&_4$$4, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_4$$4);
			add_assoc_stringl_ex(&_4$$4, SL("class"), SL("alert alert-info"));
			zephir_array_update_string(&_3$$4, SL("info"), &_4$$4, PH_SEPARATE);
			zephir_create_array(&_4$$4, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_4$$4);
			add_assoc_stringl_ex(&_4$$4, SL("class"), SL("alert alert-warning"));
			zephir_array_update_string(&_3$$4, SL("warning"), &_4$$4, PH_SEPARATE);
			zephir_create_array(&_4$$4, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_4$$4);
			add_assoc_stringl_ex(&_4$$4, SL("class"), SL("alert alert-danger"));
			zephir_array_update_string(&_3$$4, SL("danger"), &_4$$4, PH_SEPARATE);
			zephir_update_property_zval(this_ptr, SL("someArrayWithSubArray"), &_3$$4);
		}
		zephir_read_property(&_5, this_ptr, SL("someArrayWithSpecifyKeys"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_5) == IS_NULL) {
			zephir_create_array(&_6$$5, 3, 0);
			ZEPHIR_MM_ADD_ENTRY(&_6$$5);
			add_assoc_stringl_ex(&_6$$5, SL("a"), SL("A"));
			add_assoc_stringl_ex(&_6$$5, SL("b"), SL("B"));
			add_assoc_stringl_ex(&_6$$5, SL("c"), SL("C"));
			zephir_update_property_zval(this_ptr, SL("someArrayWithSpecifyKeys"), &_6$$5);
		}
		zephir_read_property(&_7, this_ptr, SL("someArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_7) == IS_NULL) {
			zephir_create_array(&_8$$6, 4, 0);
			ZEPHIR_MM_ADD_ENTRY(&_8$$6);
			ZVAL_LONG(&_9$$6, 1);
			zephir_array_fast_append(&_8$$6, &_9$$6);
			ZVAL_LONG(&_9$$6, 2);
			zephir_array_fast_append(&_8$$6, &_9$$6);
			ZVAL_LONG(&_9$$6, 3);
			zephir_array_fast_append(&_8$$6, &_9$$6);
			ZVAL_LONG(&_9$$6, 4);
			zephir_array_fast_append(&_8$$6, &_9$$6);
			zephir_update_property_zval(this_ptr, SL("someArray"), &_8$$6);
		}
		zephir_read_property(&_10, this_ptr, SL("someEmptyArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_10) == IS_NULL) {
			array_init(&_11$$7);
			ZEPHIR_MM_ADD_ENTRY(&_11$$7);
			zephir_update_property_zval(this_ptr, SL("someEmptyArray"), &_11$$7);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}

}

