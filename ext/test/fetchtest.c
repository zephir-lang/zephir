
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
 * Fetch statement tests
 */
ZEPHIR_INIT_CLASS(Test_FetchTest) {

	ZEPHIR_REGISTER_CLASS(Test, FetchTest, test, fetchtest, test_fetchtest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_FetchTest, testFetchArray1) {

	zval *a, *b, *c;

	zephir_fetch_params(0, 2, 0, &a, &b);



	RETURN_BOOL(zephir_array_isset_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray2) {

	int b;
	zval *a, *b_param = NULL, *c;

	zephir_fetch_params(0, 2, 0, &a, &b_param);

	b = zephir_get_intval(b_param);


	RETURN_BOOL(zephir_array_isset_long_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchArray3) {

	zval *b = NULL;
	zval *a, *b_param = NULL, *c;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b_param);

	zephir_get_strval(b, b_param);


	RETURN_MM_BOOL(zephir_array_isset_fetch(&c, a, b, 1 TSRMLS_CC));

}

PHP_METHOD(Test_FetchTest, testFetchObject1) {

	zval *a, *b, *c;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_OBS_VAR(c);
	RETURN_MM_BOOL(zephir_fetch_property_zval(&c, a, b, PH_SILENT_CC));

}

