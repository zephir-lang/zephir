
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
#include "kernel/string.h"
#include "kernel/fcall.h"


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

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray3) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, 1.1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, 2.2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, 3.3);
	zephir_array_fast_append(a, _0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray4) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_false));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_true));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_false));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray5) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_null));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_null));
	zephir_array_fast_append(a, ZEPHIR_GLOBAL(global_null));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray6) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "x", 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "y", 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "z", 1);
	zephir_array_fast_append(a, _0);
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
	zephir_create_array(d, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, a);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, c);
	zephir_array_fast_append(d, _0);
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
	zephir_create_array(d, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, a);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, c);
	zephir_array_fast_append(d, _0);
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
	zephir_create_array(d, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, a);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_fast_append(d, _0);
	ZEPHIR_INIT_NVAR(_0);
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
	zephir_create_array(d, 3, 0 TSRMLS_CC);
	zephir_array_fast_append(d, a);
	zephir_array_fast_append(d, b);
	zephir_array_fast_append(d, c);
	RETURN_CCTOR(d);

}

PHP_METHOD(Test_NativeArray, testArray11) {

	zval *a, *b, *c, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_VAR(b);
	zephir_create_array(b, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 5);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 6);
	zephir_array_fast_append(b, _0);
	ZEPHIR_INIT_VAR(c);
	zephir_create_array(c, 2, 0 TSRMLS_CC);
	zephir_array_fast_append(c, a);
	zephir_array_fast_append(c, b);
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_NativeArray, testArray12) {

	zval *a, *b, *c, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_VAR(b);
	zephir_create_array(b, 1, 0 TSRMLS_CC);
	zephir_array_fast_append(b, a);
	ZEPHIR_INIT_VAR(c);
	zephir_create_array(c, 1, 0 TSRMLS_CC);
	zephir_array_fast_append(c, b);
	RETURN_CCTOR(c);

}

