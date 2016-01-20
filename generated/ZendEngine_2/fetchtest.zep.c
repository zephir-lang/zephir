
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/operators.h"


/**
 * Fetch statement tests
 */
ZEPHIR_INIT_CLASS(Test_FetchTest) {

	ZEPHIR_REGISTER_CLASS(Test, FetchTest, test, fetchtest, test_fetchtest_method_entry, 0);

	zend_declare_property_null(test_fetchtest_ce, SL("values"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_FetchTest, setValues) {

	zval *values;

	zephir_fetch_params(0, 1, 0, &values);



	zephir_update_property_this(this_ptr, SL("values"), values TSRMLS_CC);

}

PHP_METHOD(Test_FetchTest, getValues) {

	

	RETURN_MEMBER(this_ptr, "values");

}

PHP_METHOD(Test_FetchTest, testFetchArray1) {

	zval *a, *b, *c = NULL;

	zephir_fetch_params(0, 2, 0, &a, &b);



	RETURN_BOOL(zephir_array_isset_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray2) {

	int b;
	zval *a, *b_param = NULL, *c = NULL;

	zephir_fetch_params(0, 2, 0, &a, &b_param);

	b = zephir_get_intval(b_param);


	RETURN_BOOL(zephir_array_isset_long_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray3) {

	zval *b = NULL;
	zval *a, *b_param = NULL, *c = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b_param);

	zephir_get_strval(b, b_param);


	RETURN_MM_BOOL(zephir_array_isset_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchObject1) {

	zval *a, *b, *c = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_OBS_VAR(c);
	RETURN_MM_BOOL(zephir_fetch_property_zval(&c, a, b, PH_SILENT_CC));

}

PHP_METHOD(Test_FetchTest, testFetchPost) {

	zval *b, *_POST, *c = NULL;

	ZEPHIR_MM_GROW();
	zephir_get_global(&_POST, SS("_POST") TSRMLS_CC);
	zephir_fetch_params(0, 1, 0, &b);



	if (!(zephir_array_isset_fetch(&c, _POST, b, 1 TSRMLS_CC))) {
		RETURN_MM_BOOL(0);
	}
	RETURN_CTOR(c);

}

PHP_METHOD(Test_FetchTest, hasValue) {

	zval *name_param = NULL, *_POST, *_0$$4;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_get_global(&_POST, SS("_POST") TSRMLS_CC);
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(name, name_param);


	if (zephir_array_isset(_POST, name)) {
		RETURN_MM_BOOL(1);
	} else {
		_0$$4 = zephir_fetch_nproperty_this(this_ptr, SL("values"), PH_NOISY_CC);
		if (zephir_array_isset(_0$$4, name)) {
			RETURN_MM_BOOL(1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_FetchTest, getValue) {

	zval *name_param = NULL, *_POST, *value = NULL, *_0$$3;
	zval *name = NULL;

	ZEPHIR_MM_GROW();
	zephir_get_global(&_POST, SS("_POST") TSRMLS_CC);
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(name, name_param);


	ZEPHIR_OBS_VAR(value);
	if (!(zephir_array_isset_fetch(&value, _POST, name, 0 TSRMLS_CC))) {
		ZEPHIR_OBS_NVAR(value);
		_0$$3 = zephir_fetch_nproperty_this(this_ptr, SL("values"), PH_NOISY_CC);
		if (!(zephir_array_isset_fetch(&value, _0$$3, name, 0 TSRMLS_CC))) {
			RETURN_MM_NULL();
		}
	}
	RETURN_CCTOR(value);

}

