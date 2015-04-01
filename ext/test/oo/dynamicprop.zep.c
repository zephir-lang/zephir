
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
#include "kernel/operators.h"


/**
 *
 */
ZEPHIR_INIT_CLASS(Test_Oo_DynamicProp) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, DynamicProp, test, oo_dynamicprop, test_oo_dynamicprop_method_entry, 0);

	zend_declare_property_null(test_oo_dynamicprop_ce, SL("test"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyInt) {

	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 10);
	zephir_update_property_zval_zval(this_ptr, property, _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyBoolTrue) {

	zval *property_param = NULL;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	zephir_update_property_zval_zval(this_ptr, property, ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyBoolFalse) {

	zval *property_param = NULL;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	zephir_update_property_zval_zval(this_ptr, property, ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyString) {

	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "string", 1);
	zephir_update_property_zval_zval(this_ptr, property, _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyChar) {

	char a = 'A';
	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_update_property_zval_zval(this_ptr, property, _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyUChar) {

	unsigned char a = 'A';
	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_update_property_zval_zval(this_ptr, property, _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyNull) {

	zval *property_param = NULL;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	zephir_update_property_zval_zval(this_ptr, property, ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableInt) {

	int a = 10;
	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_update_property_zval_zval(this_ptr, property, _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableString) {

	zval *property_param = NULL, *a;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}
	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "test", 1);


	zephir_update_property_zval_zval(this_ptr, property, a TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableBoolTrue) {

	zend_bool a = 1;
	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, a);
	zephir_update_property_zval_zval(this_ptr, property, _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableBoolFalse) {

	zend_bool a = 0;
	zval *property_param = NULL, *_0;
	zval *property = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (unlikely(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(property, property_param);
	} else {
		ZEPHIR_INIT_VAR(property);
		ZVAL_EMPTY_STRING(property);
	}


	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, a);
	zephir_update_property_zval_zval(this_ptr, property, _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setExistingStringProperty) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "works", 1);
	zephir_update_property_zval(this_ptr, SL("test"),  _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setExistingStringPropertyString) {

	zval *value_param = NULL;
	zval *value = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value_param);

	if (unlikely(Z_TYPE_P(value_param) != IS_STRING && Z_TYPE_P(value_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'value' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (likely(Z_TYPE_P(value_param) == IS_STRING)) {
		zephir_get_strval(value, value_param);
	} else {
		ZEPHIR_INIT_VAR(value);
		ZVAL_EMPTY_STRING(value);
	}


	zephir_update_property_zval(this_ptr, SL("eproperty"), value TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setNonExistingStringProperty) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "works", 1);
	zephir_update_property_zval(this_ptr, SL("property"),  _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

