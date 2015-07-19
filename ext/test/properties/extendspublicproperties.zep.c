
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

	ZEPHIR_REGISTER_CLASS_EX(Test\\Properties, ExtendsPublicProperties, test, properties_extendspublicproperties, test_properties_publicproperties_ce, test_properties_extendspublicproperties_method_entry, 0);

	zend_declare_property_long(test_properties_extendspublicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(test_properties_extendspublicproperties_ce, SL("someGetterSetterArray"), ZEND_ACC_PROTECTED TSRMLS_CC);

	test_properties_extendspublicproperties_ce->create_object = zephir_init_properties_Test_Properties_ExtendsPublicProperties;
	return SUCCESS;

}

static zend_object_value zephir_init_properties_Test_Properties_ExtendsPublicProperties(zend_class_entry *class_type TSRMLS_DC) {

		zval *_1 = NULL, *_4 = NULL;
		zval *_0, *_2 = NULL, *_3, *_5, *_6, *_7;

		ZEPHIR_MM_GROW();
	
	{
		zval *this_ptr = NULL;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		_0 = zephir_fetch_nproperty_this(this_ptr, SL("someGetterSetterArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(_1);
			zephir_create_array(_1, 1, 0 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_2);
			ZVAL_STRING(_2, "holy", 1);
			zephir_array_fast_append(_1, _2);
			zephir_update_property_this(this_ptr, SL("someGetterSetterArray"), _1 TSRMLS_CC);
		}
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("someArrayWithSubArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_3) == IS_NULL) {
			ZEPHIR_INIT_NVAR(_1);
			zephir_create_array(_1, 4, 0 TSRMLS_CC);
			ZEPHIR_INIT_VAR(_4);
			zephir_create_array(_4, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_4, SS("class"), SL("alert alert-success"), 1);
			zephir_array_update_string(&_1, SL("success"), &_4, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_4);
			zephir_create_array(_4, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_4, SS("class"), SL("alert alert-info"), 1);
			zephir_array_update_string(&_1, SL("info"), &_4, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_4);
			zephir_create_array(_4, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_4, SS("class"), SL("alert alert-warning"), 1);
			zephir_array_update_string(&_1, SL("warning"), &_4, PH_COPY | PH_SEPARATE);
			ZEPHIR_INIT_NVAR(_4);
			zephir_create_array(_4, 1, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_4, SS("class"), SL("alert alert-danger"), 1);
			zephir_array_update_string(&_1, SL("danger"), &_4, PH_COPY | PH_SEPARATE);
			zephir_update_property_this(this_ptr, SL("someArrayWithSubArray"), _1 TSRMLS_CC);
		}
		_5 = zephir_fetch_nproperty_this(this_ptr, SL("someArrayWithSpecifyKeys"), PH_NOISY_CC);
		if (Z_TYPE_P(_5) == IS_NULL) {
			ZEPHIR_INIT_NVAR(_1);
			zephir_create_array(_1, 3, 0 TSRMLS_CC);
			add_assoc_stringl_ex(_1, SS("a"), SL("A"), 1);
			add_assoc_stringl_ex(_1, SS("b"), SL("B"), 1);
			add_assoc_stringl_ex(_1, SS("c"), SL("C"), 1);
			zephir_update_property_this(this_ptr, SL("someArrayWithSpecifyKeys"), _1 TSRMLS_CC);
		}
		_6 = zephir_fetch_nproperty_this(this_ptr, SL("someArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_6) == IS_NULL) {
			ZEPHIR_INIT_NVAR(_1);
			zephir_create_array(_1, 4, 0 TSRMLS_CC);
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_LONG(_2, 1);
			zephir_array_fast_append(_1, _2);
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_LONG(_2, 2);
			zephir_array_fast_append(_1, _2);
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_LONG(_2, 3);
			zephir_array_fast_append(_1, _2);
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_LONG(_2, 4);
			zephir_array_fast_append(_1, _2);
			zephir_update_property_this(this_ptr, SL("someArray"), _1 TSRMLS_CC);
		}
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("someEmptyArray"), PH_NOISY_CC);
		if (Z_TYPE_P(_7) == IS_NULL) {
			ZEPHIR_INIT_NVAR(_2);
			array_init(_2);
			zephir_update_property_this(this_ptr, SL("someEmptyArray"), _2 TSRMLS_CC);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJVAL_P(this_ptr);
	}

}

