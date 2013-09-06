
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

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray3) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, 1.1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, 2.2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, 3.3);
	zephir_array_append(&a, _0, 0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray4) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, 0);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, 0);
	zephir_array_append(&a, _0, 0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray5) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_NULL(_0);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_NULL(_0);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_NULL(_0);
	zephir_array_append(&a, _0, 0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray6) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "x", 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "y", 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "z", 1);
	zephir_array_append(&a, _0, 0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray7) {

	int a, b, c;
	zval *d, *_0 = NULL;

	ZEPHIR_MM_GROW();

	a = 1;
	b = 2;
	c = 3;
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, c);
	zephir_array_append(&d, _0, 0);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray8) {

	double a, b, c;
	zval *d, *_0 = NULL;

	ZEPHIR_MM_GROW();

	a = (double) (1);
	b = (double) (2);
	c = (double) (3);
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, a);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, c);
	zephir_array_append(&d, _0, 0);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray9) {

	zend_bool a, b, c;
	zval *d, *_0 = NULL;

	ZEPHIR_MM_GROW();

	a = 1;
	b = 0;
	c = 1;
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, a);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, c);
	zephir_array_append(&d, _0, 0);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray10) {

	zephir_str *a = NULL, *b = NULL, *c = NULL;
	zval *d, *_0 = NULL;

	ZEPHIR_MM_GROW();

	zephir_str_assign(a, "hello1", sizeof("hello1")-1);
	zephir_str_assign(b, "hello2", sizeof("hello2")-1);
	zephir_str_assign(c, "hello3", sizeof("hello3")-1);
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRINGL(_0, a->str, a->len, 1);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRINGL(_0, b->str, b->len, 1);
	zephir_array_append(&d, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRINGL(_0, c->str, c->len, 1);
	zephir_array_append(&d, _0, 0);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray11) {

	zval *a, *b, *c, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_VAR(b);
	array_init(b);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_append(&b, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 5);
	zephir_array_append(&b, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 6);
	zephir_array_append(&b, _0, 0);
	ZEPHIR_INIT_VAR(c);
	array_init(c);
	zephir_array_append(&c, a, 0);
	zephir_array_append(&c, b, 0);
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_NativeArray, testArray12) {

	zval *a, *b, *c, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_VAR(b);
	array_init(b);
	zephir_array_append(&b, a, 0);
	ZEPHIR_INIT_VAR(c);
	array_init(c);
	zephir_array_append(&c, b, 0);
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_NativeArray, testArray13) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_index_stringl(a, 1, SL("hello1"), 1);
	add_index_stringl(a, 2, SL("hello2"), 1);
	add_index_stringl(a, 3, SL("hello3"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray14) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("hello1"), 1);
	add_assoc_long_ex(a, SS("hello2"), 2);
	add_assoc_long_ex(a, SS("hello3"), 3);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray15) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_bool_ex(a, SS("hello1"), 1);
	add_assoc_bool_ex(a, SS("hello2"), 0);
	add_assoc_bool_ex(a, SS("hello3"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray16) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_double_ex(a, SS("hello1"), 1.0);
	add_assoc_double_ex(a, SS("hello2"), 2.0);
	add_assoc_double_ex(a, SS("hello3"), 3.0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray17) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_null_ex(a, SS(""));
	add_assoc_null_ex(a, SS(""));
	add_assoc_null_ex(a, SS(""));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray18) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_stringl_ex(a, SS("hello1"), SL("a"), 1);
	add_assoc_stringl_ex(a, SS("hello2"), SL("b"), 1);
	add_assoc_stringl_ex(a, SS("hello3"), SL("c"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray19) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_index_bool(a, 0, 1);
	add_index_bool(a, 1, 0);
	add_index_bool(a, 2, 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray20) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_index_double(a, 0, 1.0);
	add_index_double(a, 1, 2.0);
	add_index_double(a, 2, 3.0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray21) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_index_null(a, 0);
	add_index_null(a, 1);
	add_index_null(a, 2);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray22) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_index_long(a, 0, 4);
	add_index_long(a, 1, 5);
	add_index_long(a, 2, 6);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray23) {

	int b;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	b = 0;
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray24) {

	double b;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	b = 0.0;
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray25) {

	zend_bool b;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	b = 0;
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray26) {

	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_NULL(b);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_long(&a, 0, &b, PH_COPY | PH_SEPARATE);
	zephir_array_update_long(&a, 1, &b, PH_COPY | PH_SEPARATE);
	zephir_array_update_long(&a, 2, &b, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray27) {

	zephir_str *b = NULL;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	zephir_str_assign(b, "hello", sizeof("hello")-1);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRINGL(_0, b->str, b->len, 1);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRINGL(_0, b->str, b->len, 1);
	zephir_array_update_long(&a, 1, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRINGL(_0, b->str, b->len, 1);
	zephir_array_update_long(&a, 2, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray28) {

	zephir_str *b = NULL;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	zephir_str_assign(b, "hello", sizeof("hello")-1);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRINGL(_0, b->str, b->len, 1);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRINGL(_0, b->str, b->len, 1);
	zephir_array_update_long(&a, 1, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRINGL(_0, b->str, b->len, 1);
	zephir_array_update_long(&a, 2, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray29) {

	long b;
	zval *a;

	ZEPHIR_MM_GROW();

	b = 1;
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_index_stringl(a, b, SL("hello"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray30) {

	zephir_str *b = NULL;
	zval *a;

	ZEPHIR_MM_GROW();

	zephir_str_assign(b, "hello", sizeof("hello")-1);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_stringl_ex(a, b->str, b->len + 1, SL("hello"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayAccess1) {

	zval *a, *b, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_OBS_VAR(b);
	zephir_array_fetch_long(&b, a, 0, PH_NOISY);
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess2) {

	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	ZEPHIR_OBS_VAR(b);
	zephir_array_fetch_string(&b, a, SL("a"), PH_NOISY);
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess3) {

	long c;
	zval *a, *b, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	c = 0;
	ZEPHIR_OBS_VAR(b);
	zephir_array_fetch_long(&b, a, c, PH_NOISY);
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess4) {

	zephir_str *c = NULL;
	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	zephir_str_assign(c, "a", sizeof("a")-1);
	ZEPHIR_OBS_VAR(b);
	zephir_array_fetch_string(&b, a, c->str, c->len, PH_NOISY);
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess5) {

	zval *a, *b, *c;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	ZEPHIR_INIT_VAR(c);
	ZVAL_STRING(c, "a", 1);
	ZEPHIR_OBS_VAR(b);
	zephir_array_fetch(&b, a, c, PH_NOISY);
	RETURN_CCTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate1) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate2) {

	int b;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	b = 0;
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, b, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate3) {

	int b;
	zval *a, *_0 = NULL, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_append(&a, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_append(&a, _0, 0);
	b = 0;
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, b, &_0, PH_COPY | PH_SEPARATE);
	b = 1;
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 5);
	zephir_array_update_long(&a, b, &_1, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate4) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_string(&a, SL("a"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayWrongUpdate1) {

	zval *x, *y, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(x);
	array_init(x);
	ZEPHIR_INIT_VAR(y);
	ZVAL_STRING(y, "hello", 1);
	ZEPHIR_INIT_VAR(_0);
	object_init(_0);
	zephir_array_update_zval(&x, y, &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_MM_RESTORE();

}

