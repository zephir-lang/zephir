
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/operators.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_Assign) {

	ZEPHIR_REGISTER_CLASS(Test, Assign, test, assign, test_assign_method_entry, 0);

	zend_declare_property_null(test_assign_ce, SL("testVar"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_assign_ce, SL("myArray"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_assign_ce, SL("testVarStatic"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Assign, getTestVar) {

	ZEPHIR_INIT_THIS();


	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, getMyArray) {

	ZEPHIR_INIT_THIS();


	RETURN_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testAssign1) {

	int a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign2) {

	int a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign3) {

	int a = 0;
	ZEPHIR_INIT_THIS();



	a = 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign4) {

	int a = 0;
	ZEPHIR_INIT_THIS();



	a = 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign5) {

	int a = 0;
	ZEPHIR_INIT_THIS();



	a = (long) (2.0);
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign6) {

	zend_bool a = 0;
	ZEPHIR_INIT_THIS();



	a = (1) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign7) {

	zend_bool a = 0;
	ZEPHIR_INIT_THIS();



	a = (1.0) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign8) {

	zend_bool a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign9) {

	zend_bool a = 0;
	ZEPHIR_INIT_THIS();



	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign10) {

	zend_bool a = 0;
	ZEPHIR_INIT_THIS();



	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign11) {

	double a = 0;
	ZEPHIR_INIT_THIS();



	a = 0.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign12) {

	double a = 0;
	ZEPHIR_INIT_THIS();



	a = (double) (4);
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign13) {

	double a = 0;
	ZEPHIR_INIT_THIS();



	a = 0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign14) {

	double a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign15) {

	double a = 0;
	ZEPHIR_INIT_THIS();



	a = 5.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign16) {

	int a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign17) {

	double a = 0;
	ZEPHIR_INIT_THIS();



	a = 1.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign18) {

	zend_bool a = 0;
	ZEPHIR_INIT_THIS();



	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign19) {

	zend_bool a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign20) {

	zval a;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Assign, testAssign21) {

	int a = 0, b = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	b = a;
	RETURN_LONG(b);

}

PHP_METHOD(Test_Assign, testAssign22) {

	double a = 0, b = 0;
	ZEPHIR_INIT_THIS();



	a = 1.0;
	b = a;
	RETURN_DOUBLE(b);

}

PHP_METHOD(Test_Assign, testAssign23) {

	zend_bool a = 0, b = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	b = a;
	RETURN_BOOL(b);

}

PHP_METHOD(Test_Assign, testAssign24) {

	double b = 0;
	int a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	b = (double) a;
	RETURN_DOUBLE(b);

}

PHP_METHOD(Test_Assign, testAssign25) {

	double b = 0;
	int a = 0;
	ZEPHIR_INIT_THIS();



	a = 1;
	b = (double) a;
	RETURN_DOUBLE(b);

}

PHP_METHOD(Test_Assign, testAssign26) {

	double a = 0;
	int b = 0;
	ZEPHIR_INIT_THIS();



	a = (double) (1);
	b = (long) a;
	RETURN_LONG(b);

}

PHP_METHOD(Test_Assign, testAssign27) {

	double a = 0;
	int b = 0;
	ZEPHIR_INIT_THIS();



	b = 1;
	a = (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign28) {

	zend_bool a = 0;
	int b = 0;
	ZEPHIR_INIT_THIS();



	b = 1;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign29) {

	zend_bool a = 0;
	double b = 0;
	ZEPHIR_INIT_THIS();



	b = 1.0;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign30) {

	int a = 0;
	zend_bool b = 0;
	ZEPHIR_INIT_THIS();



	b = 0;
	a = b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign31) {

	double a = 0;
	zend_bool b = 0;
	ZEPHIR_INIT_THIS();



	b = 0;
	a = (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign32) {

	zend_bool b = 0, a = 0;
	ZEPHIR_INIT_THIS();



	b = 0;
	a = b;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign33) {

	zend_bool b = 0, a = 0;
	ZEPHIR_INIT_THIS();



	b = 0;
	a = b;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign34) {

	zend_bool a = 0;
	int b = 0;
	ZEPHIR_INIT_THIS();



	b = 0;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign35) {

	zend_bool a = 0;
	double b = 0;
	ZEPHIR_INIT_THIS();



	b = 0;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign36) {

	zend_bool b = 0, a = 0;
	ZEPHIR_INIT_THIS();



	b = 0;
	a = b;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign37) {

	zval _0, _3;
	zval v, arr, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_3);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "abc");
	ZEPHIR_INIT_VAR(&arr);
	array_init(&arr);
	ZEPHIR_INIT_NVAR(&arr);
	zephir_create_array(&arr, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0 TSRMLS_CC);
	add_assoc_stringl_ex(&_0, SL("b_key"), SL("b_val"));
	ZEPHIR_INIT_VAR(&_1);
	array_init(&_1);
	zephir_array_update_string(&_0, SL("b"), &_1, PH_COPY | PH_SEPARATE);
	zephir_array_update_string(&arr, SL("a"), &_0, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "d_val");
	zephir_array_update_multi(&arr, &_1 TSRMLS_CC, SL("sss"), 6, SL("a"), SL("b"), SL("d_key"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_update_string(&arr, SL("s"), &_1, PH_COPY | PH_SEPARATE);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "f");
	zephir_array_update_multi(&arr, &_2 TSRMLS_CC, SL("sssss"), 10, SL("a"), SL("b"), SL("c"), SL("d"), SL("e"));
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 1, 0 TSRMLS_CC);
	add_index_long(&_3, 3, 4);
	zephir_array_update_long(&_0, 2, &_3, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_array_update_long(&arr, 1, &_0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 6);
	zephir_array_update_multi(&arr, &_2 TSRMLS_CC, SL("lll"), 3, 1, 2, 5);
	zephir_array_update_multi(&arr, &v TSRMLS_CC, SL("llz"), 3, 1, 2, &v);
	RETURN_CCTOR(arr);

}

PHP_METHOD(Test_Assign, testAssign38) {

	zval *index, index_sub, arr, _0;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&arr);
	array_init(&arr);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "val");
	zephir_array_update_zval(&arr, index, &_0, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(arr);

}

PHP_METHOD(Test_Assign, testPropertyAssign1) {

	zval __$null, __$true, __$false, _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_DOUBLE(&_0, 1.5);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	zephir_update_property_zval(this_ptr, SL("testVar"), &__$null);
	if (0) {
		zephir_update_property_zval(this_ptr, SL("testVar"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, SL("testVar"), &__$false);
	}
	if (1) {
		zephir_update_property_zval(this_ptr, SL("testVar"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, SL("testVar"), &__$false);
	}
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 'A');
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_zval(this_ptr, SL("testVar"), &_1);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_1);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAssign2) {

	zval f;
	char e;
	zend_bool d;
	double b;
	int a;
	zval __$true, __$false, c, _0;
	ZEPHIR_INIT_THIS();

	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&f);

	ZEPHIR_MM_GROW();

	a = 1;
	b = 1.5;
	ZEPHIR_INIT_VAR(&c);
	ZVAL_NULL(&c);
	d = 0;
	e = 'A';
	ZEPHIR_INIT_VAR(&f);
	array_init(&f);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, a);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	zephir_update_property_zval(this_ptr, SL("testVar"), &c);
	if (d) {
		zephir_update_property_zval(this_ptr, SL("testVar"), &__$true);
	} else {
		zephir_update_property_zval(this_ptr, SL("testVar"), &__$false);
	}
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, e);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	zephir_update_property_zval(this_ptr, SL("testVar"), &f);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyIncr1) {

	zval _0;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("testVar") TSRMLS_CC));
	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAddAssign1) {

	zval _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_ADD_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAddAssign2) {

	zval _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_ADD_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAssignValuePlus1) {

	zval _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	zephir_read_property(&_0, this_ptr, SL("testVar"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(&_1, (zephir_get_numberval(&_0) + 1));
	zephir_update_property_zval(this_ptr, SL("testVar"), &_1);
	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyDecr) {

	zval _0;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 2);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("testVar") TSRMLS_CC));
	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertySubAssign1) {

	zval _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 0);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_SUB_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertySubAssign2) {

	zval _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_SUB_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyMulAssign1) {

	zval _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_MUL_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyMulAssign2) {

	zval _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 3);
	ZEPHIR_MUL_ASSIGN(&_0, &_1)
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAssignStringConcat) {

	zval _0;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "test");
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	zephir_concat_self_str(&_0, " string", sizeof(" string")-1 TSRMLS_CC);
	zephir_update_property_zval(this_ptr, SL("testVar"), &_0);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyArray1) {

	zval __$null, __$true, __$false, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &__$null TSRMLS_CC);
	if (0) {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$true TSRMLS_CC);
	} else {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$false TSRMLS_CC);
	}
	if (1) {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$true TSRMLS_CC);
	} else {
		zephir_update_property_array_append(this_ptr, SL("myArray"), &__$false TSRMLS_CC);
	}
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_2);
	array_init(&_2);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_2 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray2) {

	zval f;
	char e;
	zend_bool d;
	zval c, _0, _1;
	double b;
	int a;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&f);

	ZEPHIR_MM_GROW();

	a = 1;
	b = 1.5;
	ZEPHIR_INIT_VAR(&c);
	ZVAL_NULL(&c);
	d = 0;
	e = 'A';
	ZEPHIR_INIT_VAR(&f);
	array_init(&f);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(&_1, a);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_DOUBLE(&_1, b);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &c TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_BOOL(&_1, d);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(&_1, e);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &_1 TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), &f TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray3) {

	zval __$null, __$false, __$true, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_11);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &_1, &_2 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 0);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_DOUBLE(&_4, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), &_3, &_4 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_LONG(&_5, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_5, &__$null TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_6);
	ZVAL_LONG(&_6, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_6, &__$false TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_7);
	ZVAL_LONG(&_7, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_7, &__$true TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_8);
	ZVAL_LONG(&_8, 0);
	ZEPHIR_INIT_VAR(&_9);
	ZVAL_LONG(&_9, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), &_8, &_9 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_10);
	array_init(&_10);
	ZEPHIR_INIT_VAR(&_11);
	ZVAL_LONG(&_11, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), &_11, &_10 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray4) {

	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2, _3, _4;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_2 TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &__$null TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &__$false TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &__$true TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_3 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_4);
	array_init(&_4);
	zephir_update_property_array(this_ptr, SL("myArray"), index, &_4 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray5) {

	zval *index_param = NULL, __$null, __$false, __$true, _0, _1, _2, _3, _4;
	zval index;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	zephir_get_strval(&index, index_param);


	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_2 TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &__$null TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &__$false TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &__$true TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_3 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_4);
	array_init(&_4);
	zephir_update_property_array(this_ptr, SL("myArray"), &index, &_4 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray6) {

	zval __$null, __$false, __$true, _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("ll"), 2, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("ll"), 2, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 1);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 1);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray7) {

	zval __$null, __$false, __$true, _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray8) {

	zval *index, index_sub, __$null, __$false, __$true, _0, _1;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("zz"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("zz"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray9) {

	zval *index_param = NULL, __$null, __$false, __$true, _0, _1;
	int index;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	index = zephir_get_intval(index_param);


	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("ll"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("ll"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 2, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray10) {

	zval *index_param = NULL, __$null, __$false, __$true, _0, _1;
	zval index;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	zephir_get_strval(&index, index_param);


	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, &index, &index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("zz"), 2, &index, &index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("zz"), 2, &index, &index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, &index, &index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 2, &index, &index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray11) {

	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2, _3, _4;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_0 TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_2 TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_3 TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_4);
	array_init(&_4);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_4 TSRMLS_CC, SL("zza"), 3, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray12) {

	zval *index, index_sub, __$null, __$false, __$true, temp1, temp2, temp3, _0, _1, _2, _3, _4, _5, _6;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&temp1);
	ZVAL_UNDEF(&temp2);
	ZVAL_UNDEF(&temp3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_0 TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("zza"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("zza"), 3, index, index);
	zephir_read_property(&_2, this_ptr, SL("myArray"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_OBS_VAR(&temp1);
	zephir_array_fetch(&temp1, &_2, index, PH_NOISY, "test/assign.zep", 585 TSRMLS_CC);
	ZEPHIR_OBS_VAR(&temp2);
	zephir_read_property(&temp2, this_ptr, SL("myArray"), PH_NOISY_CC);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_3 TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_4 TSRMLS_CC, SL("zza"), 3, index, index);
	ZEPHIR_INIT_VAR(&_5);
	array_init(&_5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_5 TSRMLS_CC, SL("zza"), 3, index, index);
	zephir_read_property(&_6, this_ptr, SL("myArray"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&temp3);
	ZVAL_LONG(&temp3, zephir_fast_count_int(&_6 TSRMLS_CC));
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray13) {

	zval *index, index_sub, __$null, __$false, __$true, temp1, temp2, temp3, _0, _1, _2, _3, _4, _5, _6;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&temp1);
	ZVAL_UNDEF(&temp2);
	ZVAL_UNDEF(&temp3);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_0 TSRMLS_CC, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("za"), 2, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$null TSRMLS_CC, SL("za"), 2, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$false TSRMLS_CC, SL("za"), 2, index);
	zephir_read_property(&_2, this_ptr, SL("myArray"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_OBS_VAR(&temp1);
	zephir_array_fetch(&temp1, &_2, index, PH_NOISY, "test/assign.zep", 602 TSRMLS_CC);
	ZEPHIR_OBS_VAR(&temp2);
	zephir_read_property(&temp2, this_ptr, SL("myArray"), PH_NOISY_CC);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &__$true TSRMLS_CC, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_3 TSRMLS_CC, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "hello");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_4 TSRMLS_CC, SL("za"), 2, index);
	ZEPHIR_INIT_VAR(&_5);
	array_init(&_5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_5 TSRMLS_CC, SL("za"), 2, index);
	zephir_read_property(&_6, this_ptr, SL("myArray"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&temp3);
	ZVAL_LONG(&temp3, zephir_fast_count_int(&_6 TSRMLS_CC));
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray14) {

	zval _0, _1, _3;
	zval v, _2, _4, _5, _6;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "abc");
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(&_1, SL("b_key"), SL("b_val"));
	zephir_array_update_string(&_0, SL("a"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "d_val");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_2 TSRMLS_CC, SL("sss"), 6, SL("a"), SL("b"), SL("d_key"));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 1, 0 TSRMLS_CC);
	add_index_long(&_3, 3, 4);
	zephir_array_update_long(&_1, 2, &_3, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &_2, &_1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "s");
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_LONG(&_5, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), &_4, &_5 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_6);
	ZVAL_STRING(&_6, "f");
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_6 TSRMLS_CC, SL("sssss"), 10, SL("a"), SL("b"), SL("c"), SL("d"), SL("e"));
	ZEPHIR_INIT_NVAR(&_6);
	ZVAL_LONG(&_6, 6);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_6 TSRMLS_CC, SL("lll"), 3, 1, 2, 5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &v TSRMLS_CC, SL("llz"), 3, 1, 2, &v);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testStaticPropertyAssign1) {

	zval _2;
	zval __$null, __$false, __$true, _0, _1, _3;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 1);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_DOUBLE(&_0, 1.5);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &__$null);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &__$false);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &__$true);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, 'A');
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_1);
	array_init(&_1);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 1);
	zephir_array_fast_append(&_2, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 2);
	zephir_array_fast_append(&_2, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 3);
	zephir_array_fast_append(&_2, &_3);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_2);
	zephir_read_static_property_ce(&_0, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_0);

}

