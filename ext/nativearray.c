
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
#include "kernel/string_type.h"


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

PHP_METHOD(Test_NativeArray, testArray5) {

	zval *a, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_NULL(_0);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_NULL(_1);
	zephir_array_append(&a, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_NULL(_2);
	zephir_array_append(&a, _2, 0);

	RETURN_CCTOR(a);


}

PHP_METHOD(Test_NativeArray, testArray6) {

	zval *a, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "x", 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "y", 1);
	zephir_array_append(&a, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "z", 1);
	zephir_array_append(&a, _2, 0);

	RETURN_CCTOR(a);


}

PHP_METHOD(Test_NativeArray, testArray7) {

	long a, b, c;
	zval *d, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	a = 1;

	b = 2;

	c = 3;

	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, b);
	zephir_array_append(&d, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, c);
	zephir_array_append(&d, _2, 0);

	RETURN_CCTOR(d);


}

PHP_METHOD(Test_NativeArray, testArray8) {

	double a, b, c;
	zval *d, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	a = (double) (1);

	b = (double) (2);

	c = (double) (3);

	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, a);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_DOUBLE(_1, b);
	zephir_array_append(&d, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_DOUBLE(_2, c);
	zephir_array_append(&d, _2, 0);

	RETURN_CCTOR(d);


}

PHP_METHOD(Test_NativeArray, testArray9) {

	zend_bool a, b, c;
	zval *d, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	a = 1;

	b = 0;

	c = 1;

	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, a);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_BOOL(_1, b);
	zephir_array_append(&d, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_BOOL(_2, c);
	zephir_array_append(&d, _2, 0);

	RETURN_CCTOR(d);


}

PHP_METHOD(Test_NativeArray, testArray10) {

	zephir_str *a = NULL, *b = NULL, *c = NULL;
	zval *d, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();
	zephir_str_assign(a, "hello1", sizeof("hello1")-1);

	zephir_str_assign(b, "hello2", sizeof("hello2")-1);

	zephir_str_assign(c, "hello3", sizeof("hello3")-1);

	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, a->str, 1);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, b->str, 1);
	zephir_array_append(&d, _1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, c->str, 1);
	zephir_array_append(&d, _2, 0);

	RETURN_CCTOR(d);


}

PHP_METHOD(Test_NativeArray, testArray11) {

	zval *a, *b, *c, *_0, *_1, *_2, *_3, *_4, *_5;

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

	ZEPHIR_INIT_VAR(b);
	array_init(b);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 4);
	zephir_array_append(&b, _3, 0);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_LONG(_4, 5);
	zephir_array_append(&b, _4, 0);
	ZEPHIR_INIT_VAR(_5);
	ZVAL_LONG(_5, 6);
	zephir_array_append(&b, _5, 0);

	ZEPHIR_INIT_VAR(c);
	array_init(c);
	zephir_array_append(&c, a, 0);
	zephir_array_append(&c, b, 0);

	RETURN_CCTOR(c);


}

PHP_METHOD(Test_NativeArray, testArray12) {

	zval *a, *b, *c, *_0, *_1, *_2;

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

	ZEPHIR_INIT_VAR(b);
	array_init(b);
	zephir_array_append(&b, a, 0);

	ZEPHIR_INIT_VAR(c);
	array_init(c);
	zephir_array_append(&c, b, 0);

	RETURN_CCTOR(c);


}

