
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Unset statement tests
 */
ZEPHIR_INIT_CLASS(Stub_Unsettest) {

	ZEPHIR_REGISTER_CLASS(Stub, Unsettest, stub, unsettest, stub_unsettest_method_entry, 0);

	zend_declare_property_null(stub_unsettest_ce, SL("property"), ZEND_ACC_PROTECTED);

	return SUCCESS;

}

PHP_METHOD(Stub_Unsettest, getProperty) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "property");

}

PHP_METHOD(Stub_Unsettest, has) {

	zval *key, key_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &key);



	zephir_read_property(&_0, this_ptr, ZEND_STRL("property"), PH_NOISY_CC | PH_READONLY);
	RETURN_BOOL(zephir_array_isset(&_0, key));

}

PHP_METHOD(Stub_Unsettest, addValueToProperty) {

	zval *key, key_sub, *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(key)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(2, 0, &key, &value);



	zephir_update_property_array(this_ptr, SL("property"), key, value);

}

PHP_METHOD(Stub_Unsettest, testUnsetValueFromProperty) {

	zval *key, key_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&key_sub);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(key)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &key);



	zephir_read_property(&_0, this_ptr, ZEND_STRL("property"), PH_NOISY_CC | PH_READONLY);
	zephir_array_unset(&_0, key, PH_SEPARATE);

}

PHP_METHOD(Stub_Unsettest, testUnsetFromArray) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *arrayParameter, arrayParameter_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arrayParameter_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(arrayParameter)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &arrayParameter);

	ZEPHIR_SEPARATE_PARAM(arrayParameter);


	zephir_array_unset_long(arrayParameter, 0, PH_SEPARATE);
	RETVAL_ZVAL(arrayParameter, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Stub_Unsettest, testUnsetFromArrayByIndexVar) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *arrayParameter, arrayParameter_sub, *index, index_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arrayParameter_sub);
	ZVAL_UNDEF(&index_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(arrayParameter)
		Z_PARAM_ZVAL(index)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &arrayParameter, &index);

	ZEPHIR_SEPARATE_PARAM(arrayParameter);


	zephir_array_unset(arrayParameter, index, PH_SEPARATE);
	RETVAL_ZVAL(arrayParameter, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Stub_Unsettest, testUnsetProperty) {

	zval *this_ptr = getThis();



	zephir_unset_property(this_ptr, "property");
	RETURN_MEMBER(getThis(), "property");

}

PHP_METHOD(Stub_Unsettest, testStdClassUnset) {

	zval simpleObject, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&simpleObject);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&simpleObject);
	object_init(&simpleObject);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 12345);
	zephir_update_property_zval(&simpleObject, ZEND_STRL("property1"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "test");
	zephir_update_property_zval(&simpleObject, ZEND_STRL("property2"), &_1);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 12345);
	zephir_update_property_zval(&simpleObject, ZEND_STRL("property3"), &_0);
	zephir_unset_property(&simpleObject, "property2");
	RETURN_CCTOR(&simpleObject);

}

PHP_METHOD(Stub_Unsettest, testUnsetTypedArray) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval key;
	zval *arr_param = NULL, *key_param = NULL;
	zval arr;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&key);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ARRAY(arr)
		Z_PARAM_STR(key)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &arr_param, &key_param);

	zephir_get_arrval(&arr, arr_param);
	zephir_get_strval(&key, key_param);


	zephir_array_unset(&arr, &key, PH_SEPARATE);
	RETURN_CTOR(&arr);

}

