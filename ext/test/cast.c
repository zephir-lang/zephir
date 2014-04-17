
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/array.h"


/**
 * X
 */
ZEPHIR_INIT_CLASS(Test_Cast) {

	ZEPHIR_REGISTER_CLASS(Test, Cast, test, cast, test_cast_method_entry, 0);

	return SUCCESS;

}

/** To int cast */
PHP_METHOD(Test_Cast, testIntCastFromFloat) {


	RETURN_LONG((int) 5.0);

}

PHP_METHOD(Test_Cast, testIntCastFromVariableFloat) {

	double a = 5.0;


	RETURN_LONG((int) (a));

}

PHP_METHOD(Test_Cast, testIntCastFromBooleanTrue) {


	RETURN_LONG(1);

}

PHP_METHOD(Test_Cast, testIntCastFromBooleanFalse) {


	RETURN_LONG(0);

}

PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanTrue) {

	zend_bool a = 1;


	RETURN_LONG((int) (a));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanFalse) {

	zend_bool a = 0;


	RETURN_LONG((int) (a));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableNull) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	ZVAL_NULL(a);

	RETURN_MM_LONG(zephir_get_intval(a));

}

PHP_METHOD(Test_Cast, testIntCastFromNull) {


	RETURN_LONG(0);

}

PHP_METHOD(Test_Cast, testIntCastFromVariableEmptyArray) {

	zval *a;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	RETURN_MM_LONG(zephir_get_intval(a));

}

PHP_METHOD(Test_Cast, testIntCastFromEmptyArray) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	RETURN_MM_LONG(zephir_get_intval(_0));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableArray) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	ZEPHIR_INIT_VAR(a);
	array_init_size(a, 7);
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
	zephir_array_fast_append(a, _0);
	RETURN_MM_LONG(zephir_get_intval(a));

}

PHP_METHOD(Test_Cast, testIntCastFromArray) {

	zval *_1;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init_size(_0, 7);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, 4);
	zephir_array_fast_append(_0, _1);
	RETURN_MM_LONG(zephir_get_intval(_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testIntCastFromStdClass) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	object_init(_0);
	RETURN_MM_LONG(zephir_get_intval(_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testIntCastFromVariableStdClass) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	object_init(a);
	RETURN_MM_LONG(zephir_get_intval(a));

}

/** To boolean cast */
PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue1) {


	RETURN_BOOL((zend_bool) 1);

}

PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue2) {


	RETURN_BOOL((zend_bool) 1000);

}

PHP_METHOD(Test_Cast, testBooleanCastFromIntFalse) {


	RETURN_BOOL((zend_bool) 0);

}

PHP_METHOD(Test_Cast, testBooleanCastFromObject) {

	zval *simpleObject;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(simpleObject);
	object_init(simpleObject);
	RETURN_MM_BOOL(zephir_get_boolval(simpleObject));

}

PHP_METHOD(Test_Cast, testBooleanCastFromEmptyArray) {

	zval *a;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	RETURN_MM_BOOL(zephir_get_boolval(a));

}

PHP_METHOD(Test_Cast, testBooleanCastFromArray) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	ZEPHIR_INIT_VAR(a);
	array_init_size(a, 7);
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
	zephir_array_fast_append(a, _0);
	RETURN_MM_BOOL(zephir_get_boolval(a));

}

PHP_METHOD(Test_Cast, testBooleanCastFromNull) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	ZVAL_NULL(a);

	RETURN_MM_BOOL(zephir_get_boolval(a));

}

