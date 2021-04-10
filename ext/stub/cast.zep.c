
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/array.h"


/**
 * Cast tests
 */
ZEPHIR_INIT_CLASS(Stub_Cast)
{
	ZEPHIR_REGISTER_CLASS(Stub, Cast, stub, cast, stub_cast_method_entry, 0);

	return SUCCESS;
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testCharCastFromChar)
{
	zval *this_ptr = getThis();



	RETURN_LONG('a');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testCharCastFromVariableChar)
{
	char a, _0;
	zval *this_ptr = getThis();




	a = 'A';
	_0 = a;
	RETURN_LONG(_0);
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testStringCastChar)
{
	zval *this_ptr = getThis();



	RETURN_STRING("z");
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testStringCastVariableChar)
{
	zval _0;
	char a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	a = 'X';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRINGL(&_0, &a, 1);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Cast, testIntCastFromFloat)
{
	zval *this_ptr = getThis();



	RETURN_LONG((int) 5.0);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableFloat)
{
	double a;
	zval *this_ptr = getThis();




	a = 5.0;
	RETURN_LONG((int) (a));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testIntCastFromChar)
{
	zval *this_ptr = getThis();



	RETURN_LONG('A');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testIntCastFromVariableChar)
{
	char a;
	zval *this_ptr = getThis();




	a = 'a';
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Cast, testIntCastFromBooleanTrue)
{
	zval *this_ptr = getThis();



	RETURN_LONG(1);
}

PHP_METHOD(Stub_Cast, testIntCastFromBooleanFalse)
{
	zval *this_ptr = getThis();



	RETURN_LONG(0);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableBooleanTrue)
{
	zend_bool a;
	zval *this_ptr = getThis();




	a = 1;
	RETURN_LONG((int) (a));
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableBooleanFalse)
{
	zend_bool a;
	zval *this_ptr = getThis();




	a = 0;
	RETURN_LONG((int) (a));
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableNull)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromStringValue)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "test");
	RETURN_MM_LONG(zephir_get_intval_ex(&_0));
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableString)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "test");
	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromParameterString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL;
	zval a;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(a)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_strval(&a, a_param);


	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromNull)
{
	zval *this_ptr = getThis();



	RETURN_LONG(0);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableEmptyArray)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromEmptyArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	RETURN_MM_LONG(zephir_get_intval(&_0));
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableArray)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 4, 0);
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
	zephir_array_fast_append(&a, &_0);
	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromArray)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 4, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	RETURN_MM_LONG(zephir_get_intval(&_0));
}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Stub_Cast, testIntCastFromStdClass)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	object_init(&_0);
	RETURN_MM_LONG(zephir_get_intval(&_0));
}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Stub_Cast, testIntCastFromVariableStdClass)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	object_init(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testLongCastFromChar)
{
	zval *this_ptr = getThis();



	RETURN_LONG((long) 'a');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testLongCastFromVariableChar)
{
	char a;
	zval *this_ptr = getThis();




	a = 'A';
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Cast, testFloatCastFromFloat)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(5.0);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableFloat)
{
	double a;
	zval *this_ptr = getThis();




	a = 5.0;
	RETURN_DOUBLE((double) (a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromBooleanTrue)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(1);
}

PHP_METHOD(Stub_Cast, testFloatCastFromBooleanFalse)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(0);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableBooleanTrue)
{
	zend_bool a;
	zval *this_ptr = getThis();




	a = 1;
	RETURN_DOUBLE((double) (a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableBooleanFalse)
{
	zend_bool a;
	zval *this_ptr = getThis();




	a = 0;
	RETURN_DOUBLE((double) (a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableNull)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromNull)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(0);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableEmptyArray)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromEmptyArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&_0));
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableArray)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 4, 0);
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
	zephir_array_fast_append(&a, &_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromArray)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 4, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&_0));
}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Stub_Cast, testFloatCastFromStdClass)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	object_init(&_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&_0));
}

/**
 * Notice: Object of class stdClass could not be converted to int
 * @return int 1
 */
PHP_METHOD(Stub_Cast, testFloatCastFromVariableStdClass)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	object_init(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testDoubleCastFromVChar)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE((double) 'a');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testDoubleCastFromVariableChar)
{
	char a;
	zval *this_ptr = getThis();




	a = 'A';
	RETURN_DOUBLE((double) a);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromIntTrue1)
{
	zval *this_ptr = getThis();



	RETURN_BOOL((zend_bool) 1);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromIntTrue2)
{
	zval *this_ptr = getThis();



	RETURN_BOOL((zend_bool) 1000);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromIntFalse)
{
	zval *this_ptr = getThis();



	RETURN_BOOL((zend_bool) 0);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromObject)
{
	zval simpleObject;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&simpleObject);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&simpleObject);
	object_init(&simpleObject);
	RETURN_MM_BOOL(zephir_get_boolval(&simpleObject));
}

PHP_METHOD(Stub_Cast, testBooleanCastFromEmptyArray)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_MM_BOOL(zephir_get_boolval(&a));
}

