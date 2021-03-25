
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_NativeArray) {

	ZEPHIR_REGISTER_CLASS(Stub, NativeArray, stub, nativearray, stub_nativearray_method_entry, 0);

	zend_declare_property_null(stub_nativearray_ce, SL("prefixes"), ZEND_ACC_PROTECTED);

	stub_nativearray_ce->create_object = zephir_init_properties_Stub_NativeArray;

	return SUCCESS;

}

PHP_METHOD(Stub_NativeArray, testArray1) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray2) {

	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray3) {

	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 1.1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 2.2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 3.3);
	zephir_array_fast_append(&a, &_0);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray4) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$false, __$true, a;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_fast_append(&a, &__$false);
	zephir_array_fast_append(&a, &__$true);
	zephir_array_fast_append(&a, &__$false);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray5) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_fast_append(&a, &__$null);
	zephir_array_fast_append(&a, &__$null);
	zephir_array_fast_append(&a, &__$null);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray6) {

	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "x");
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "y");
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "z");
	zephir_array_fast_append(&a, &_0);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray7) {

	zend_long a = 0, b = 0, c = 0;
	zval d, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&d);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	a = 1;
	b = 2;
	c = 3;
	ZEPHIR_INIT_VAR(&d);
	zephir_create_array(&d, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_array_fast_append(&d, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_array_fast_append(&d, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, c);
	zephir_array_fast_append(&d, &_0);
	RETURN_CCTOR(&d);

}

PHP_METHOD(Stub_NativeArray, testArray8) {

	double a = 0, b = 0, c = 0;
	zval d, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&d);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	a = (double) (1);
	b = (double) (2);
	c = (double) (3);
	ZEPHIR_INIT_VAR(&d);
	zephir_create_array(&d, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, a);
	zephir_array_fast_append(&d, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_array_fast_append(&d, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, c);
	zephir_array_fast_append(&d, &_0);
	RETURN_CCTOR(&d);

}

PHP_METHOD(Stub_NativeArray, testArray9) {

	zend_bool a = 0, b = 0, c = 0;
	zval d, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&d);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	a = 1;
	b = 0;
	c = 1;
	ZEPHIR_INIT_VAR(&d);
	zephir_create_array(&d, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, a);
	zephir_array_fast_append(&d, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, b);
	zephir_array_fast_append(&d, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, c);
	zephir_array_fast_append(&d, &_0);
	RETURN_CCTOR(&d);

}

PHP_METHOD(Stub_NativeArray, testArray10) {

	zval a, b, c;
	zval d;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&d);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "hello1");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "hello2");
	ZEPHIR_INIT_VAR(&c);
	ZVAL_STRING(&c, "hello3");
	ZEPHIR_INIT_VAR(&d);
	zephir_create_array(&d, 3, 0);
	zephir_array_fast_append(&d, &a);
	zephir_array_fast_append(&d, &b);
	zephir_array_fast_append(&d, &c);
	RETURN_CCTOR(&d);

}

