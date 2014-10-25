
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
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Test_Typeoff) {

	ZEPHIR_REGISTER_CLASS(Test, Typeoff, test, typeoff, test_typeoff_method_entry, 0);

	zend_declare_property_null(test_typeoff_ce, SL("property"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Typeoff, testNativeStringFalse) {

	zval *testVar;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(testVar);
	ZVAL_STRING(testVar, "sdfsdf", 1);
	RETURN_MM_BOOL(1 == 0);

}

PHP_METHOD(Test_Typeoff, testNativeStringTrue) {

	zval *testVar;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(testVar);
	ZVAL_STRING(testVar, "sdfsdf", 1);
	RETURN_MM_BOOL(1 == 1);

}

PHP_METHOD(Test_Typeoff, testNativeIntFalse) {

	int testVar;


	testVar = 12345;
	RETURN_BOOL(1 == 0);

}

PHP_METHOD(Test_Typeoff, testNativeIntTrue) {

	int testVar;


	testVar = 12345;
	RETURN_BOOL(1 == 1);

}

PHP_METHOD(Test_Typeoff, testNativeDoubleTrue) {

	double testVar;


	testVar = (double) (12345);
	RETURN_BOOL(1 == 1);

}

PHP_METHOD(Test_Typeoff, testNativeBoolTrue) {

	zend_bool testVar;


	testVar = 1;
	RETURN_BOOL(1 == 1);

}

PHP_METHOD(Test_Typeoff, testNativeBoolFalse) {

	zend_bool testVar;


	testVar = 0;
	RETURN_BOOL(1 == 0);

}

PHP_METHOD(Test_Typeoff, testArrayFalse) {

	zval *testVar;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(testVar);
	array_init(testVar);
	RETURN_MM_BOOL(Z_TYPE_P(testVar) == IS_STRING);

}

PHP_METHOD(Test_Typeoff, testArrayTrue) {

	zval *testVar;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(testVar);
	array_init(testVar);
	RETURN_MM_BOOL(Z_TYPE_P(testVar) == IS_ARRAY);

}

PHP_METHOD(Test_Typeoff, testClassPropertyAccess) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_STRING(_0, "test string", 1);
	zephir_update_property_this(this_ptr, SL("property"), _0 TSRMLS_CC);
	ZEPHIR_OBS_VAR(_1);
	zephir_read_property_this(&_1, this_ptr, SL("property"), PH_NOISY_CC);
	RETURN_MM_BOOL(Z_TYPE_P(_1) == IS_STRING);

}

PHP_METHOD(Test_Typeoff, testUnknownTypeOf) {

	zval *u, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &u);



	ZEPHIR_INIT_VAR(_0);
	zephir_gettype(_0, u TSRMLS_CC);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Typeoff, testCallableTypeOf) {

	zval *cb;

	zephir_fetch_params(0, 1, 0, &cb);



	RETURN_BOOL(zephir_is_callable(cb TSRMLS_CC) == 1);

}

