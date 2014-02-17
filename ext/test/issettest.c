
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
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/object.h"


/**
 * Unset statement tests
 */
ZEPHIR_INIT_CLASS(Test_IssetTest) {

	ZEPHIR_REGISTER_CLASS(Test, IssetTest, test, issettest, test_issettest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_IssetTest, testIssetArray1) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	RETURN_BOOL(zephir_array_isset(a, b));

}

PHP_METHOD(Test_IssetTest, testIssetArray2) {

	int b;
	zval *a, *b_param = NULL;

	zephir_fetch_params(0, 2, 0, &a, &b_param);

	b = zephir_get_intval(b_param);


	RETURN_BOOL(zephir_array_isset_long(a, b));

}

PHP_METHOD(Test_IssetTest, testIssetArray3) {

	zval *b = NULL;
	zval *a, *b_param = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b_param);

	zephir_get_strval(b, b_param);


	RETURN_MM_BOOL(zephir_array_isset(a, b));

}

PHP_METHOD(Test_IssetTest, testIssetArray4) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(zephir_array_isset_long(a, 0));

}

PHP_METHOD(Test_IssetTest, testIssetArray5) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(zephir_array_isset_string(a, SS("a")));

}

PHP_METHOD(Test_IssetTest, testIssetProperty1) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL(zephir_isset_property(a, SS("b") TSRMLS_CC));

}

PHP_METHOD(Test_IssetTest, testIssetProperty2) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	RETURN_BOOL(zephir_isset_property_zval(a, b TSRMLS_CC));

}

PHP_METHOD(Test_IssetTest, testIssetProperty3) {

	zval *a;

	zephir_fetch_params(0, 1, 0, &a);



	RETURN_BOOL((0 == 0));

}