PHP_METHOD(Stub_NativeArray, testArray11) {

	zval a, b, c, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_VAR(&b);
	zephir_create_array(&b, 3, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&b, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_array_fast_append(&b, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 6);
	zephir_array_fast_append(&b, &_0);
	ZEPHIR_INIT_VAR(&c);
	zephir_create_array(&c, 2, 0);
	zephir_array_fast_append(&c, &a);
	zephir_array_fast_append(&c, &b);
	RETURN_CCTOR(&c);

}

PHP_METHOD(Stub_NativeArray, testArray12) {

	zval a, b, c, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_VAR(&b);
	zephir_create_array(&b, 1, 0);
	zephir_array_fast_append(&b, &a);
	ZEPHIR_INIT_VAR(&c);
	zephir_create_array(&c, 1, 0);
	zephir_array_fast_append(&c, &b);
	RETURN_CCTOR(&c);

}

PHP_METHOD(Stub_NativeArray, testArray13) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_index_stringl(&a, 1, SL("hello1"));
	add_index_stringl(&a, 2, SL("hello2"));
	add_index_stringl(&a, 3, SL("hello3"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray14) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_long_ex(&a, SL("hello1"), 1);
	add_assoc_long_ex(&a, SL("hello2"), 2);
	add_assoc_long_ex(&a, SL("hello3"), 3);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray15) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$true, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_update_string(&a, SL("hello1"), &__$true, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello2"), &__$false, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello3"), &__$true, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray16) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_double_ex(&a, SL("hello1"), 1.0);
	add_assoc_double_ex(&a, SL("hello2"), 2.0);
	add_assoc_double_ex(&a, SL("hello3"), 3.0);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray17) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_update_string(&a, SL("hello1"), &__$null, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello2"), &__$null, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("hello3"), &__$null, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray18) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_stringl_ex(&a, SL("hello1"), SL("a"));
	add_assoc_stringl_ex(&a, SL("hello2"), SL("b"));
	add_assoc_stringl_ex(&a, SL("hello3"), SL("c"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray19) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$true, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_update_long(&a, 0, &__$true, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 1, &__$false, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 2, &__$true, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray20) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_index_double(&a, 0, 1.0);
	add_index_double(&a, 1, 2.0);
	add_index_double(&a, 2, 3.0);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray21) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_update_long(&a, 0, &__$null, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 1, &__$null, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 2, &__$null, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray22) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_index_long(&a, 0, 4);
	add_index_long(&a, 1, 5);
	add_index_long(&a, 2, 6);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray23) {

	zend_long b = 0;
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	b = 0;
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray24) {

	double b = 0;
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	b = 0.0;
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray25) {

	zend_bool b = 0;
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	b = 0;
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, b);
	zephir_array_update_long(&a, 0, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, b);
	zephir_array_update_long(&a, 1, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, b);
	zephir_array_update_long(&a, 2, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray26) {

	zval a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&b);
	ZVAL_NULL(&b);
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_update_long(&a, 0, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 1, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 2, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray27) {

	zval b;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "hello");
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_update_long(&a, 0, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 1, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 2, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray28) {

	zval b;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "hello");
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	zephir_array_update_long(&a, 0, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 1, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&a, 2, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray29) {

	long b = 0;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	b = 1;
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	add_index_stringl(&a, b, SL("hello"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArray30) {

	zval b;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "hello");
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	add_assoc_stringl_ex(&a, Z_STRVAL_P(&b), Z_STRLEN_P(&b), SL("hello"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayAccess1) {

	zval a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	zephir_array_fetch_long(&b, &a, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 245);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayAccess2) {

	zval a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_long_ex(&a, SL("a"), 1);
	add_assoc_long_ex(&a, SL("b"), 2);
	add_assoc_long_ex(&a, SL("c"), 3);
	zephir_array_fetch_string(&b, &a, SL("a"), PH_NOISY | PH_READONLY, "stub/nativearray.zep", 255);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayAccess3) {

	long c = 0;
	zval a, b, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	c = 0;
	zephir_array_fetch_long(&b, &a, c, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 267);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayAccess4) {

	zval c;
	zval a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_long_ex(&a, SL("a"), 1);
	add_assoc_long_ex(&a, SL("b"), 2);
	add_assoc_long_ex(&a, SL("c"), 3);
	ZEPHIR_INIT_VAR(&c);
	ZVAL_STRING(&c, "a");
	zephir_array_fetch(&b, &a, &c, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 279);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayAccess5) {

	zval a, b, c;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_long_ex(&a, SL("a"), 1);
	add_assoc_long_ex(&a, SL("b"), 2);
	add_assoc_long_ex(&a, SL("c"), 3);
	ZEPHIR_INIT_VAR(&c);
	ZVAL_STRING(&c, "a");
	zephir_array_fetch(&b, &a, &c, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 290);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayAccess6) {

	zval a, b, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_long_ex(&a, SL("a"), 1);
	add_assoc_long_ex(&a, SL("b"), 2);
	add_assoc_long_ex(&a, SL("c"), 3);
	zephir_array_fetch_string(&_0, &a, SL("a"), PH_NOISY | PH_READONLY, "stub/nativearray.zep", 300);
	zephir_array_fetch_string(&_1, &a, SL("b"), PH_NOISY | PH_READONLY, "stub/nativearray.zep", 300);
	ZEPHIR_INIT_VAR(&b);
	zephir_add_function(&b, &_0, &_1);
	RETURN_CCTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayMultipleAccess1) {

	zval _0;
	zval a, b, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	add_index_stringl(&_0, 0, SL("a"));
	add_index_stringl(&_0, 1, SL("b"));
	zephir_array_update_long(&a, 0, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_fetch_long(&_1, &a, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 310);
	zephir_array_fetch_long(&b, &_1, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 310);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayMultipleAccess2) {

	zval _0;
	zval a, b, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "a");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&a, &_0);
	zephir_array_fetch_long(&_2, &a, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 320);
	zephir_array_fetch_long(&b, &_2, 1, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 320);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayMultipleAccess3) {

	zval _0, _1;
	zval a, b, _2, _3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "a");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "b");
	zephir_array_fast_append(&_1, &_2);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&a, &_0);
	zephir_array_fetch_long(&_3, &a, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 330);
	zephir_array_fetch_long(&_4, &_3, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 330);
	zephir_array_fetch_long(&b, &_4, 1, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 330);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayMultipleAccess4) {

	zval _0, _1;
	zval a, b, _2, _3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "a");
	zephir_array_fast_append(&_1, &_2);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "b");
	zephir_array_fast_append(&_1, &_2);
	zephir_array_fast_append(&_0, &_1);
	zephir_array_fast_append(&a, &_0);
	zephir_array_fetch_long(&_3, &a, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 340);
	zephir_array_fetch_long(&_4, &_3, 0, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 340);
	zephir_array_fetch_long(&b, &_4, 1, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 340);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayMultipleAccess5) {

	zval _0, _1;
	zval a, b, _2, _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 1, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	add_assoc_long_ex(&_1, SL("c"), 0);
	add_assoc_long_ex(&_1, SL("d"), 1);
	zephir_array_update_string(&_0, SL("b"), &_1, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&a, SL("a"), &_0, PH_COPY | PH_SEPARATE);
	zephir_array_fetch_string(&_2, &a, SL("a"), PH_NOISY | PH_READONLY, "stub/nativearray.zep", 350);
	zephir_array_fetch_string(&_3, &_2, SL("b"), PH_NOISY | PH_READONLY, "stub/nativearray.zep", 350);
	zephir_array_fetch_string(&b, &_3, SL("c"), PH_NOISY | PH_READONLY, "stub/nativearray.zep", 350);
	RETURN_CTOR(&b);

}

