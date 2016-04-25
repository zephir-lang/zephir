
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

zend_object_value zephir_init_properties_Test_Properties_ExtendsPublicProperties(zend_class_entry *class_type TSRMLS_DC) {

		zval *_1$$3, *_2$$3 = NULL, *_4$$4, *_6$$5, *_11$$7;
	zval *_0, *_3, *_5, *_8, *_10, *_7$$5 = NULL, *_9$$6, *_12$$7;

		ZEPHIR_MM_GROW();
	
	{
		zval *this_ptr = NULL;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("someArrayWithSubArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(_1$$3);
			zephir_create_array(_1$$3, 4, 0 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_2$$3);
			zephir_create_array(_2$$3, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_2$$3, SS("class"), SL("alert alert-success"), 1);
			zephir_array_update_string(&_1$$3, SL("success"), &_2$$3, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_2$$3);
			zephir_create_array(_2$$3, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_2$$3, SS("class"), SL("alert alert-info"), 1);
			zephir_array_update_string(&_1$$3, SL("info"), &_2$$3, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_2$$3);
			zephir_create_array(_2$$3, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_2$$3, SS("class"), SL("alert alert-warning"), 1);
			zephir_array_update_string(&_1$$3, SL("warning"), &_2$$3, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_2$$3);
			zephir_create_array(_2$$3, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_2$$3, SS("class"), SL("alert alert-danger"), 1);
			zephir_array_update_string(&_1$$3, SL("danger"), &_2$$3, PH_COPY | PH_SEPARATE);
			zephir_update_property_this(this_ptr, SL("someArrayWithSubArray"), _1$$3 TSRMLS_CC);
		}
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("someArrayWithSpecifyKeys"), PH_NOISY_CC);
		if (Z_TYPE_P(_3) == IS_NULL) {
			ZEPHIR_INIT_VAR(_4$$4);
			zephir_create_array(_4$$4, 3, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_4$$4, SS("a"), SL("A"), 1);
			add_assoc_stringl_ex(_4$$4, SS("b"), SL("B"), 1);
			add_assoc_stringl_ex(_4$$4, SS("c"), SL("C"), 1);
			zephir_update_property_this(this_ptr, SL("someArrayWithSpecifyKeys"), _4$$4 TSRMLS_CC);
		}
		_5 = zephir_fetch_nproperty_this(this_ptr, SL("someArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_5) == IS_NULL) {
			ZEPHIR_INIT_VAR(_6$$5);
			zephir_create_array(_6$$5, 4, 0 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_7$$5);
			ZVAL_LONG(_7$$5, 1);
			zephir_array_fast_append(_6$$5, _7$$5);
			ZEPHIR_INIT_NVAR(_7$$5);
			ZVAL_LONG(_7$$5, 2);
			zephir_array_fast_append(_6$$5, _7$$5);
			ZEPHIR_INIT_NVAR(_7$$5);
			ZVAL_LONG(_7$$5, 3);
			zephir_array_fast_append(_6$$5, _7$$5);
			ZEPHIR_INIT_NVAR(_7$$5);
			ZVAL_LONG(_7$$5, 4);
			zephir_array_fast_append(_6$$5, _7$$5);
			zephir_update_property_this(this_ptr, SL("someArray"), _6$$5 TSRMLS_CC);
		}
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("someEmptyArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_8) == IS_NULL) {
			ZEPHIR_INIT_VAR(_9$$6);
			array_init(_9$$6);
			zephir_update_property_this(this_ptr, SL("someEmptyArray"), _9$$6 TSRMLS_CC);
		}
		_10 = zephir_fetch_nproperty_this(this_ptr, SL("someGetterSetterArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_10) == IS_NULL) {
			ZEPHIR_INIT_VAR(_11$$7);
			zephir_create_array(_11$$7, 1, 0 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_12$$7);
			ZVAL_STRING(_12$$7, "holy", 1);
			zephir_array_fast_append(_11$$7, _12$$7);
			zephir_update_property_this(this_ptr, SL("someGetterSetterArray"), _11$$7 TSRMLS_CC);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJVAL_P(this_ptr);
	}

}