PHP_METHOD(Test_Assign, testStaticPropertyAssign2) {

	zval f;
	char e;
	zend_bool d;
	zval c, _0;
	double b;
	int a;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&f);

	ZEPHIR_MM_GROW();

	a = 1;
	b = 1.5;
	ZEPHIR_INIT_VAR(&c);
	ZVAL_NULL(&c);
	d = 0;
	e = 'A';
	ZEPHIR_INIT_VAR(&f);
	array_init(&f);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, a);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &c);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_BOOL(&_0, d);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, e);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &f);
	zephir_read_static_property_ce(&_0, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_0);

}

PHP_METHOD(Test_Assign, testStaticPropertyArray1) {

	zval __$null, __$false, __$true, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("l"), 1, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$null TSRMLS_CC, SL("l"), 1, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("l"), 1, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$true TSRMLS_CC, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("l"), 1, 0);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("l"), 1, 0);
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testStaticPropertyArray2) {

	zval __$null, __$false, __$true, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("s"), 2, SL("hello"));
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$null TSRMLS_CC, SL("s"), 2, SL("hello"));
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("s"), 2, SL("hello"));
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$true TSRMLS_CC, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("s"), 2, SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("s"), 2, SL("hello"));
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testStaticPropertyArray3) {

	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("z"), 1, index);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$null TSRMLS_CC, SL("z"), 1, index);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("z"), 1, index);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$true TSRMLS_CC, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("z"), 1, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("z"), 1, index);
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testStaticPropertyArrayAppend) {

	zval __$false, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_BOOL(&__$false, 0);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "test");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("a"), 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("a"), 1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("a"), 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("a"), 1);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("a"), 1);
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testStaticPropertyArrayMutli1) {

	zval __$null, __$false, __$true, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$null TSRMLS_CC, SL("ll"), 2, 0, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("ll"), 2, 0, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$true TSRMLS_CC, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 0);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ll"), 2, 0, 0);
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testStaticPropertyArrayMutli2) {

	zval __$null, __$false, __$true, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$null TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$true TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("ss"), 4, SL("hello"), SL("hello"));
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testStaticPropertyArrayMutli3) {

	zval *index, index_sub, __$null, __$false, __$true, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$null TSRMLS_CC, SL("zz"), 2, index, index);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("zz"), 2, index, index);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$true TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("zz"), 2, index, index);
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testStaticPropertyArrayMulti4) {

	zval _0, _1, _3;
	zval v, _2, _4;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "abc");
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 1, 0 TSRMLS_CC);
	add_assoc_stringl_ex(&_1, SL("b_key"), SL("b_val"));
	zephir_array_update_string(&_0, SL("a"), &_1, PH_COPY | PH_SEPARATE);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "d_val");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_2 TSRMLS_CC, SL("sss"), 6, SL("a"), SL("b"), SL("d_key"));
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_array(&_1, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	zephir_create_array(&_3, 1, 0 TSRMLS_CC);
	add_index_long(&_3, 3, 4);
	zephir_array_update_long(&_1, 2, &_3, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("l"), 1, 1);
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_2 TSRMLS_CC, SL("s"), 2, SL("s"));
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_STRING(&_2, "f");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_2 TSRMLS_CC, SL("sssss"), 10, SL("a"), SL("b"), SL("c"), SL("d"), SL("e"));
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 6);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_2 TSRMLS_CC, SL("lll"), 3, 1, 2, 5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &v TSRMLS_CC, SL("llz"), 3, 1, 2, &v);
	zephir_read_static_property_ce(&_4, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_4);

}