PHP_METHOD(Stub_NativeArray, testArrayUpdate1) {

	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_update_long(&a, 0, &_0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayUpdate2) {

	zend_long b = 0;
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	b = 0;
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_update_long(&a, b, &_0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayUpdate3) {

	zend_long b = 0;
	zval a, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	b = 0;
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_update_long(&a, b, &_0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	b = 1;
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_array_update_long(&a, b, &_1, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayUpdate4) {

	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	add_assoc_long_ex(&a, SL("a"), 1);
	add_assoc_long_ex(&a, SL("b"), 2);
	add_assoc_long_ex(&a, SL("c"), 3);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_update_string(&a, SL("a"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayUpdate5) {

	zval _1, _2, _3;
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 3, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_1, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_1, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&_1, &_0);
	zephir_array_update_long(&a, 0, &_1, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 3, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&_2, &_0);
	zephir_array_update_long(&a, 1, &_2, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 3, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_3, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_3, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&_3, &_0);
	zephir_array_update_long(&a, 2, &_3, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayAppend1) {

	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a, _0;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	zephir_array_append(&a, &_0, PH_SEPARATE, "stub/nativearray.zep", 402);
	zephir_array_append(&a, &__$null, PH_SEPARATE, "stub/nativearray.zep", 402);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, 0);
	zephir_array_append(&a, &_0, PH_SEPARATE, "stub/nativearray.zep", 402);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 1.10);
	zephir_array_append(&a, &_0, PH_SEPARATE, "stub/nativearray.zep", 402);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_array_append(&a, &_0, PH_SEPARATE, "stub/nativearray.zep", 402);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 3, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_1, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_1, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&_1, &_0);
	zephir_array_append(&a, &_1, PH_SEPARATE, "stub/nativearray.zep", 402);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayAppend2) {

	zend_long f;
	double e;
	zend_bool d;
	zval b;
	zval a, c, g, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&g);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "hello");
	ZEPHIR_INIT_VAR(&c);
	ZVAL_NULL(&c);
	d = 0;
	e = 1.10;
	f = 5;
	ZEPHIR_INIT_VAR(&g);
	zephir_create_array(&g, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&g, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&g, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&g, &_0);
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_append(&a, &b, PH_SEPARATE, "stub/nativearray.zep", 409);
	zephir_array_append(&a, &c, PH_SEPARATE, "stub/nativearray.zep", 409);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, d);
	zephir_array_append(&a, &_0, PH_SEPARATE, "stub/nativearray.zep", 409);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, e);
	zephir_array_append(&a, &_0, PH_SEPARATE, "stub/nativearray.zep", 409);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, f);
	zephir_array_append(&a, &_0, PH_SEPARATE, "stub/nativearray.zep", 409);
	zephir_array_append(&a, &g, PH_SEPARATE, "stub/nativearray.zep", 409);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate1) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("ss"), 4, SL("x"), SL("y"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate2) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("sss"), 6, SL("x"), SL("y"), SL("z"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate3) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("ll"), 2, 0, 1);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate4) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("lll"), 3, 0, 1, 2);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate5) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("sls"), 5, SL("a"), 1, SL("c"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate6) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("lsl"), 4, 0, SL("b"), 2);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate7) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$true, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$true, SL("sls"), 5, SL("a"), 1, SL("c"));
	zephir_array_update_multi(&a, &__$false, SL("lsl"), 4, 0, SL("b"), 2);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate8) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$true, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("sl"), 3, SL("a"), 0);
	zephir_array_update_multi(&a, &__$true, SL("sl"), 3, SL("a"), 1);
	zephir_array_update_multi(&a, &__$false, SL("sl"), 3, SL("a"), 2);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate9) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$true, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("sl"), 3, SL("a"), 0);
	zephir_array_update_multi(&a, &__$true, SL("sl"), 3, SL("a"), 1);
	zephir_array_update_multi(&a, &__$false, SL("sl"), 3, SL("a"), 1);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate10) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$true, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("sl"), 3, SL("a"), 0);
	zephir_array_update_multi(&a, &__$true, SL("sl"), 3, SL("a"), 1);
	zephir_array_update_multi(&a, &__$false, SL("sls"), 5, SL("a"), 1, SL("c"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate11) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a, b, c;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "y");
	ZEPHIR_INIT_VAR(&c);
	ZVAL_STRING(&c, "x");
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("zz"), 2, &b, &c);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate12) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a, b, c, d;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&d);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "y");
	ZEPHIR_INIT_VAR(&c);
	ZVAL_STRING(&c, "x");
	ZEPHIR_INIT_VAR(&d);
	ZVAL_STRING(&d, "z");
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_multi(&a, &__$null, SL("zzz"), 3, &b, &c, &d);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayUpdate13) {

	zval b, c, d, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&d);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&d);
	zephir_create_array(&d, 3, 0);
	zephir_array_update_string(&d, SL("x"), &__$null, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&d, SL("y"), &__$null, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&d, SL("z"), &__$null, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&b);
	zephir_create_array(&b, 3, 0);
	add_assoc_long_ex(&b, SL("a"), 1);
	add_assoc_long_ex(&b, SL("b"), 2);
	zephir_array_update_string(&b, SL("c"), &d, PH_COPY | PH_SEPARATE);
	ZEPHIR_CPY_WRT(&c, &b);
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 2, 0);
	zephir_array_update_long(&a, 0, &b, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	add_assoc_long_ex(&_0, SL("a"), 1);
	add_assoc_long_ex(&_0, SL("b"), 2);
	add_assoc_long_ex(&_0, SL("c"), 3);
	zephir_array_update_long(&a, 1, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_multi(&a, &__$false, SL("lss"), 5, 0, SL("a"), SL("y"));
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayAppend1) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a, b;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "y");
	zephir_array_update_multi(&a, &__$null, SL("lsza"), 5, 0, SL("hello"), &b);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayAppend2) {

	zval x;
	zend_long b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, a;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&x);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	b = 100;
	ZEPHIR_INIT_VAR(&x);
	ZVAL_STRING(&x, "hello");
	zephir_array_update_multi(&a, &__$null, SL("lslza"), 6, 0, SL("hello"), b, &x);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testMultipleArrayAppend3) {

	zval _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval __$null, __$false, a, b, _0, _1, _3;
	zval *this_ptr = getThis();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_2);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "y");
	zephir_array_update_multi(&a, &__$null, SL("lsz"), 4, 0, SL("hello"), &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 100);
	zephir_array_update_multi(&a, &_0, SL("lsza"), 5, 0, SL("hello"), &b);
	zephir_array_update_multi(&a, &__$false, SL("lsza"), 5, 0, SL("hello"), &b);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.2);
	zephir_array_update_multi(&a, &_1, SL("lsza"), 5, 0, SL("hello"), &b);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 3, 0);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 1);
	zephir_array_fast_append(&_2, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 2);
	zephir_array_fast_append(&_2, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 3);
	zephir_array_fast_append(&_2, &_3);
	zephir_array_update_multi(&a, &_2, SL("lsza"), 5, 0, SL("hello"), &b);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_NativeArray, testArrayKeys) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(param)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &param);



	zephir_array_keys(return_value, param);
	return;

}