PHP_METHOD(Stub_Cast, testBooleanCastFromArray)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 4, 0);
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
	zephir_array_fast_append(&a, &_0);
	RETURN_MM_BOOL(zephir_get_boolval(&a));
}

PHP_METHOD(Stub_Cast, testBooleanCastFromNull)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_MM_BOOL(zephir_get_boolval(&a));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testBooleanCastFromChar)
{
	zval *this_ptr = getThis();



	RETURN_BOOL((zend_bool) 'a');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testBooleanCastFromVariableChar)
{
	char a;
	zval *this_ptr = getThis();




	a = 'A';
	RETURN_BOOL((zend_bool) a);
}

PHP_METHOD(Stub_Cast, testObjectCastFromInt)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromFloat)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 5.0);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromFalse)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 0);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromTrue)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromNull)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromEmptyArray)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	ZEPHIR_CPY_WRT(&_1, &_0);
	zephir_convert_to_object(&_1);
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Cast, testObjectCastFromArray)
{
	zval _1, _2;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 4, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 4);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_CPY_WRT(&_2, &_0);
	zephir_convert_to_object(&_2);
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_Cast, testObjectCastFromEmptyString)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "");
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromString)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "test string");
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testCastStdinToInteger)
{
	zval handle;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&handle);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&handle);
	ZEPHIR_GET_CONSTANT(&handle, "STDIN");
	RETURN_MM_LONG(zephir_get_intval(&handle));
}

PHP_METHOD(Stub_Cast, testCastStdoutToInteger)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_GET_CONSTANT(&_0, "STDOUT");
	RETURN_MM_LONG(zephir_get_intval(&_0));
}

PHP_METHOD(Stub_Cast, testCastFileResourceToInteger)
{
	zend_long id = 0;
	zval *fileName, fileName_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&fileName_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(fileName)
	ZEND_PARSE_PARAMETERS_END();
#endif


	zephir_fetch_params_without_memory_grow(1, 0, &fileName);


	id = zephir_get_intval(fileName);
	RETURN_LONG(id);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableArray)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	zephir_create_array(&uids, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&uids, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "2");
	zephir_array_fast_append(&uids, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 3);
	zephir_array_fast_append(&uids, &_0);
	zephir_get_arrval(&_1, &uids);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableTrue)
{
	zval _0;
	zval uids;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_BOOL(&uids, 1);
	zephir_get_arrval(&_0, &uids);
	ZEPHIR_CPY_WRT(&uids, &_0);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableFalse)
{
	zval _0;
	zval uids;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_BOOL(&uids, 0);
	zephir_get_arrval(&_0, &uids);
	ZEPHIR_CPY_WRT(&uids, &_0);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableNull)
{
	zval _0;
	zval uids;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_NULL(&uids);
	zephir_get_arrval(&_0, &uids);
	ZEPHIR_CPY_WRT(&uids, &_0);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableInteger)
{
	zval _0;
	zval uids;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_LONG(&uids, 1);
	zephir_get_arrval(&_0, &uids);
	ZEPHIR_CPY_WRT(&uids, &_0);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableFloat)
{
	zval _0;
	zval uids;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_DOUBLE(&uids, 1.1);
	zephir_get_arrval(&_0, &uids);
	ZEPHIR_CPY_WRT(&uids, &_0);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableString)
{
	zval _0;
	zval uids;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_STRING(&uids, "aaa");
	zephir_get_arrval(&_0, &uids);
	ZEPHIR_CPY_WRT(&uids, &_0);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableStdClass)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&uids);
	object_init(&uids);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "v1");
	zephir_update_property_zval(&uids, ZEND_STRL("p1"), &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "v2");
	zephir_update_property_zval(&uids, ZEND_STRL("p2"), &_0);
	zephir_get_arrval(&_1, &uids);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testIssue828)
{
	zend_long intVersion = 0, intNonNumeric = 0;
	double floatVersion = 0, floatNonNumeric = 0;
	zval version, nonNumericString, _0;
	zval ret;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&version);
	ZVAL_UNDEF(&nonNumericString);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&ret);
	array_init(&ret);
	ZEPHIR_INIT_VAR(&version);
	ZVAL_STRING(&version, "1.0 200 OK");
	ZEPHIR_INIT_VAR(&nonNumericString);
	ZVAL_STRING(&nonNumericString, "OK");
	zephir_array_append(&ret, &version, PH_SEPARATE, "stub/cast.zep", 491);
	zephir_array_append(&ret, &nonNumericString, PH_SEPARATE, "stub/cast.zep", 492);
	floatVersion = zephir_get_doubleval(&version);
	intVersion = zephir_get_intval(&version);
	floatNonNumeric = zephir_get_doubleval(&nonNumericString);
	intNonNumeric = zephir_get_intval(&nonNumericString);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, floatVersion);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 499);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, intVersion);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 500);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, floatNonNumeric);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 501);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, intNonNumeric);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 502);
	RETURN_CTOR(&ret);
}

