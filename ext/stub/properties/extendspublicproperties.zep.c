
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


ZEPHIR_INIT_CLASS(Stub_Properties_ExtendsPublicProperties) {

	ZEPHIR_REGISTER_CLASS_EX(Stub\\Properties, ExtendsPublicProperties, stub, properties_extendspublicproperties, stub_properties_publicproperties_ce, NULL, 0);

	zend_declare_property_long(stub_properties_extendspublicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC);

	zend_declare_property_null(stub_properties_extendspublicproperties_ce, SL("someGetterSetterArray"), ZEND_ACC_PROTECTED);

	stub_properties_extendspublicproperties_ce->create_object = zephir_init_properties_Stub_Properties_ExtendsPublicProperties;
	return SUCCESS;

}

zend_object *zephir_init_properties_Stub_Properties_ExtendsPublicProperties(zend_class_entry *class_type) {

		zval _1$$3, _2$$3, _4$$4, _6$$5, _11$$7;
	zval _0, _3, _5, _8, _10, _7$$5, _9$$6, _12$$7;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
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
	

		ZEPHIR_MM_GROW();
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("someArrayWithSubArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			zephir_create_array(&_1$$3, 4, 0);
			ZEPHIR_INIT_VAR(&_2$$3);
			zephir_create_array(&_2$$3, 1, 0);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-success"));
			zephir_array_update_string(&_1$$3, SL("success"), &_2$$3, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(&_2$$3);
			zephir_create_array(&_2$$3, 1, 0);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-info"));
			zephir_array_update_string(&_1$$3, SL("info"), &_2$$3, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(&_2$$3);
			zephir_create_array(&_2$$3, 1, 0);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-warning"));
			zephir_array_update_string(&_1$$3, SL("warning"), &_2$$3, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(&_2$$3);
			zephir_create_array(&_2$$3, 1, 0);
			add_assoc_stringl_ex(&_2$$3, SL("class"), SL("alert alert-danger"));
			zephir_array_update_string(&_1$$3, SL("danger"), &_2$$3, PH_COPY | PH_SEPARATE);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("someArrayWithSubArray"), &_1$$3);
		}
		zephir_read_property_ex(&_3, this_ptr, ZEND_STRL("someArrayWithSpecifyKeys"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_3) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_4$$4);
			zephir_create_array(&_4$$4, 3, 0);
			add_assoc_stringl_ex(&_4$$4, SL("a"), SL("A"));
			add_assoc_stringl_ex(&_4$$4, SL("b"), SL("B"));
			add_assoc_stringl_ex(&_4$$4, SL("c"), SL("C"));
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("someArrayWithSpecifyKeys"), &_4$$4);
		}
		zephir_read_property_ex(&_5, this_ptr, ZEND_STRL("someArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_5) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_6$$5);
			zephir_create_array(&_6$$5, 4, 0);
			ZEPHIR_INIT_VAR(&_7$$5);
			ZVAL_LONG(&_7$$5, 1);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			ZEPHIR_INIT_NVAR(&_7$$5);
			ZVAL_LONG(&_7$$5, 2);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			ZEPHIR_INIT_NVAR(&_7$$5);
			ZVAL_LONG(&_7$$5, 3);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			ZEPHIR_INIT_NVAR(&_7$$5);
			ZVAL_LONG(&_7$$5, 4);
			zephir_array_fast_append(&_6$$5, &_7$$5);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("someArray"), &_6$$5);
		}
		zephir_read_property_ex(&_8, this_ptr, ZEND_STRL("someEmptyArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_8) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_9$$6);
			array_init(&_9$$6);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("someEmptyArray"), &_9$$6);
		}
		zephir_read_property_ex(&_10, this_ptr, ZEND_STRL("someGetterSetterArray"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_10) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_11$$7);
			zephir_create_array(&_11$$7, 1, 0);
			ZEPHIR_INIT_VAR(&_12$$7);
			ZVAL_STRING(&_12$$7, "holy");
			zephir_array_fast_append(&_11$$7, &_12$$7);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("someGetterSetterArray"), &_11$$7);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}

}

