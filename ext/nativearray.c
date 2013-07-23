
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/array.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_NativeArray) {

	ZEPHIR_REGISTER_CLASS(Test, NativeArray, nativearray, test_nativearray_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_NativeArray, testArray1) {

	zval *a;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	RETURN_CCTOR(a);


}

PHP_METHOD(Test_NativeArray, testArray2) {

	zval *a, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_append(&a, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, 3);
	zephir_array_append(&a, _2, 0);

	RETURN_CCTOR(a);


}

PHP_METHOD(Test_NativeArray, testArray3) {

	zval *a, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, 1.1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_DOUBLE(_1, 2.2);
	zephir_array_append(&a, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_DOUBLE(_2, 3.3);
	zephir_array_append(&a, _2, 0);

	RETURN_CCTOR(a);


}

PHP_METHOD(Test_NativeArray, testArray4) {

	zval *a, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, 0);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_BOOL(_1, 1);
	zephir_array_append(&a, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_BOOL(_2, 0);
	zephir_array_append(&a, _2, 0);

	RETURN_CCTOR(a);


}

