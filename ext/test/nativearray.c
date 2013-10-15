
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
#include "kernel/array.h"
#include "kernel/operators.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_NativeArray) {

	ZEPHIR_REGISTER_CLASS(Test, NativeArray, test, nativearray, test_nativearray_method_entry, 0);


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

	zval *a, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray3) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, 1.1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, 2.2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, 3.3);
	zephir_array_fast_append(a, _0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray4) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_false));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_true));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_false));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray5) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_null));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_null));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_null));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray6) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "x", 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "y", 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_STRING(_0, "z", 1);
	zephir_array_fast_append(a, _0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray7) {

	int a, b, c;
	zval *d, *_0;

	ZEPHIR_MM_GROW();

	a = 1;
	b = 2;
	c = 3;
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, c);
	zephir_array_fast_append(d, _0);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray8) {

	double a, b, c;
	zval *d, *_0;

	ZEPHIR_MM_GROW();

	a = (double) (1);
	b = (double) (2);
	c = (double) (3);
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, a);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, c);
	zephir_array_fast_append(d, _0);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray9) {

	zend_bool a, b, c;
	zval *d, *_0;

	ZEPHIR_MM_GROW();

	a = 1;
	b = 0;
	c = 1;
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, a);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_BOOL(_0, c);
	zephir_array_fast_append(d, _0);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray10) {

	zval *a, *b, *c;
	zval *d;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "hello1", 1);
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello2", 1);
	ZEPHIR_INIT_VAR(c);
	ZVAL_STRING(c, "hello3", 1);
	ZEPHIR_INIT_VAR(d);
	array_init(d);
	zephir_array_fast_append(d, a);
	zephir_array_fast_append(d, b);
	zephir_array_fast_append(d, c);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray11) {

	zval *a, *b, *c, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_VAR(b);
	array_init(b);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 5);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 6);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_VAR(c);
	array_init(c);
	zephir_array_fast_append(c, a);
	zephir_array_fast_append(c, b);
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_NativeArray, testArray12) {

	zval *a, *b, *c, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_VAR(b);
	array_init(b);
	zephir_array_fast_append(b, a);
	ZEPHIR_INIT_VAR(c);
	array_init(c);
	zephir_array_fast_append(c, b);
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
	zephir_array_update_string(&a, SL("hello1"), &ZEPHIR_GLOBAL(global_true), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello2"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello3"), &ZEPHIR_GLOBAL(global_true), PH_COPY | PH_SEPARATE);
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
	zephir_array_update_string(&a, SL("hello1"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello2"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello3"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
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
	zephir_array_update_long(&a, 0, &ZEPHIR_GLOBAL(global_true), PH_COPY);
	add_index_bool(a, 1, 0);
	zephir_array_update_long(&a, 1, &ZEPHIR_GLOBAL(global_false), PH_COPY);
	add_index_bool(a, 2, 1);
	zephir_array_update_long(&a, 2, &ZEPHIR_GLOBAL(global_true), PH_COPY);
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
	zephir_array_update_long(&a, 0, &ZEPHIR_GLOBAL(global_null), PH_COPY);
	zephir_array_update_long(&a, 1, &ZEPHIR_GLOBAL(global_null), PH_COPY);
	zephir_array_update_long(&a, 2, &ZEPHIR_GLOBAL(global_null), PH_COPY);
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
	zval *a, *_0;

	ZEPHIR_MM_GROW();

	b = 0;
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray24) {

	double b;
	zval *a, *_0;

	ZEPHIR_MM_GROW();

	b = 0.0;
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray25) {

	zend_bool b;
	zval *a, *_0;

	ZEPHIR_MM_GROW();

	b = 0;
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray26) {

	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_NULL(b);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_long(&a, 0, &b, PH_COPY);
	zephir_array_update_long(&a, 1, &b, PH_COPY);
	zephir_array_update_long(&a, 2, &b, PH_COPY);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray27) {

	zval *b;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_long(&a, 0, &b, PH_COPY);
	zephir_array_update_long(&a, 1, &b, PH_COPY);
	zephir_array_update_long(&a, 2, &b, PH_COPY);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray28) {

	zval *b;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_long(&a, 0, &b, PH_COPY);
	zephir_array_update_long(&a, 1, &b, PH_COPY);
	zephir_array_update_long(&a, 2, &b, PH_COPY);
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

	zval *b;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_stringl_ex(a, Z_STRVAL_P(b), Z_STRLEN_P(b) + 1, SL("hello"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayAccess1) {

	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&b, a, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess2) {

	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	zephir_array_fetch_string(&b, a, SL("a"), PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess3) {

	long c;
	zval *a, *b, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	c = 0;
	zephir_array_fetch_long(&b, a, c, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess4) {

	zval *c;
	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	ZEPHIR_INIT_VAR(c);
	ZVAL_STRING(c, "a", 1);
	zephir_array_fetch(&b, a, c, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

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
	zephir_array_fetch(&b, a, c, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess6) {

	zval *a, b, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	zephir_array_fetch_string(&_0, a, SL("a"), PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_string(&_1, a, SL("b"), PH_NOISY | PH_READONLY TSRMLS_CC);
	ZEPHIR_SINIT_VAR(b);
	zephir_add_function(&b, _0, _1 TSRMLS_CC);
	RETURN_LCTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess1) {

	zval *a, *b, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	add_index_stringl(_0, 0, SL("a"), 1);
	add_index_stringl(_0, 1, SL("b"), 1);
	zephir_array_update_long(&a, 0, &_0, PH_COPY);
	zephir_array_fetch_long(&_1, a, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_long(&b, _1, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess2) {

	zval *a, *b, *_0, *_1, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "a", 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "b", 1);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&_2, a, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_long(&b, _2, 1, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess3) {

	zval *a, *b, *_0, *_1, *_2, *_3, *_4;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	ZEPHIR_INIT_VAR(_1);
	array_init(_1);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "a", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "b", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&_3, a, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_long(&_4, _3, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_long(&b, _4, 1, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess4) {

	zval *a, *b, *_0, *_1, *_2, *_3, *_4;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	ZEPHIR_INIT_VAR(_1);
	array_init(_1);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "a", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_BNVAR(_2);
	ZVAL_STRING(_2, "b", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&_3, a, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_long(&_4, _3, 0, PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_long(&b, _4, 1, PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess5) {

	zval *a, *b, *_0, *_1, *_2, *_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	ZEPHIR_INIT_VAR(_1);
	array_init(_1);
	add_assoc_long_ex(_1, SS("c"), 0);
	add_assoc_long_ex(_1, SS("d"), 1);
	zephir_array_update_string(&_0, SL("b"), &_1, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("a"), &_0, PH_COPY | PH_SEPARATE);
	zephir_array_fetch_string(&_2, a, SL("a"), PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_string(&_3, _2, SL("b"), PH_NOISY | PH_READONLY TSRMLS_CC);
	zephir_array_fetch_string(&b, _3, SL("c"), PH_NOISY | PH_READONLY TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate1) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate2) {

	int b;
	zval *a, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	b = 0;
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, b, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate3) {

	int b;
	zval *a, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	b = 0;
	ZEPHIR_INIT_BNVAR(_0);
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

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate1) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("ss"), 4, SL("x"), SL("y"));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate2) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("sss"), 6, SL("x"), SL("y"), SL("z"));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate3) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("ll"), 2, 0, 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate4) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("lll"), 3, 0, 1, 2);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate5) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("sls"), 5, SL("a"), 1, SL("c"));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate6) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("lsl"), 4, 0, SL("b"), 2);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate7) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("sls"), 5, SL("a"), 1, SL("c"));
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("lsl"), 4, 0, SL("b"), 2);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate8) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("sl"), 3, SL("a"), 0);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("sl"), 3, SL("a"), 1);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("sl"), 3, SL("a"), 2);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate9) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("sl"), 3, SL("a"), 0);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("sl"), 3, SL("a"), 1);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("sl"), 3, SL("a"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate10) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("sl"), 3, SL("a"), 0);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("sl"), 3, SL("a"), 1);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("sls"), 5, SL("a"), 1, SL("c"));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate11) {

	zval *a, *b, *c;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "y", 1);
	ZEPHIR_INIT_VAR(c);
	ZVAL_STRING(c, "x", 1);

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("zz"), 2, b, c);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate12) {

	zval *a, *b, *c, *d;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "y", 1);
	ZEPHIR_INIT_VAR(c);
	ZVAL_STRING(c, "x", 1);
	ZEPHIR_INIT_VAR(d);
	ZVAL_STRING(d, "z", 1);

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("zzz"), 3, b, c, d);
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

