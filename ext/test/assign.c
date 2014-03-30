
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
#include "kernel/operators.h"
#include "kernel/array.h"


/**
 * Control Flow
 */
ZEPHIR_INIT_CLASS(Test_Assign) {

	ZEPHIR_REGISTER_CLASS(Test, Assign, test, assign, test_assign_method_entry, 0);

	zend_declare_property_null(test_assign_ce, SL("testVar"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_null(test_assign_ce, SL("myArray"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Assign, getTestVar) {


	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testAssign1) {

	int a;


	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign2) {

	int a;


	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign3) {

	int a;


	a = 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign4) {

	int a;


	a = 0;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign5) {

	int a;


	a = (long) (2.0);
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign6) {

	zend_bool a;


	a = (1) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign7) {

	zend_bool a;


	a = (1.0) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign8) {

	zend_bool a;


	a = 1;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign9) {

	zend_bool a;


	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign10) {

	zend_bool a;


	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign11) {

	double a;


	a = 0.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign12) {

	double a;


	a = (double) (4);
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign13) {

	double a;


	a = 0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign14) {

	double a;


	a = 1;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign15) {

	double a;


	a = 5.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign16) {

	int a;


	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign17) {

	double a;


	a = 1.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign18) {

	zend_bool a;


	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign19) {

	zend_bool a;


	a = 1;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign20) {

	zval a;


	ZEPHIR_SINIT_VAR(a);
	ZVAL_NULL(&a);
	RETURN_LCTORW(a);

}

PHP_METHOD(Test_Assign, testAssign21) {

	int a, b;


	a = 1;
	b = a;
	RETURN_LONG(b);

}

PHP_METHOD(Test_Assign, testAssign22) {

	double a, b;


	a = 1.0;
	b = a;
	RETURN_DOUBLE(b);

}

PHP_METHOD(Test_Assign, testAssign23) {

	zend_bool a, b;


	a = 1;
	b = a;
	RETURN_BOOL(b);

}

PHP_METHOD(Test_Assign, testAssign24) {

	double b;
	int a;


	a = 1;
	b = (double) a;
	RETURN_DOUBLE(b);

}

PHP_METHOD(Test_Assign, testAssign25) {

	double b;
	int a;


	a = 1;
	b = (double) a;
	RETURN_DOUBLE(b);

}

PHP_METHOD(Test_Assign, testAssign26) {

	double a;
	int b;


	a = (double) (1);
	b = (long) a;
	RETURN_LONG(b);

}

PHP_METHOD(Test_Assign, testAssign27) {

	double a;
	int b;


	b = 1;
	a = (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign28) {

	zend_bool a;
	int b;


	b = 1;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign29) {

	zend_bool a;
	double b;


	b = 1.0;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign30) {

	int a;
	zend_bool b;


	b = 0;
	a = b;
	RETURN_LONG(a);

}

PHP_METHOD(Test_Assign, testAssign31) {

	double a;
	zend_bool b;


	b = 0;
	a = (double) b;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Assign, testAssign32) {

	zend_bool b, a;


	b = 0;
	a = b;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign33) {

	zend_bool b, a;


	b = 0;
	a = b;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign34) {

	zend_bool a;
	int b;


	b = 0;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign35) {

	zend_bool a;
	double b;


	b = 0;
	a = (b) ? 1 : 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testAssign36) {

	zend_bool b, a;


	b = 0;
	a = b;
	RETURN_BOOL(a);

}

PHP_METHOD(Test_Assign, testPropertyIncr1) {

	zval *_0;


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_ON_FAILURE(zephir_property_incr(this_ptr, SL("testVar") TSRMLS_CC));
	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAddAssign1) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 0);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	ZEPHIR_ADD_ASSIGN(_0, _1)
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAddAssign2) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	ZEPHIR_ADD_ASSIGN(_0, _1)
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAssignValuePlus1) {

	zval *_0, *_1;


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("testVar"), PH_NOISY_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, (zephir_get_numberval(_0) + 1));
	zephir_update_property_this(this_ptr, SL("testVar"), _1 TSRMLS_CC);
	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyDecr) {

	zval *_0;


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 2);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_ON_FAILURE(zephir_property_decr(this_ptr, SL("testVar") TSRMLS_CC));
	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertySubAssign1) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 0);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	ZEPHIR_SUB_ASSIGN(_0, _1)
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertySubAssign2) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	ZEPHIR_SUB_ASSIGN(_0, _1)
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyMulAssign1) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 2);
	ZEPHIR_MUL_ASSIGN(_0, _1)
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyMulAssign2) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 3);
	ZEPHIR_MUL_ASSIGN(_0, _1)
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyAssignStringConcat) {

	zval *_0;


	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_STRING(_0, "test", 1);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	zephir_concat_self_str(&_0, " string", sizeof(" string")-1 TSRMLS_CC);
	zephir_update_property_this(this_ptr, SL("testVar"), _0 TSRMLS_CC);
	RETURN_MEMBER(this_ptr, "testVar");

}

