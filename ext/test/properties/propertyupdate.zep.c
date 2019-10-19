
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


ZEPHIR_INIT_CLASS(Test_Properties_PropertyUpdate) {

	ZEPHIR_REGISTER_CLASS(Test\\Properties, PropertyUpdate, test, properties_propertyupdate, test_properties_propertyupdate_method_entry, 0);

	zend_declare_property_null(test_properties_propertyupdate_ce, SL("p1"), ZEND_ACC_PUBLIC);

	return SUCCESS;

}

PHP_METHOD(Test_Properties_PropertyUpdate, update1) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "aaa");
	zephir_update_property_array_append(this_ptr, SL("p1"), &_0);
	ZEPHIR_MM_RESTORE();

}

