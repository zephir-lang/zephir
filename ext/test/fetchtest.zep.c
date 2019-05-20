
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

	zval *values, values_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&values_sub);

	zephir_fetch_params(0, 1, 0, &values);



	zephir_update_property_zval(this_ptr, SL("values"), values);
	RETURN_MM_THIS();

}

PHP_METHOD(Test_FetchTest, getValues) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_MEMBER(getThis(), "values");

}

PHP_METHOD(Test_FetchTest, testFetchArray1) {

	zval *a, a_sub, *b, b_sub, c;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&c);

	zephir_fetch_params(0, 2, 0, &a, &b);



	RETURN_MM_BOOL(zephir_array_isset_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray2) {

	zval *a = NULL, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);

	ZEPHIR_SEPARATE_PARAM(a);


	RETURN_MM_BOOL(zephir_array_isset_fetch(a, a, b, 0 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray3) {

	zend_long b;
	zval *a, a_sub, *b_param = NULL, c;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&c);

	zephir_fetch_params(0, 2, 0, &a, &b_param);

	b = zephir_get_intval(b_param);


	RETURN_MM_BOOL(zephir_array_isset_long_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray4) {

	zend_long b;
	zval *a = NULL, a_sub, *b_param = NULL;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);

	zephir_fetch_params(1, 2, 0, &a, &b_param);

	ZEPHIR_SEPARATE_PARAM(a);
	b = zephir_get_intval(b_param);


	RETURN_MM_BOOL(zephir_array_isset_long_fetch(a, a, b, 0 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray5) {

	zval b;
	zval *a, a_sub, *b_param = NULL, c;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&b);

	zephir_fetch_params(1, 2, 0, &a, &b_param);

	zephir_get_strval(&b, b_param);


	RETURN_MM_BOOL(zephir_array_isset_fetch(&c, a, &b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray6) {

	zval b;
	zval *a = NULL, a_sub, *b_param = NULL;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b);

	zephir_fetch_params(1, 2, 0, &a, &b_param);

	ZEPHIR_SEPARATE_PARAM(a);
	zephir_get_strval(&b, b_param);


	RETURN_MM_BOOL(zephir_array_isset_fetch(a, a, &b, 0 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchObject1) {

	zval *a, a_sub, *b, b_sub, c;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&c);

	zephir_fetch_params(1, 2, 0, &a, &b);



	RETURN_MM_BOOL(zephir_fetch_property_zval(&c, a, b, PH_SILENT_CC));

}

PHP_METHOD(Test_FetchTest, testFetchObject2) {

	zval *a = NULL, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);

	ZEPHIR_SEPARATE_PARAM(a);


	RETURN_MM_BOOL(zephir_fetch_property_zval(a, a, b, PH_SILENT_CC));

}

PHP_METHOD(Test_FetchTest, testFetchPost) {

	zval *b, b_sub, _POST, c;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_POST);
	ZVAL_UNDEF(&c);

	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(0, 1, 0, &b);



	if (!(zephir_array_isset_fetch(&c, &_POST, b, 1 TSRMLS_CC))) {
		RETURN_MM_BOOL(0);
	}
	RETURN_MM_CTOR(&c);

}

PHP_METHOD(Test_FetchTest, hasValue) {

	zval *name_param = NULL, _POST, _0$$4;
	zval name;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_POST);
	ZVAL_UNDEF(&_0$$4);

	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(&name, name_param);


	if (zephir_array_isset(&_POST, &name)) {
		RETURN_MM_BOOL(1);
	} else {
		zephir_read_property(&_0$$4, this_ptr, SL("values"), PH_NOISY_CC | PH_READONLY);
		if (zephir_array_isset(&_0$$4, &name)) {
			RETURN_MM_BOOL(1);
		}
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_FetchTest, getValue) {

	zval *name_param = NULL, _POST, value, _0$$3;
	zval name;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_POST);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_0$$3);

	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(1, 1, 0, &name_param);

	zephir_get_strval(&name, name_param);


	if (!(zephir_array_isset_fetch(&value, &_POST, &name, 0 TSRMLS_CC))) {
		zephir_read_property(&_0$$3, this_ptr, SL("values"), PH_NOISY_CC | PH_READONLY);
		if (!(zephir_array_isset_fetch(&value, &_0$$3, &name, 0 TSRMLS_CC))) {
			RETURN_MM_NULL();
		}
	}
	RETURN_MM_CTOR(&value);

}