PHP_METHOD(Test_Assign, testPropertyArray1) {

	zval *_0, *_1, *_2;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), (0) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), (1) ? ZEPHIR_GLOBAL(global_true) : ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, 'A');
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_STRING(_1, "hello", 1);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	array_init(_2);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _2 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray2) {

	zval *f;
	char e = 'A';
	zend_bool d = 0;
	zval *c = NULL, *_0, *_1;
	double b = 1.5;
	int a = 1;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(c);
	ZVAL_NULL(c);
	ZEPHIR_INIT_VAR(f);
	array_init(f);

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, a);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_DOUBLE(_1, b);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), c TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_BOOL(_1, d);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	ZEPHIR_INIT_ZVAL_NREF(_1);
	ZVAL_LONG(_1, e);
	zephir_update_property_array_append(this_ptr, SL("myArray"), _1 TSRMLS_CC);
	zephir_update_property_array_append(this_ptr, SL("myArray"), f TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray3) {

	zval *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7, *_8, *_9, *_10, *_11;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 0);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), _1, _2 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 0);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_DOUBLE(_4, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), _3, _4 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_5);
	ZVAL_LONG(_5, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), _5, ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	ZEPHIR_INIT_VAR(_6);
	ZVAL_LONG(_6, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), _6, ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	ZEPHIR_INIT_VAR(_7);
	ZVAL_LONG(_7, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), _7, ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	ZEPHIR_INIT_VAR(_8);
	ZVAL_LONG(_8, 0);
	ZEPHIR_INIT_VAR(_9);
	ZVAL_LONG(_9, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), _8, _9 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_10);
	array_init(_10);
	ZEPHIR_INIT_VAR(_11);
	ZVAL_LONG(_11, 0);
	zephir_update_property_array(this_ptr, SL("myArray"), _11, _10 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray4) {

	zval *index, *_0, *_1, *_2, *_3, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), index, _1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_DOUBLE(_2, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), index, _2 TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), index, _3 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_4);
	array_init(_4);
	zephir_update_property_array(this_ptr, SL("myArray"), index, _4 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray5) {

	zval *index_param = NULL, *_0, *_1, *_2, *_3, *_4;
	zval *index = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	zephir_get_strval(index, index_param);


	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array(this_ptr, SL("myArray"), index, _1 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_DOUBLE(_2, 1.5);
	zephir_update_property_array(this_ptr, SL("myArray"), index, _2 TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, ZEPHIR_GLOBAL(global_null) TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, ZEPHIR_GLOBAL(global_false) TSRMLS_CC);
	zephir_update_property_array(this_ptr, SL("myArray"), index, ZEPHIR_GLOBAL(global_true) TSRMLS_CC);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_LONG(_3, 'A');
	zephir_update_property_array(this_ptr, SL("myArray"), index, _3 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_4);
	array_init(_4);
	zephir_update_property_array(this_ptr, SL("myArray"), index, _4 TSRMLS_CC);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray6) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, 0, 1);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("ll"), 3, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("ll"), 3, 0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("ll"), 3, 0, 1);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, 0, 1);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "hello", 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, 0, 1);
	ZEPHIR_INIT_BNVAR(_1);
	array_init(_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, 0, 1);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray7) {

	zval *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "hello", 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	ZEPHIR_INIT_BNVAR(_1);
	array_init(_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ss"), 5, SL("hello"), SL("hello"));
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray8) {

	zval *index, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "hello", 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	array_init(_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray9) {

	zval *index_param = NULL, *_0, *_1;
	int index;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	index = zephir_get_intval(index_param);


	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("ll"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("ll"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("ll"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "hello", 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	array_init(_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("ll"), 3, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray10) {

	zval *index_param = NULL, *_0, *_1;
	zval *index = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	zephir_get_strval(index, index_param);


	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "hello", 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	array_init(_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray11) {

	zval *index_param = NULL, *_0, *_1;
	zval *index = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	zephir_get_strval(index, index_param);


	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	zephir_update_property_this(this_ptr, SL("myArray"), _0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("zz"), 3, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_STRING(_1, "hello", 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	ZEPHIR_INIT_BNVAR(_1);
	array_init(_1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zz"), 3, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testPropertyArray12) {

	zval *index, *_0, *_1, *_2, *_3, *_4;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index);



	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_0 TSRMLS_CC, SL("zza"), 2, index, index);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_DOUBLE(_1, 1.5);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_1 TSRMLS_CC, SL("zza"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_null) TSRMLS_CC, SL("zza"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_false) TSRMLS_CC, SL("zza"), 2, index, index);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &ZEPHIR_GLOBAL(global_true) TSRMLS_CC, SL("zza"), 2, index, index);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, 'A');
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_2 TSRMLS_CC, SL("zza"), 2, index, index);
	ZEPHIR_INIT_VAR(_3);
	ZVAL_STRING(_3, "hello", 1);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_3 TSRMLS_CC, SL("zza"), 2, index, index);
	ZEPHIR_INIT_VAR(_4);
	array_init(_4);
	zephir_update_property_array_multi(this_ptr, SL("myArray"), &_4 TSRMLS_CC, SL("zza"), 2, index, index);
	RETURN_MM_MEMBER(this_ptr, "myArray");

}

PHP_METHOD(Test_Assign, testArrayVarAssign1) {

	zval *index, *value, *a;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &index, &value);



	ZEPHIR_INIT_VAR(a);
	array_init(a);
	zephir_array_update_zval(&a, index, &value, PH_COPY | PH_SEPARATE);
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Assign, testArrayVarAssign2) {

	int _0;
	zval *index, *value, *_POST;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &index, &value);



	ZEPHIR_INIT_VAR(_POST);
	array_init(_POST);
	_0 = zephir_maybe_separate_zval(&_POST);
	zephir_array_update_zval(&_POST, index, &value, PH_COPY | PH_SEPARATE);
	if (_0) {
		ZEND_SET_SYMBOL(&EG(symbol_table), "_POST", _POST);
	}
	RETURN_CCTOR(_POST);

}

PHP_METHOD(Test_Assign, testArrayProperty) {

	int _0;
	zval *index, *value, *_POST;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &index, &value);



	ZEPHIR_INIT_VAR(_POST);
	array_init(_POST);
	_0 = zephir_maybe_separate_zval(&_POST);
	zephir_array_update_zval(&_POST, index, &value, PH_COPY | PH_SEPARATE);
	if (_0) {
		ZEND_SET_SYMBOL(&EG(symbol_table), "_POST", _POST);
	}
	RETURN_CCTOR(_POST);

}

/**
 * @link https://github.com/phalcon/zephir/issues/159
 */
PHP_METHOD(Test_Assign, testGlobalVarAssign) {

	int _0;
	zval *index, *value, *_POST;

	zephir_fetch_params(0, 2, 0, &index, &value);



	zephir_get_global(&_POST, SS("_POST") TSRMLS_CC);
	_0 = zephir_maybe_separate_zval(&_POST);
	zephir_array_update_zval(&_POST, index, &value, PH_COPY | PH_SEPARATE);
	if (_0) {
		ZEND_SET_SYMBOL(&EG(symbol_table), "_POST", _POST);
	}

}

