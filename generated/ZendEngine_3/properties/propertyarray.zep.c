
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * @link https://github.com/phalcon/zephir/issues/520
 */
ZEPHIR_INIT_CLASS(Test_Properties_PropertyArray) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, PropertyArray, test, properties_propertyarray, test_properties_propertyarray_method_entry, 0);

	/**
	 * This is a public property with an initial empty-array value
	 */
	zend_declare_property_null(test_properties_propertyarray_ce, SL("someEmptyArray"), ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * This is a public property with an initial array value
	 */
	zend_declare_property_null(test_properties_propertyarray_ce, SL("someArray"), ZEND_ACC_PUBLIC TSRMLS_CC);

	test_properties_propertyarray_ce->create_object = zephir_init_properties_Test_Properties_PropertyArray;
	return SUCCESS;

}

PHP_METHOD(Test_Properties_PropertyArray, __construct) {

	zval _1;
	zval _0;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	zephir_update_property_zval(this_ptr, SL("someArray"), &_0);
	ZEPHIR_MM_RESTORE();

}

zend_object *zephir_init_properties_Test_Properties_PropertyArray(zend_class_entry *class_type TSRMLS_DC) {

		zval _1$$3;
	zval _0, _3, _2$$3, _4$$4;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_1$$3);

		ZEPHIR_MM_GROW();
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property(&_0, this_ptr, SL("someArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 4, 0 TSRMLS_CC);
			ZEPHIR_INIT_VAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 1);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 2);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 3);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			ZEPHIR_INIT_NVAR(&_2$$3);
			ZVAL_LONG(&_2$$3, 4);
			zephir_array_fast_append(&_1$$3, &_2$$3);
			zephir_update_property_zval(this_ptr, SL("someArray"), &_1$$3);
		}
		zephir_read_property(&_3, this_ptr, SL("someEmptyArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_3) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_4$$4);
			array_init(&_4$$4);
			zephir_update_property_zval(this_ptr, SL("someEmptyArray"), &_4$$4);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}

}

