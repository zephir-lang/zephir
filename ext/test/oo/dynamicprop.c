
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


/**
 *
 */
ZEPHIR_INIT_CLASS(Test_Oo_DynamicProp) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, DynamicProp, test, oo_dynamicprop, test_oo_dynamicprop_method_entry, 0);

	zend_declare_property_null(test_oo_dynamicprop_ce, SL("test"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyInt) {

	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 10);
	zephir_update_property_this(this_ptr, SL("property"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyBool) {

	zval *property_param = NULL;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	zephir_update_property_this(this_ptr, SL("property"), ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyString) {

	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "string", 1);
	zephir_update_property_this(this_ptr, SL("property"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyChar) {

	char a = 'A';
	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_update_property_zval(this_ptr, SL("property"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyUChar) {

	unsigned char a = 'A';
	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_update_property_zval(this_ptr, SL("property"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, getProperty) {

	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(property_param) == IS_STRING) {
		property = property_param;
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_OBS_VAR(_0);
	zephir_read_property_zval(&_0, this_ptr, property, PH_NOISY_CC);
	RETURN_CCTOR(_0);

}