PHP_METHOD(Stub_NativeArray, testImplodeArray) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *param, param_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&param_sub);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(param)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &param);



	ZEPHIR_INIT_VAR(&_0);
	zephir_array_keys(&_0, param);
	zephir_fast_join_str(return_value, SL("|"), &_0);
	RETURN_MM();

}

/**
 * @link https://github.com/zephir-lang/zephir/issues/110
 */
PHP_METHOD(Stub_NativeArray, issue110) {

	zval byteUnits, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&byteUnits);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&byteUnits);
	zephir_create_array(&byteUnits, 9, 0);
	add_assoc_long_ex(&byteUnits, SL("B"), 0);
	add_assoc_long_ex(&byteUnits, SL("K"), 10);
	add_assoc_long_ex(&byteUnits, SL("M"), 20);
	add_assoc_long_ex(&byteUnits, SL("G"), 30);
	add_assoc_long_ex(&byteUnits, SL("T"), 40);
	add_assoc_long_ex(&byteUnits, SL("KB"), 10);
	add_assoc_long_ex(&byteUnits, SL("MB"), 20);
	add_assoc_long_ex(&byteUnits, SL("GB"), 30);
	add_assoc_long_ex(&byteUnits, SL("TB"), 40);
	ZEPHIR_INIT_VAR(&_0);
	zephir_array_keys(&_0, &byteUnits);
	zephir_fast_join_str(return_value, SL("|"), &_0);
	RETURN_MM();

}