PHP_METHOD(Test_NativeArray, testArray13) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_index_stringl(a, 1, SL("hello1"), 1);
	add_index_stringl(a, 2, SL("hello2"), 1);
	add_index_stringl(a, 3, SL("hello3"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray14) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(a, SS("hello1"), 1);
	add_assoc_long_ex(a, SS("hello2"), 2);
	add_assoc_long_ex(a, SS("hello3"), 3);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray15) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_update_string(&a, SL("hello1"), &ZEPHIR_GLOBAL(global_true), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello2"), &ZEPHIR_GLOBAL(global_false), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello3"), &ZEPHIR_GLOBAL(global_true), PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray16) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_double_ex(a, SS("hello1"), 1.0);
	add_assoc_double_ex(a, SS("hello2"), 2.0);
	add_assoc_double_ex(a, SS("hello3"), 3.0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray17) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_update_string(&a, SL("hello1"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello2"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello3"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray18) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_stringl_ex(a, SS("hello1"), SL("a"), 1);
	add_assoc_stringl_ex(a, SS("hello2"), SL("b"), 1);
	add_assoc_stringl_ex(a, SS("hello3"), SL("c"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray19) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_index_bool(a, 0, 1);
	zephir_array_update_long(&a, 0, &ZEPHIR_GLOBAL(global_true), PH_COPY, "test/nativearray.zep", 147);
	add_index_bool(a, 1, 0);
	zephir_array_update_long(&a, 1, &ZEPHIR_GLOBAL(global_false), PH_COPY, "test/nativearray.zep", 147);
	add_index_bool(a, 2, 1);
	zephir_array_update_long(&a, 2, &ZEPHIR_GLOBAL(global_true), PH_COPY, "test/nativearray.zep", 147);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray20) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_index_double(a, 0, 1.0);
	add_index_double(a, 1, 2.0);
	add_index_double(a, 2, 3.0);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray21) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_update_long(&a, 0, &ZEPHIR_GLOBAL(global_null), PH_COPY, "test/nativearray.zep", 161);
	zephir_array_update_long(&a, 1, &ZEPHIR_GLOBAL(global_null), PH_COPY, "test/nativearray.zep", 161);
	zephir_array_update_long(&a, 2, &ZEPHIR_GLOBAL(global_null), PH_COPY, "test/nativearray.zep", 161);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray22) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
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
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY, "test/nativearray.zep", 177);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY, "test/nativearray.zep", 177);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY, "test/nativearray.zep", 177);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray24) {

	double b;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	b = 0.0;
	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY, "test/nativearray.zep", 186);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY, "test/nativearray.zep", 186);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY, "test/nativearray.zep", 186);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray25) {

	zend_bool b;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	b = 0;
	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY, "test/nativearray.zep", 195);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY, "test/nativearray.zep", 195);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY, "test/nativearray.zep", 195);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray26) {

	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_NULL(b);
	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_update_long(&a, 0, &b, PH_COPY, "test/nativearray.zep", 204);
	zephir_array_update_long(&a, 1, &b, PH_COPY, "test/nativearray.zep", 204);
	zephir_array_update_long(&a, 2, &b, PH_COPY, "test/nativearray.zep", 204);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray27) {

	zval *b;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_update_long(&a, 0, &b, PH_COPY, "test/nativearray.zep", 213);
	zephir_array_update_long(&a, 1, &b, PH_COPY, "test/nativearray.zep", 213);
	zephir_array_update_long(&a, 2, &b, PH_COPY, "test/nativearray.zep", 213);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray28) {

	zval *b;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	zephir_array_update_long(&a, 0, &b, PH_COPY, "test/nativearray.zep", 222);
	zephir_array_update_long(&a, 1, &b, PH_COPY, "test/nativearray.zep", 222);
	zephir_array_update_long(&a, 2, &b, PH_COPY, "test/nativearray.zep", 222);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArray29) {

	long b;
	zval *a;

	ZEPHIR_MM_GROW();

	b = 1;
	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 1, 0 TSRMLS_CC);
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
	zephir_create_array(a, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(a, Z_STRVAL_P(b), Z_STRLEN_P(b) + 1, SL("hello"), 1);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayAccess1) {

	zval *a, *b, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&b, a, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 248 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess2) {

	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	zephir_array_fetch_string(&b, a, SL("a"), PH_NOISY | PH_READONLY, "test/nativearray.zep", 258 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess3) {

	long c;
	zval *a, *b, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	c = 0;
	zephir_array_fetch_long(&b, a, c, PH_NOISY | PH_READONLY, "test/nativearray.zep", 270 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess4) {

	zval *c;
	zval *a, *b;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	ZEPHIR_INIT_VAR(c);
	ZVAL_STRING(c, "a", 1);
	zephir_array_fetch(&b, a, c, PH_NOISY | PH_READONLY, "test/nativearray.zep", 282 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess5) {

	zval *a, *b, *c;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	ZEPHIR_INIT_VAR(c);
	ZVAL_STRING(c, "a", 1);
	zephir_array_fetch(&b, a, c, PH_NOISY | PH_READONLY, "test/nativearray.zep", 293 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayAccess6) {

	zval *a, b, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	zephir_array_fetch_string(&_0, a, SL("a"), PH_NOISY | PH_READONLY, "test/nativearray.zep", 303 TSRMLS_CC);
	zephir_array_fetch_string(&_1, a, SL("b"), PH_NOISY | PH_READONLY, "test/nativearray.zep", 303 TSRMLS_CC);
	ZEPHIR_SINIT_VAR(b);
	zephir_add_function_ex(&b, _0, _1 TSRMLS_CC);
	RETURN_LCTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess1) {

	zval *_0;
	zval *a, *b, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 2, 0 TSRMLS_CC);
	add_index_stringl(_0, 0, SL("a"), 1);
	add_index_stringl(_0, 1, SL("b"), 1);
	zephir_array_update_long(&a, 0, &_0, PH_COPY, "test/nativearray.zep", 312);
	zephir_array_fetch_long(&_1, a, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 313 TSRMLS_CC);
	zephir_array_fetch_long(&b, _1, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 313 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess2) {

	zval *_0;
	zval *a, *b, *_1 = NULL, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 2, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_STRING(_1, "a", 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_STRING(_1, "b", 1);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&_2, a, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 323 TSRMLS_CC);
	zephir_array_fetch_long(&b, _2, 1, PH_NOISY | PH_READONLY, "test/nativearray.zep", 323 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess3) {

	zval *_0, *_1;
	zval *a, *b, *_2 = NULL, *_3, *_4;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_create_array(_1, 2, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "a", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_NVAR(_2);
	ZVAL_STRING(_2, "b", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&_3, a, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 333 TSRMLS_CC);
	zephir_array_fetch_long(&_4, _3, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 333 TSRMLS_CC);
	zephir_array_fetch_long(&b, _4, 1, PH_NOISY | PH_READONLY, "test/nativearray.zep", 333 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess4) {

	zval *_0, *_1;
	zval *a, *b, *_2 = NULL, *_3, *_4;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_create_array(_1, 2, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_STRING(_2, "a", 1);
	zephir_array_fast_append(_1, _2);
	ZEPHIR_INIT_NVAR(_2);
	ZVAL_STRING(_2, "b", 1);
	zephir_array_fast_append(_1, _2);
	zephir_array_fast_append(_0, _1);
	zephir_array_fast_append(a, _0);
	zephir_array_fetch_long(&_3, a, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 343 TSRMLS_CC);
	zephir_array_fetch_long(&_4, _3, 0, PH_NOISY | PH_READONLY, "test/nativearray.zep", 343 TSRMLS_CC);
	zephir_array_fetch_long(&b, _4, 1, PH_NOISY | PH_READONLY, "test/nativearray.zep", 343 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayMultipleAccess5) {

	zval *_0, *_1;
	zval *a, *b, *_2, *_3;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_create_array(_1, 2, 0 TSRMLS_CC);
	add_assoc_long_ex(_1, SS("c"), 0);
	add_assoc_long_ex(_1, SS("d"), 1);
	zephir_array_update_string(&_0, SL("b"), &_1, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("a"), &_0, PH_COPY | PH_SEPARATE);
	zephir_array_fetch_string(&_2, a, SL("a"), PH_NOISY | PH_READONLY, "test/nativearray.zep", 353 TSRMLS_CC);
	zephir_array_fetch_string(&_3, _2, SL("b"), PH_NOISY | PH_READONLY, "test/nativearray.zep", 353 TSRMLS_CC);
	zephir_array_fetch_string(&b, _3, SL("c"), PH_NOISY | PH_READONLY, "test/nativearray.zep", 353 TSRMLS_CC);
	RETURN_CTOR(b);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate1) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE, "test/nativearray.zep", 361);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate2) {

	int b;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	b = 0;
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, b, &_0, PH_COPY | PH_SEPARATE, "test/nativearray.zep", 370);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate3) {

	int b;
	zval *a, *_0 = NULL, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	b = 0;
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_long(&a, b, &_0, PH_COPY | PH_SEPARATE, "test/nativearray.zep", 379);
	b = 1;
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 5);
	zephir_array_update_long(&a, b, &_1, PH_COPY | PH_SEPARATE, "test/nativearray.zep", 381);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate4) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(a, SS("a"), 1);
	add_assoc_long_ex(a, SS("b"), 2);
	add_assoc_long_ex(a, SS("c"), 3);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_update_string(&a, SL("a"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayUpdate5) {

	zval *_1, *_2, *_3;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_VAR(_1);
	zephir_create_array(_1, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(_1, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(_1, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(_1, _0);
	zephir_array_update_long(&a, 0, &_1, PH_COPY | PH_SEPARATE, "test/nativearray.zep", 396);
	ZEPHIR_INIT_VAR(_2);
	zephir_create_array(_2, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(_2, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(_2, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(_2, _0);
	zephir_array_update_long(&a, 1, &_2, PH_COPY | PH_SEPARATE, "test/nativearray.zep", 397);
	ZEPHIR_INIT_VAR(_3);
	zephir_create_array(_3, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(_3, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(_3, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(_3, _0);
	zephir_array_update_long(&a, 2, &_3, PH_COPY | PH_SEPARATE, "test/nativearray.zep", 398);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayAppend1) {

	zval *_1;
	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "hello", 1);
	zephir_array_append(&a, _0, PH_SEPARATE, "test/nativearray.zep", 405);
	zephir_array_append(&a, ZEPHIR_GLOBAL(global_null), PH_SEPARATE, "test/nativearray.zep", 405);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, 0);
	zephir_array_append(&a, _0, PH_SEPARATE, "test/nativearray.zep", 405);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, 1.10);
	zephir_array_append(&a, _0, PH_SEPARATE, "test/nativearray.zep", 405);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 5);
	zephir_array_append(&a, _0, PH_SEPARATE, "test/nativearray.zep", 405);
	ZEPHIR_INIT_VAR(_1);
	zephir_create_array(_1, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(_1, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(_1, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(_1, _0);
	zephir_array_append(&a, _1, PH_SEPARATE, "test/nativearray.zep", 405);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testArrayAppend2) {

	int f = 5;
	double e = 1.10;
	zend_bool d = 0;
	zval *b;
	zval *a, *c = NULL, *g = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "hello", 1);
	ZEPHIR_INIT_VAR(c);
	ZVAL_NULL(c);
	ZEPHIR_INIT_VAR(g);
	array_init(g);

	ZEPHIR_INIT_NVAR(g);
	zephir_create_array(g, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(g, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(g, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(g, _0);
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_append(&a, b, PH_SEPARATE, "test/nativearray.zep", 412);
	zephir_array_append(&a, c, PH_SEPARATE, "test/nativearray.zep", 412);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_BOOL(_0, d);
	zephir_array_append(&a, _0, PH_SEPARATE, "test/nativearray.zep", 412);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, e);
	zephir_array_append(&a, _0, PH_SEPARATE, "test/nativearray.zep", 412);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, f);
	zephir_array_append(&a, _0, PH_SEPARATE, "test/nativearray.zep", 412);
	zephir_array_append(&a, g, PH_SEPARATE, "test/nativearray.zep", 412);
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

PHP_METHOD(Test_NativeArray, testMultipleArrayUpdate13) {

	zval *b, *c = NULL, *d, *_0;
	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(d);
	zephir_create_array(d, 3, 0 TSRMLS_CC);
	zephir_array_update_string(&d, SL("x"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&d, SL("y"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&d, SL("z"), &ZEPHIR_GLOBAL(global_null), PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(b);
	zephir_create_array(b, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(b, SS("a"), 1);
	add_assoc_long_ex(b, SS("b"), 2);
	zephir_array_update_string(&b, SL("c"), &d, PH_COPY | PH_SEPARATE);
	ZEPHIR_CPY_WRT(c, b);
	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 2, 0 TSRMLS_CC);
	zephir_array_update_long(&a, 0, &b, PH_COPY, "test/nativearray.zep", 542);
	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(_0, SS("a"), 1);
	add_assoc_long_ex(_0, SS("b"), 2);
	add_assoc_long_ex(_0, SS("c"), 3);
	zephir_array_update_long(&a, 1, &_0, PH_COPY, "test/nativearray.zep", 542);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("lss"), 5, 0, SL("a"), SL("y"));
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayAppend1) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "y", 1);

	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("lsza"), 5, 0, SL("hello"), b);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayAppend2) {

	zval *x;
	int b = 100;
	zval *a;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(x);
	ZVAL_STRING(x, "hello", 1);

	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("lslza"), 6, 0, SL("hello"), b, x);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_NativeArray, testMultipleArrayAppend3) {

	zval *_2;
	zval *a, *b, *_0, *_1, *_3 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);
	ZEPHIR_INIT_VAR(b);
	ZVAL_STRING(b, "y", 1);

	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("lsz"), 4, 0, SL("hello"), b);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 100);
	zephir_array_update_multi(&a, &_0 TSRMLS_CC, SL("lsza"), 5, 0, SL("hello"), b);
	zephir_array_update_multi(&a, &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("lsza"), 5, 0, SL("hello"), b);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_DOUBLE(_1, 1.2);
	zephir_array_update_multi(&a, &_1 TSRMLS_CC, SL("lsza"), 5, 0, SL("hello"), b);
	ZEPHIR_INIT_VAR(_2);
	zephir_create_array(_2, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 1);
	zephir_array_fast_append(_2, _3);
	ZEPHIR_INIT_NVAR(_3);
	ZVAL_LONG(_3, 2);
	zephir_array_fast_append(_2, _3);
	ZEPHIR_INIT_NVAR(_3);
	ZVAL_LONG(_3, 3);
	zephir_array_fast_append(_2, _3);
	zephir_array_update_multi(&a, &_2 TSRMLS_CC, SL("lsza"), 5, 0, SL("hello"), b);
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

PHP_METHOD(Test_NativeArray, testArrayKeys) {

	zval *param;

	zephir_fetch_params(0, 1, 0, &param);



	zephir_array_keys(return_value, param TSRMLS_CC);
	return;

}

PHP_METHOD(Test_NativeArray, testImplodeArray) {

	zval *param, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &param);



	ZEPHIR_INIT_VAR(_0);
	zephir_array_keys(_0, param TSRMLS_CC);
	zephir_fast_join_str(return_value, SL("|"), _0 TSRMLS_CC);
	RETURN_MM();

}

/**
 * @link https://github.com/phalcon/zephir/issues/110
 */
PHP_METHOD(Test_NativeArray, issue110) {

	zval *byteUnits, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(byteUnits);
	zephir_create_array(byteUnits, 9, 0 TSRMLS_CC);
	add_assoc_long_ex(byteUnits, SS("B"), 0);
	add_assoc_long_ex(byteUnits, SS("K"), 10);
	add_assoc_long_ex(byteUnits, SS("M"), 20);
	add_assoc_long_ex(byteUnits, SS("G"), 30);
	add_assoc_long_ex(byteUnits, SS("T"), 40);
	add_assoc_long_ex(byteUnits, SS("KB"), 10);
	add_assoc_long_ex(byteUnits, SS("MB"), 20);
	add_assoc_long_ex(byteUnits, SS("GB"), 30);
	add_assoc_long_ex(byteUnits, SS("TB"), 40);
	ZEPHIR_INIT_VAR(_0);
	zephir_array_keys(_0, byteUnits TSRMLS_CC);
	zephir_fast_join_str(return_value, SL("|"), _0 TSRMLS_CC);
	RETURN_MM();

}

PHP_METHOD(Test_NativeArray, issue264) {

	zval *tokens_param = NULL;
	zval *tokens = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &tokens_param);

	zephir_get_arrval(tokens, tokens_param);


	RETURN_MM_BOOL(!(zephir_array_isset_long(tokens, 1)));

}

PHP_METHOD(Test_NativeArray, issue743a) {

	zval *current_param = NULL, *_0;
	zval *current = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &current_param);

	zephir_get_arrval(current, current_param);


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "ok", 1);
	zephir_array_update_multi(&current, &_0 TSRMLS_CC, SL("ls"), 3, 42, SL("str"));
	RETURN_CTOR(current);

}

PHP_METHOD(Test_NativeArray, issue743b) {

	zval *current_param = NULL, *_0;
	zval *current = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &current_param);

	zephir_get_arrval(current, current_param);


	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "ok", 1);
	zephir_array_update_multi(&current, &_0 TSRMLS_CC, SL("sl"), 3, SL("str"), 42);
	RETURN_CTOR(current);

}

PHP_METHOD(Test_NativeArray, issue743c) {

	zval *current_param = NULL, *key, *_0;
	zval *current = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &current_param);

	zephir_get_arrval(current, current_param);


	ZEPHIR_INIT_VAR(key);
	ZVAL_STRING(key, "hey", 1);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "ok", 1);
	zephir_array_update_multi(&current, &_0 TSRMLS_CC, SL("sz"), 3, SL("str"), key);
	RETURN_CTOR(current);

}

/**
 * @link https://github.com/phalcon/zephir/issues/709
 */
PHP_METHOD(Test_NativeArray, issue709) {

	zephir_fcall_cache_entry *_6 = NULL;
	int _1, _2, ZEPHIR_LAST_CALL_STATUS;
	zend_bool works = 1, _0, _7;
	zval *c = NULL, *arr = NULL, *_3 = NULL, *_4, *_5 = NULL;

	ZEPHIR_MM_GROW();

	_2 = 50;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(c);
			ZVAL_LONG(c, _1);
			ZEPHIR_INIT_NVAR(arr);
			zephir_create_array(arr, 2, 0 TSRMLS_CC);
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_LONG(_3, 1);
			zephir_array_fast_append(arr, _3);
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_LONG(_3, 2);
			zephir_array_fast_append(arr, _3);
			ZEPHIR_CALL_FUNCTION(&_5, "array_rand", &_6, 50, arr);
			zephir_check_call_status();
			zephir_array_fetch(&_4, arr, _5, PH_NOISY | PH_READONLY, "test/nativearray.zep", 636 TSRMLS_CC);
			ZEPHIR_CPY_WRT(arr, _4);
			_7 = ZEPHIR_LT_LONG(arr, 0);
			if (!(_7)) {
				_7 = ZEPHIR_GT_LONG(arr, 2);
			}
			if (_7) {
				works = 0;
			}
		}
	}
	RETURN_MM_BOOL(works);

}