PHP_METHOD(Test_Assign, testStaticPropertyArrayAppend1) {

	zval __$null, __$false, __$true, _0, _1, _2;
	ZEPHIR_INIT_THIS();

	ZVAL_NULL(&__$null);
	ZVAL_BOOL(&__$false, 0);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(test_assign_ce, SL("testVarStatic"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_DOUBLE(&_1, 1.5);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("la"), 2, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$null TSRMLS_CC, SL("la"), 2, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$false TSRMLS_CC, SL("la"), 2, 0);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &__$true TSRMLS_CC, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 'A');
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("la"), 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	array_init(&_1);
	zephir_update_static_property_array_multi_ce(test_assign_ce, SL("testVarStatic"), &_1 TSRMLS_CC, SL("la"), 2, 0);
	zephir_read_static_property_ce(&_2, test_assign_ce, SL("testVarStatic"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(_2);

}

PHP_METHOD(Test_Assign, testArrayVarAssign1) {

	zval *index, index_sub, *value, value_sub, a;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &index, &value);



	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	zephir_array_update_zval(&a, index, value, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Assign, testArrayVarAssign2) {

	int _0;
	zval *index, index_sub, *value, value_sub, _POST;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_POST);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(1, 2, 0, &index, &value);



	ZEPHIR_INIT_VAR(&_POST);
	array_init(&_POST);
	_0 = zephir_maybe_separate_zval(&_POST);
	zephir_array_update_zval(&_POST, index, value, PH_COPY | PH_SEPARATE);
	if (_0) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_POST", &_POST);
	}
	RETURN_CCTOR(_POST);

}

