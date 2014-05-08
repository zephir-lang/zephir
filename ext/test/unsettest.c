
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
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Unset statement tests
 */
ZEPHIR_INIT_CLASS(Test_Unsettest) {

	ZEPHIR_REGISTER_CLASS(Test, Unsettest, test, unsettest, test_unsettest_method_entry, 0);

	zend_declare_property_null(test_unsettest_ce, SL("property"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Unsettest, has) {

	zval *key, *_0;

	zephir_fetch_params(0, 1, 0, &key);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("property"), PH_NOISY_CC);
	RETURN_BOOL(zephir_array_isset(_0, key));

}

PHP_METHOD(Test_Unsettest, addValueToProperty) {

	zval *key, *value;

	zephir_fetch_params(0, 2, 0, &key, &value);



	zephir_update_property_array(this_ptr, SL("property"), key, value TSRMLS_CC);

}

PHP_METHOD(Test_Unsettest, testUnsetValueFromProperty) {

	zval *key, *_0;

	zephir_fetch_params(0, 1, 0, &key);



	_0 = zephir_fetch_nproperty_this(this_ptr, SL("property"), PH_NOISY_CC);
	zephir_array_unset(&_0, key, PH_SEPARATE);

}

PHP_METHOD(Test_Unsettest, testUnsetFromArray) {

	zval *arrayParameter;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &arrayParameter);

	ZEPHIR_SEPARATE_PARAM(arrayParameter);


	zephir_array_unset_long(&arrayParameter, 0, PH_SEPARATE);
	RETURN_CCTOR(arrayParameter);

}

PHP_METHOD(Test_Unsettest, testUnsetFromArrayByIndexVar) {

	zval *arrayParameter, *index;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &arrayParameter, &index);

	ZEPHIR_SEPARATE_PARAM(arrayParameter);


	zephir_array_unset(&arrayParameter, index, PH_SEPARATE);
	RETURN_CCTOR(arrayParameter);

}

PHP_METHOD(Test_Unsettest, testUnsetTypedArray) {

	zval *key = NULL;
	zval *arr_param = NULL, *key_param = NULL;
	zval *arr = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &arr_param, &key_param);

	zephir_get_arrval(arr, arr_param);
	ZEPHIR_SEPARATE_PARAM(arr);
	zephir_get_strval(key, key_param);


	zephir_array_unset(&arr, key, PH_SEPARATE);
	RETURN_CTOR(arr);

}

