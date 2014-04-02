
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


/**
 *
 */
ZEPHIR_INIT_CLASS(Test_Oo_DynamicProp) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, DynamicProp, test, oo_dynamicprop, test_oo_dynamicprop_method_entry, 0);

	zend_declare_property_null(test_oo_dynamicprop_ce, SL("test"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_DynamicProp, setProperty) {

	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	zephir_get_strval(property, property_param);


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 10);
	zephir_update_property_this(this_ptr, Z_STRVAL_P(property), Z_STRLEN_P(property), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