PHP_METHOD(Test_Assign, testArrayProperty) {

	int _0;
	zval *index, index_sub, *value, value_sub, _POST;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_POST);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(1, 2, 0, &index, &value);



	ZEPHIR_INIT_VAR(&_POST);
	array_init(&_POST);
	_0 = zephir_maybe_separate_zval(&_POST);
	zephir_array_update_zval(&_POST, index, value, PH_COPY | PH_SEPARATE);
	if (_0) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_POST", &_POST);
	}
	RETURN_CCTOR(_POST);

}

/**
 * @link https://github.com/phalcon/zephir/issues/159
 */
PHP_METHOD(Test_Assign, testGlobalVarAssign) {

	int _0;
	zval *index, index_sub, *value, value_sub, _POST;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&index_sub);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_POST);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_POST, SL("_POST"));
	zephir_fetch_params(0, 2, 0, &index, &value);



	_0 = zephir_maybe_separate_zval(&_POST);
	zephir_array_update_zval(&_POST, index, value, PH_COPY | PH_SEPARATE);
	if (_0) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_POST", &_POST);
	}
	ZEPHIR_MM_RESTORE();

}

/**
 * @link https://github.com/phalcon/zephir/issues/523
 */
PHP_METHOD(Test_Assign, testConstantKeyAssign) {

	zval elements;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&elements);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&elements);
	zephir_create_array(&elements, 3, 0 TSRMLS_CC);
	add_assoc_long_ex(&elements, SL("abc"), 1);
	add_index_long(&elements, 131072, 131079);
	add_index_long(&elements, 131073, 131080);
	ZEPHIR_MM_RESTORE();

}