PHP_METHOD(Stub_NativeArray, issue264) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *tokens_param = NULL;
	zval tokens;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&tokens);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(tokens)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &tokens_param);

	zephir_get_arrval(&tokens, tokens_param);


	RETURN_MM_BOOL(!(zephir_array_isset_long(&tokens, 1)));

}

PHP_METHOD(Stub_NativeArray, issue743a) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *current743a_param = NULL, _0;
	zval current743a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&current743a);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(current743a)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &current743a_param);

	zephir_get_arrval(&current743a, current743a_param);


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "ok");
	zephir_array_update_multi(&current743a, &_0, SL("ls"), 3, 42, SL("str"));
	RETURN_CTOR(&current743a);

}

PHP_METHOD(Stub_NativeArray, issue743b) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *current_param = NULL, _0;
	zval current;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&current);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(current)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &current_param);

	zephir_get_arrval(&current, current_param);


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "ok");
	zephir_array_update_multi(&current, &_0, SL("sl"), 3, SL("str"), 42);
	RETURN_CTOR(&current);

}

PHP_METHOD(Stub_NativeArray, issue743c) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *current_param = NULL, key, _0;
	zval current;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&current);
	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(current)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &current_param);

	zephir_get_arrval(&current, current_param);


	ZEPHIR_INIT_VAR(&key);
	ZVAL_STRING(&key, "hey");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "ok");
	zephir_array_update_multi(&current, &_0, SL("sz"), 3, SL("str"), &key);
	RETURN_CTOR(&current);

}

