
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
 * Cast tests
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

PHP_METHOD(Test_Cast, testIntCastFromStringValue) {

	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_STRING(_0, "test", 1);
	RETURN_MM_LONG(zephir_get_intval_ex(_0));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableString) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "test", 1);
	RETURN_MM_LONG(zephir_get_intval(a));

}

PHP_METHOD(Test_Cast, testIntCastFromParameterString) {

	zval *a_param = NULL;
	zval *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	zephir_get_strval(a, a_param);


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

	zval *a = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	ZEPHIR_INIT_NVAR(a);
	zephir_create_array(a, 4, 0 TSRMLS_CC);
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
	zephir_array_fast_append(a, _0);
	RETURN_MM_LONG(zephir_get_intval(a));

}

PHP_METHOD(Test_Cast, testIntCastFromArray) {

	zval *_1 = NULL;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
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

/** To float cast */
PHP_METHOD(Test_Cast, testFloatCastFromFloat) {


	RETURN_DOUBLE(5.0);

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableFloat) {

	double a = 5.0;


	RETURN_DOUBLE((double) (a));

}

PHP_METHOD(Test_Cast, testFloatCastFromBooleanTrue) {


	RETURN_DOUBLE(1);

}

PHP_METHOD(Test_Cast, testFloatCastFromBooleanFalse) {


	RETURN_DOUBLE(0);

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableBooleanTrue) {

	zend_bool a = 1;


	RETURN_DOUBLE((double) (a));

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableBooleanFalse) {

	zend_bool a = 0;


	RETURN_DOUBLE((double) (a));

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableNull) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	ZVAL_NULL(a);

	RETURN_MM_DOUBLE(zephir_get_doubleval(a));

}

PHP_METHOD(Test_Cast, testFloatCastFromNull) {


	RETURN_DOUBLE(0);

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableEmptyArray) {

	zval *a;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	RETURN_MM_DOUBLE(zephir_get_doubleval(a));

}

PHP_METHOD(Test_Cast, testFloatCastFromEmptyArray) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(_0));

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableArray) {

	zval *a = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	ZEPHIR_INIT_NVAR(a);
	zephir_create_array(a, 4, 0 TSRMLS_CC);
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
	zephir_array_fast_append(a, _0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(a));

}

PHP_METHOD(Test_Cast, testFloatCastFromArray) {

	zval *_1 = NULL;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 4);
	zephir_array_fast_append(_0, _1);
	RETURN_MM_DOUBLE(zephir_get_doubleval(_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testFloatCastFromStdClass) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	object_init(_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testFloatCastFromVariableStdClass) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	object_init(a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(a));

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

	zval *a = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	ZEPHIR_INIT_NVAR(a);
	zephir_create_array(a, 4, 0 TSRMLS_CC);
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

/** To object cast */
PHP_METHOD(Test_Cast, testObjectCastFromInt) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 5);
	zephir_convert_to_object(_0);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromFloat) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, 5.0);
	zephir_convert_to_object(_0);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromFalse) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, 0);
	zephir_convert_to_object(_0);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromTrue) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_BOOL(_0, 1);
	zephir_convert_to_object(_0);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromNull) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_NULL(_0);
	zephir_convert_to_object(_0);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromEmptyArray) {

	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	array_init(_0);
	ZEPHIR_CPY_WRT(_1, _0);
	zephir_convert_to_object(_1);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_Cast, testObjectCastFromArray) {

	zval *_1 = NULL, *_2 = NULL;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 4);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_CPY_WRT(_2, _0);
	zephir_convert_to_object(_2);
	RETURN_CCTOR(_2);

}

PHP_METHOD(Test_Cast, testObjectCastFromEmptyString) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "", 1);
	zephir_convert_to_object(_0);
	RETURN_CCTOR(_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromString) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZVAL_STRING(_0, "test string", 1);
	zephir_convert_to_object(_0);
	RETURN_CCTOR(_0);

}