/**
 * @link https://github.com/phalcon/zephir/issues/746
 *
 */
PHP_METHOD(Test_Assign, testArrayBoolExpressionAssign) {

	zval str, _0, _1, _2, _3, _4;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&str);
	ZVAL_STRING(&str, "abc");
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, SL("myArray"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "a");
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_BOOL(&_2, ZEPHIR_IS_STRING(&str, "abc"));
	zephir_update_property_array(this_ptr, SL("myArray"), &_1, &_2 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "b");
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_BOOL(&_4, !ZEPHIR_IS_STRING(&str, "abc"));
	zephir_update_property_array(this_ptr, SL("myArray"), &_3, &_4 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

/**
 * @link https://github.com/phalcon/zephir/issues/725
 */
PHP_METHOD(Test_Assign, testAssignSuperGlobals) {

	int _1, _2, _4, _6, _8;
	zval _GET, v, _0, _3, _5, _7;
	ZEPHIR_INIT_THIS();

	ZVAL_UNDEF(&_GET);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_7);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_GET, SL("_GET"));

	ZEPHIR_INIT_VAR(&v);
	ZVAL_STRING(&v, "stest2");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "stest");
	_1 = zephir_maybe_separate_zval(&_GET);
	zephir_array_update_string(&_GET, SL("steststr"), &_0, PH_COPY | PH_SEPARATE);
	if (_1) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_GET", &_GET);
	}
	_2 = zephir_maybe_separate_zval(&_GET);
	zephir_array_update_string(&_GET, SL("steststr2"), &v, PH_COPY | PH_SEPARATE);
	if (_2) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_GET", &_GET);
	}
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 1);
	_4 = zephir_maybe_separate_zval(&_GET);
	zephir_array_update_string(&_GET, SL("stestint"), &_3, PH_COPY | PH_SEPARATE);
	if (_4) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_GET", &_GET);
	}
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_LONG(&_5, 2);
	_6 = zephir_maybe_separate_zval(&_GET);
	zephir_array_update_string(&_GET, SL("stestint2"), &_5, PH_COPY | PH_SEPARATE);
	if (_6) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_GET", &_GET);
	}
	ZEPHIR_INIT_VAR(&_7);
	ZVAL_STRING(&_7, "testval");
	_8 = zephir_maybe_separate_zval(&_GET);
	zephir_array_update_zval(&_GET, &v, &_7, PH_COPY | PH_SEPARATE);
	if (_8) {
		ZEPHIR_SET_SYMBOL(&EG(symbol_table), "_GET", &_GET);
	}
	ZEPHIR_MM_RESTORE();

}