/**
 * @link https://github.com/zephir-lang/zephir/issues/709
 */
PHP_METHOD(Stub_NativeArray, issue709) {

	zend_bool works, _0, _7$$3;
	zval c, arr, _3$$3, _4$$3, _5$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zephir_fcall_cache_entry *_6 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);


	ZEPHIR_MM_GROW();

	works = 1;
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
			ZEPHIR_INIT_NVAR(&c);
			ZVAL_LONG(&c, _1);
			ZEPHIR_INIT_NVAR(&arr);
			zephir_create_array(&arr, 2, 0);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 1);
			zephir_array_fast_append(&arr, &_3$$3);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 2);
			zephir_array_fast_append(&arr, &_3$$3);
			ZEPHIR_CALL_FUNCTION(&_5$$3, "array_rand", &_6, 59, &arr);
			zephir_check_call_status();
			zephir_array_fetch(&_4$$3, &arr, &_5$$3, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 626);
			ZEPHIR_CPY_WRT(&arr, &_4$$3);
			_7$$3 = ZEPHIR_LT_LONG(&arr, 0);
			if (!(_7$$3)) {
				_7$$3 = ZEPHIR_GT_LONG(&arr, 2);
			}
			if (_7$$3) {
				works = 0;
			}
		}
	}
	RETURN_MM_BOOL(works);

}

PHP_METHOD(Stub_NativeArray, Issue1140) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *prefix_param = NULL, *baseDir_param = NULL, _0, _2, _3, _1$$3;
	zval prefix, baseDir;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&prefix);
	ZVAL_UNDEF(&baseDir);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_1$$3);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(prefix)
		Z_PARAM_STR(baseDir)
	ZEND_PARSE_PARAMETERS_END();

#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &prefix_param, &baseDir_param);

	zephir_get_strval(&prefix, prefix_param);
	zephir_get_strval(&baseDir, baseDir_param);


	zephir_read_property(&_0, this_ptr, ZEND_STRL("prefixes"), PH_NOISY_CC | PH_READONLY);
	if (!(zephir_array_isset(&_0, &prefix))) {
		ZEPHIR_INIT_VAR(&_1$$3);
		array_init(&_1$$3);
		zephir_update_property_array(this_ptr, SL("prefixes"), &prefix, &_1$$3);
	}
	zephir_read_property(&_2, this_ptr, ZEND_STRL("prefixes"), PH_NOISY_CC | PH_READONLY);
	zephir_array_fetch(&_3, &_2, &prefix, PH_NOISY | PH_READONLY, "stub/nativearray.zep", 639);
	ZEPHIR_MAKE_REF(&_3);
	ZEPHIR_CALL_FUNCTION(NULL, "array_push", NULL, 60, &_3, &baseDir);
	ZEPHIR_UNREF(&_3);
	zephir_check_call_status();
	RETURN_MM_MEMBER(getThis(), "prefixes");

}

PHP_METHOD(Stub_NativeArray, Issue1159) {

	zval myvar, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&myvar);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&myvar);
	ZVAL_LONG(&myvar, 1);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_fast_append(&_0, &myvar);
	ZEPHIR_CPY_WRT(&myvar, &_0);
	RETURN_CCTOR(&myvar);

}

zend_object *zephir_init_properties_Stub_NativeArray(zend_class_entry *class_type) {

		zval _0, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_MM_GROW();
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("prefixes"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("prefixes"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}

}

