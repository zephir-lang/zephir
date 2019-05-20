
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
 * Cast tests
 */
ZEPHIR_INIT_CLASS(Test_Cast) {

	ZEPHIR_REGISTER_CLASS(Test, Cast, test, cast, test_cast_method_entry, 0);

	return SUCCESS;

}

/** To int cast */
PHP_METHOD(Test_Cast, testIntCastFromFloat) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG((int) 5.0);

}

PHP_METHOD(Test_Cast, testIntCastFromVariableFloat) {

	double a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5.0;
	RETURN_MM_LONG((int) (a));

}

PHP_METHOD(Test_Cast, testIntCastFromBooleanTrue) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG(1);

}

PHP_METHOD(Test_Cast, testIntCastFromBooleanFalse) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanTrue) {

	zend_bool a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	RETURN_MM_LONG((int) (a));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableBooleanFalse) {

	zend_bool a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 0;
	RETURN_MM_LONG((int) (a));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableNull) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	ZVAL_NULL(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));

}

PHP_METHOD(Test_Cast, testIntCastFromStringValue) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_ZVAL_STRING(&_0, "test");
	RETURN_MM_LONG(zephir_get_intval_ex(&_0));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableString) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	ZEPHIR_MM_ZVAL_STRING(&a, "test");
	RETURN_MM_LONG(zephir_get_intval(&a));

}

PHP_METHOD(Test_Cast, testIntCastFromParameterString) {

	zval *a_param = NULL;
	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);

	zephir_fetch_params(1, 1, 0, &a_param);

	zephir_get_strval(&a, a_param);


	RETURN_MM_LONG(zephir_get_intval(&a));

}

PHP_METHOD(Test_Cast, testIntCastFromNull) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Cast, testIntCastFromVariableEmptyArray) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	array_init(&a);
	ZEPHIR_MM_ADD_ENTRY(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));

}

PHP_METHOD(Test_Cast, testIntCastFromEmptyArray) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	array_init(&_0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	RETURN_MM_LONG(zephir_get_intval(&_0));

}

PHP_METHOD(Test_Cast, testIntCastFromVariableArray) {

	zval a, _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	zephir_create_array(&a, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&a);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&a, &_0);
	RETURN_MM_LONG(zephir_get_intval(&a));

}

PHP_METHOD(Test_Cast, testIntCastFromArray) {

	zval _1;
	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	zephir_create_array(&_0, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	RETURN_MM_LONG(zephir_get_intval(&_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testIntCastFromStdClass) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	object_init(&_0);
	RETURN_MM_LONG(zephir_get_intval(&_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testIntCastFromVariableStdClass) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	object_init(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));

}

/** To float cast */
PHP_METHOD(Test_Cast, testFloatCastFromFloat) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_DOUBLE(5.0);

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableFloat) {

	double a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 5.0;
	RETURN_MM_DOUBLE((double) (a));

}

PHP_METHOD(Test_Cast, testFloatCastFromBooleanTrue) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_DOUBLE(1);

}

PHP_METHOD(Test_Cast, testFloatCastFromBooleanFalse) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_DOUBLE(0);

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableBooleanTrue) {

	zend_bool a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 1;
	RETURN_MM_DOUBLE((double) (a));

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableBooleanFalse) {

	zend_bool a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	a = 0;
	RETURN_MM_DOUBLE((double) (a));

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableNull) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	ZVAL_NULL(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));

}

PHP_METHOD(Test_Cast, testFloatCastFromNull) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_DOUBLE(0);

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableEmptyArray) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	array_init(&a);
	ZEPHIR_MM_ADD_ENTRY(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));

}

PHP_METHOD(Test_Cast, testFloatCastFromEmptyArray) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	array_init(&_0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&_0));

}

PHP_METHOD(Test_Cast, testFloatCastFromVariableArray) {

	zval a, _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	zephir_create_array(&a, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&a);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&a, &_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));

}

PHP_METHOD(Test_Cast, testFloatCastFromArray) {

	zval _1;
	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	zephir_create_array(&_0, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testFloatCastFromStdClass) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	object_init(&_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&_0));

}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Test_Cast, testFloatCastFromVariableStdClass) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	object_init(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));

}

/** To boolean cast */
PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue1) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_BOOL((zend_bool) 1);

}

PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue2) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_BOOL((zend_bool) 1000);

}

PHP_METHOD(Test_Cast, testBooleanCastFromIntFalse) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_BOOL((zend_bool) 0);

}

PHP_METHOD(Test_Cast, testBooleanCastFromObject) {

	zval simpleObject;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&simpleObject);


	object_init(&simpleObject);
	RETURN_MM_BOOL(zephir_get_boolval(&simpleObject));

}

PHP_METHOD(Test_Cast, testBooleanCastFromEmptyArray) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	array_init(&a);
	ZEPHIR_MM_ADD_ENTRY(&a);
	RETURN_MM_BOOL(zephir_get_boolval(&a));

}

PHP_METHOD(Test_Cast, testBooleanCastFromArray) {

	zval a, _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	zephir_create_array(&a, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&a);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&a, &_0);
	ZVAL_LONG(&_0, 4);
	zephir_array_fast_append(&a, &_0);
	RETURN_MM_BOOL(zephir_get_boolval(&a));

}

PHP_METHOD(Test_Cast, testBooleanCastFromNull) {

	zval a;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	ZVAL_NULL(&a);
	RETURN_MM_BOOL(zephir_get_boolval(&a));

}

/** To object cast */
PHP_METHOD(Test_Cast, testObjectCastFromInt) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZVAL_LONG(&_0, 5);
	zephir_convert_to_object(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromFloat) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZVAL_DOUBLE(&_0, 5.0);
	zephir_convert_to_object(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromFalse) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZVAL_BOOL(&_0, 0);
	zephir_convert_to_object(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromTrue) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZVAL_BOOL(&_0, 1);
	zephir_convert_to_object(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromNull) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZVAL_NULL(&_0);
	zephir_convert_to_object(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromEmptyArray) {

	zval _0, _1;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	array_init(&_0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZEPHIR_CPY_WRT(&_1, &_0);
	zephir_convert_to_object(&_1);
	RETURN_MM_CTOR(&_1);

}

PHP_METHOD(Test_Cast, testObjectCastFromArray) {

	zval _1, _2;
	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);


	zephir_create_array(&_0, 4, 0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_CPY_WRT(&_2, &_0);
	zephir_convert_to_object(&_2);
	RETURN_MM_CTOR(&_2);

}

PHP_METHOD(Test_Cast, testObjectCastFromEmptyString) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_ZVAL_STRING(&_0, "");
	zephir_convert_to_object(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Cast, testObjectCastFromString) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_ZVAL_STRING(&_0, "test string");
	zephir_convert_to_object(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Cast, testCastStdinToInteger) {

	zval handle;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&handle);


	ZEPHIR_GET_CONSTANT(&handle, "STDIN");
	RETURN_MM_LONG(zephir_get_intval(&handle));

}

PHP_METHOD(Test_Cast, testCastStdoutToInteger) {

	zval _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);


	ZEPHIR_GET_CONSTANT(&_0, "STDOUT");
	RETURN_MM_LONG(zephir_get_intval(&_0));

}

PHP_METHOD(Test_Cast, testCastFileResourceToInteger) {

	zend_long id = 0;
	zval *fileName, fileName_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&fileName_sub);

	zephir_fetch_params(0, 1, 0, &fileName);



	id = zephir_get_intval(fileName);
	RETURN_MM_LONG(id);

}

