
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

ZEPHIR_INIT_CLASS(Test_IssetTest) {

	ZEPHIR_REGISTER_CLASS(Test, IssetTest, test, issettest, test_issettest_method_entry, 0);

	zend_declare_property_null(test_issettest_ce, SL("s"), ZEND_ACC_PUBLIC TSRMLS_CC);

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

PHP_METHOD(Test_IssetTest, testIssetDynamicProperty1) {

	zval *g;

	ZEPHIR_MM_GROW();

	ZEPHIR_OBS_VAR(g);
	zephir_read_property(&g, this_ptr, SL("s"), PH_NOISY_CC);
	if (zephir_array_isset_string(g, SS("a"))) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_IssetTest, testIssetDynamicProperty2) {

	zval *inp, *g;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &inp);



	ZEPHIR_OBS_VAR(g);
	zephir_read_property(&g, inp, SL("s"), PH_NOISY_CC);
	if (zephir_array_isset_string(g, SS("a"))) {
		RETURN_MM_BOOL(1);
	}
	RETURN_MM_BOOL(0);

}

PHP_METHOD(Test_IssetTest, __construct) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(_0, SS("a"), SL("true"), 1);
	zephir_update_property_this(this_ptr, SL("s"), _0 TSRMLS_CC);
	ZEPHIR_MM_RESTORE();

}

