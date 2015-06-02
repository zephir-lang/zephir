
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

ZEPHIR_INIT_CLASS(Test_Properties_ExtendsPublicProperties) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Properties, ExtendsPublicProperties, test, properties_extendspublicproperties, test_properties_publicproperties_ce, test_properties_extendspublicproperties_method_entry, 0);

	zend_declare_property_long(test_properties_extendspublicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_null(test_properties_extendspublicproperties_ce, SL("someGetterSetterArray"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_ExtendsPublicProperties, __construct) {

	zval *_1;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "holy", 1);
	zephir_array_fast_append(_0, _1);
	zephir_update_property_this(this_ptr, SL("someGetterSetterArray"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

