
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Properties_ExtendsPublicProperties) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Properties, ExtendsPublicProperties, test, properties_extendspublicproperties, test_properties_publicproperties_ce, NULL, 0);

	zend_declare_property_long(test_properties_extendspublicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(test_properties_extendspublicproperties_ce, SL("someGetterSetterArray"), ZEND_ACC_PROTECTED TSRMLS_CC);

	test_properties_extendspublicproperties_ce->create_object = zephir_init_properties_Test_Properties_ExtendsPublicProperties;
	return SUCCESS;

}

zend_object *zephir_init_properties_Test_Properties_ExtendsPublicProperties(zend_class_entry *class_type TSRMLS_DC) {

		zval _1$$3, _2$$3, _4$$4, _6$$5, _11$$7;
	zval _0, _3, _5, _8, _10, _7$$5, _9$$6, _12$$7;
	
	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_7$$5);
	ZVAL_UNDEF(&_9$$6);
	ZVAL_UNDEF(&_12$$7);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_6$$5);
	ZVAL_UNDEF(&_11$$7);

	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property(&_0, this_ptr, SL("someArrayWithSubArray"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_SEPARATE(&_0);
		ZEPHIR_MM_ADD_ENTRY(&_0);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			zephir_create_array(&_1$$3, 4, 0);
			ZEPHIR_MM_ADD_ENTRY(&_1$$3);
			zephir_create_array(&_2$$3, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_2$$3);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-success"));
			zephir_array_update_string(&_1$$3, SL("success"), &_2$$3, PH_SEPARATE);
			zephir_create_array(&_2$$3, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_2$$3);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-info"));
			zephir_array_update_string(&_1$$3, SL("info"), &_2$$3, PH_SEPARATE);
			zephir_create_array(&_2$$3, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_2$$3);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-warning"));
			zephir_array_update_string(&_1$$3, SL("warning"), &_2$$3, PH_SEPARATE);
			zephir_create_array(&_2$$3, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_2$$3);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-danger"));
			zephir_array_update_string(&_1$$3, SL("danger"), &_2$$3, PH_SEPARATE);
			zephir_update_property_zval(this_ptr, SL("someArrayWithSubArray"), &_1$$3);
		}
		zephir_read_property(&_3, this_ptr, SL("someArrayWithSpecifyKeys"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_SEPARATE(&_3);
		ZEPHIR_MM_ADD_ENTRY(&_3);
		if (Z_TYPE_P(&_3) == IS_NULL) {
			zephir_create_array(&_4$$4, 3, 0);
			ZEPHIR_MM_ADD_ENTRY(&_4$$4);
			add_assoc_stringl_ex(&_4$$4, SL("a"), SL("A"));
			add_assoc_stringl_ex(&_4$$4, SL("b"), SL("B"));
			add_assoc_stringl_ex(&_4$$4, SL("c"), SL("C"));
			zephir_update_property_zval(this_ptr, SL("someArrayWithSpecifyKeys"), &_4$$4);
		}
		zephir_read_property(&_5, this_ptr, SL("someArray"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_SEPARATE(&_5);
		ZEPHIR_MM_ADD_ENTRY(&_5);
		if (Z_TYPE_P(&_5) == IS_NULL) {
			zephir_create_array(&_6$$5, 4, 0);
			ZEPHIR_MM_ADD_ENTRY(&_6$$5);
			ZVAL_LONG(&_7$$5, 1);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			ZVAL_LONG(&_7$$5, 2);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			ZVAL_LONG(&_7$$5, 3);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			ZVAL_LONG(&_7$$5, 4);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			zephir_update_property_zval(this_ptr, SL("someArray"), &_6$$5);
		}
		zephir_read_property(&_8, this_ptr, SL("someEmptyArray"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_SEPARATE(&_8);
		ZEPHIR_MM_ADD_ENTRY(&_8);
		if (Z_TYPE_P(&_8) == IS_NULL) {
			array_init(&_9$$6);
			ZEPHIR_MM_ADD_ENTRY(&_9$$6);
			zephir_update_property_zval(this_ptr, SL("someEmptyArray"), &_9$$6);
		}
		zephir_read_property(&_10, this_ptr, SL("someGetterSetterArray"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_SEPARATE(&_10);
		ZEPHIR_MM_ADD_ENTRY(&_10);
		if (Z_TYPE_P(&_10) == IS_NULL) {
			zephir_create_array(&_11$$7, 1, 0);
			ZEPHIR_MM_ADD_ENTRY(&_11$$7);
			ZEPHIR_MM_ZVAL_STRING(&_12$$7, "holy");
			zephir_array_fast_append(&_11$$7, &_12$$7);
			zephir_update_property_zval(this_ptr, SL("someGetterSetterArray"), &_11$$7);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}

}

