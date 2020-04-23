
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

	zend_declare_property_null(test_oo_dynamicprop_ce, SL("test"), ZEND_ACC_PUBLIC);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyInt) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, _0;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 10);
	zephir_update_property_zval_zval(this_ptr, &property, &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyBoolTrue) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, __$true;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_BOOL(&__$true, 1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	zephir_update_property_zval_zval(this_ptr, &property, &__$true);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyBoolFalse) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, __$false;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_BOOL(&__$false, 0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	zephir_update_property_zval_zval(this_ptr, &property, &__$false);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyString) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, _0;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "string");
	zephir_update_property_zval_zval(this_ptr, &property, &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyChar) {

	char a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, _0;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	a = 'A';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property, &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyUChar) {

	unsigned char a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, _0;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	a = 'A';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property, &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyNull) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, __$null;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_NULL(&__$null);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	zephir_update_property_zval_zval(this_ptr, &property, &__$null);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableInt) {

	zend_long a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, _0;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	a = 10;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property, &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableString) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, a;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "test");
	zephir_update_property_zval_zval(this_ptr, &property, &a);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableBoolTrue) {

	zend_bool a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, _0;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	a = 1;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property, &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setPropertyVariableBoolFalse) {

	zend_bool a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *property_param = NULL, _0;
	zval property;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&property);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &property_param);

	if (UNEXPECTED(Z_TYPE_P(property_param) != IS_STRING && Z_TYPE_P(property_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'property' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(property_param) == IS_STRING)) {
		zephir_get_strval(&property, property_param);
	} else {
		ZEPHIR_INIT_VAR(&property);
		ZVAL_EMPTY_STRING(&property);
	}


	a = 0;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, a);
	zephir_update_property_zval_zval(this_ptr, &property, &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setExistingStringProperty) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "works");
	zephir_update_property_zval(this_ptr, ZEND_STRL("test"), &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setExistingStringPropertyString) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value_param = NULL;
	zval value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value_param);

	if (UNEXPECTED(Z_TYPE_P(value_param) != IS_STRING && Z_TYPE_P(value_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'value' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(value_param) == IS_STRING)) {
		zephir_get_strval(&value, value_param);
	} else {
		ZEPHIR_INIT_VAR(&value);
		ZVAL_EMPTY_STRING(&value);
	}


	zephir_update_property_zval(this_ptr, ZEND_STRL("eproperty"), &value);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Oo_DynamicProp, setNonExistingStringProperty) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "works");
	zephir_update_property_zval(this_ptr, ZEND_STRL("property"), &_0);
	ZEPHIR_MM_RESTORE();

}

