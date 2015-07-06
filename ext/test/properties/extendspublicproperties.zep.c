
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


	test_properties_extendspublicproperties_ce->create_object = zephir_init_properties_Test_Properties_ExtendsPublicProperties;
	return SUCCESS;

}

static zend_object_value zephir_init_properties_Test_Properties_ExtendsPublicProperties(zend_class_entry *class_type TSRMLS_DC) {

		zval *_1;
		zval *_0, *_2;

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
		ZEPHIR_MM_RESTORE();
		return Z_OBJVAL_P(this_ptr);
	}

}

