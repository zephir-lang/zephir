
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
#include "kernel/fcall.h"


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

	RETURN_LONG((unsigned char) ('a'));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testCharCastFromVariableChar)
{
	char a;
	a = 'A';
	RETURN_LONG((unsigned char) (a));
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testStringCastChar)
{

	RETURN_STRING("z");
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testStringCastVariableChar)
{
	zval _0;
	char a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	a = 'X';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRINGL(&_0, &a, 1);
	RETURN_CTOR(&_0);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/2232
 */
PHP_METHOD(Stub_Cast, testStringCastFromNull)
{
	zval _0;
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	zephir_cast_to_string(&_0, &a);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Cast, testIntCastFromFloat)
{

	RETURN_LONG((int) 5.0);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableFloat)
{
	double a;
	a =  (5.0);
	RETURN_LONG((int) a);
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testIntCastFromChar)
{

	RETURN_LONG('A');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testIntCastFromVariableChar)
{
	char a;
	a = 'a';
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Cast, testIntCastFromBooleanTrue)
{

	RETURN_LONG(1);
}

PHP_METHOD(Stub_Cast, testIntCastFromBooleanFalse)
{

	RETURN_LONG(0);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableBooleanTrue)
{
	zend_bool a;
	a = 1;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableBooleanFalse)
{
	zend_bool a;
	a = 0;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableNull)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromStringValue)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "test");
	RETURN_MM_LONG(zephir_get_intval(&_0));
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableString)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "test");
	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromParameterString)
{
	zval a_zv;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(a)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&a_zv, a);
	RETURN_LONG(zephir_get_intval(&a_zv));
}

PHP_METHOD(Stub_Cast, testIntCastFromNull)
{

	RETURN_LONG(0);
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableEmptyArray)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));
}

PHP_METHOD(Stub_Cast, testIntCastFromEmptyArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	RETURN_MM_LONG(zephir_get_intval(&_0));
}

PHP_METHOD(Stub_Cast, testIntCastFromVariableArray)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	object_init(&a);
	RETURN_MM_LONG(zephir_get_intval(&a));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testLongCastFromChar)
{

	RETURN_LONG('a');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testLongCastFromVariableChar)
{
	char a;
	a = 'A';
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Cast, testFloatCastFromFloat)
{

	RETURN_DOUBLE(5.0);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableFloat)
{
	double a;
	a =  (5.0);
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Cast, testFloatCastFromBooleanTrue)
{

	RETURN_DOUBLE(1);
}

PHP_METHOD(Stub_Cast, testFloatCastFromBooleanFalse)
{

	RETURN_DOUBLE(0);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableBooleanTrue)
{
	zend_bool a;
	a = 1;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableBooleanFalse)
{
	zend_bool a;
	a = 0;
	RETURN_DOUBLE(a);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableNull)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromNull)
{

	RETURN_DOUBLE(0);
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableEmptyArray)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));
}

PHP_METHOD(Stub_Cast, testFloatCastFromEmptyArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&_0));
}

PHP_METHOD(Stub_Cast, testFloatCastFromVariableArray)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	object_init(&a);
	RETURN_MM_DOUBLE(zephir_get_doubleval(&a));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testDoubleCastFromVChar)
{

	RETURN_DOUBLE((double) 'a');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testDoubleCastFromVariableChar)
{
	char a;
	a = 'A';
	RETURN_DOUBLE((double) a);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromIntTrue1)
{

	RETURN_BOOL((zend_bool) 1);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromIntTrue2)
{

	RETURN_BOOL((zend_bool) 1000);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromIntFalse)
{

	RETURN_BOOL((zend_bool) 0);
}

PHP_METHOD(Stub_Cast, testBooleanCastFromObject)
{
	zval simpleObject;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&simpleObject);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&simpleObject);
	object_init(&simpleObject);
	RETURN_MM_BOOL(zephir_get_boolval(&simpleObject));
}

PHP_METHOD(Stub_Cast, testBooleanCastFromEmptyArray)
{
	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	RETURN_MM_BOOL(zephir_get_boolval(&a));
}

PHP_METHOD(Stub_Cast, testBooleanCastFromArray)
{
	zval a, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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

	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_MM_BOOL(zephir_get_boolval(&a));
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testBooleanCastFromChar)
{

	RETURN_BOOL((zend_bool) 'a');
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1988
 */
PHP_METHOD(Stub_Cast, testBooleanCastFromVariableChar)
{
	char a;
	a = 'A';
	RETURN_BOOL((zend_bool) a);
}

PHP_METHOD(Stub_Cast, testObjectCastFromInt)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromFloat)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, 5.0);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromFalse)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 0);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromTrue)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromNull)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromEmptyArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromArray)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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
	zephir_convert_to_object(&_0);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromEmptyString)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "");
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testObjectCastFromString)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "test string");
	zephir_convert_to_object(&_0);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Cast, testCastStdinToInteger)
{
	zval handle;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&handle);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&handle);
	ZEPHIR_GET_CONSTANT(&handle, "STDIN");
	RETURN_MM_LONG(zephir_get_intval(&handle));
}

PHP_METHOD(Stub_Cast, testCastStdoutToInteger)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_GET_CONSTANT(&_0, "STDOUT");
	RETURN_MM_LONG(zephir_get_intval(&_0));
}

PHP_METHOD(Stub_Cast, testCastFileResourceToInteger)
{
	zend_long id = 0;
	zval *fileName, fileName_sub;

	ZVAL_UNDEF(&fileName_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(fileName)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &fileName);
	id = zephir_get_intval(fileName);
	RETURN_LONG(id);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableArray)
{
	zval _2;
	zval uids, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

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
	ZEPHIR_CPY_WRT(&_1, &uids);
	zephir_get_arrval(&_2, &_1);
	ZEPHIR_CPY_WRT(&uids, &_2);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableTrue)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_BOOL(&uids, 1);
	ZEPHIR_CPY_WRT(&_0, &uids);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableFalse)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_BOOL(&uids, 0);
	ZEPHIR_CPY_WRT(&_0, &uids);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableNull)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_NULL(&uids);
	ZEPHIR_CPY_WRT(&_0, &uids);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableInteger)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_LONG(&uids, 1);
	ZEPHIR_CPY_WRT(&_0, &uids);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableFloat)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_DOUBLE(&uids, 1.1);
	ZEPHIR_CPY_WRT(&_0, &uids);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableString)
{
	zval _1;
	zval uids, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&uids);
	ZVAL_STRING(&uids, "aaa");
	ZEPHIR_CPY_WRT(&_0, &uids);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&uids, &_1);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testArrayCastFromVariableStdClass)
{
	zval _2;
	zval uids, _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&uids);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("p1", 2, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("p2", 2, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&uids);
	object_init(&uids);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "v1");
	zephir_update_property_zval_cached(&uids, _zephir_prop_0, 0, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "v2");
	zephir_update_property_zval_cached(&uids, _zephir_prop_1, 0, &_0);
	ZEPHIR_CPY_WRT(&_1, &uids);
	zephir_get_arrval(&_2, &_1);
	ZEPHIR_CPY_WRT(&uids, &_2);
	RETURN_CCTOR(&uids);
}

PHP_METHOD(Stub_Cast, testIssue828)
{
	zend_long intVersion = 0, intNonNumeric = 0;
	double floatVersion = 0, floatNonNumeric = 0;
	zval version, nonNumericString, _0;
	zval ret;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&version);
	ZVAL_UNDEF(&nonNumericString);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&ret);
	array_init(&ret);
	ZEPHIR_INIT_VAR(&version);
	ZVAL_STRING(&version, "1.0 200 OK");
	ZEPHIR_INIT_VAR(&nonNumericString);
	ZVAL_STRING(&nonNumericString, "OK");
	zephir_array_append(&ret, &version, PH_SEPARATE, "stub/cast.zep", 501);
	zephir_array_append(&ret, &nonNumericString, PH_SEPARATE, "stub/cast.zep", 502);
	floatVersion =  (zephir_get_doubleval(&version));
	intVersion = zephir_get_intval(&version);
	floatNonNumeric =  (zephir_get_doubleval(&nonNumericString));
	intNonNumeric = zephir_get_intval(&nonNumericString);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, floatVersion);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 509);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, intVersion);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 510);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, floatNonNumeric);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 511);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, intNonNumeric);
	zephir_array_append(&ret, &_0, PH_SEPARATE, "stub/cast.zep", 512);
	RETURN_CTOR(&ret);
}

/**
 * Reassigning a variable whose first inferred type comes from an
 * `(object)` cast used to make the static type-inference pass leak
 * `StaticTypeInference=object undefined` to stdout during compilation.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1877
 */
PHP_METHOD(Stub_Cast, testObjectCastThenReassign)
{
	zval _0;
	zval data;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&data);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	add_assoc_stringl_ex(&_0, SL("key"), SL("value"));
	zephir_convert_to_object(&_0);
	ZEPHIR_CPY_WRT(&data, &_0);
	ZEPHIR_INIT_NVAR(&data);
	ZVAL_STRING(&data, "reassigned");
	RETURN_CCTOR(&data);
}

/**
 * Every cast target has to accept every source form, not just the handful
 * of literal types the cast operator used to enumerate. Each method below
 * walks one target across: literals, native-typed locals, an `array` local,
 * a compound expression, a method call and a genuinely dynamic parameter.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToInt)
{
	zval arr, _2;
	char c;
	zval s;
	zend_bool b;
	double d;
	long l;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i;
	zval *dyn, dyn_sub, _0, _1, _3, _4;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	l = 6;
	d =  (5.5);
	b = 1;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "12abc");
	c = 'z';
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_create_array(return_value, 19, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (int) 5.5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "12abc");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(&_1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(&_2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	array_init(&_0);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, zephir_get_intval(&_0));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, i);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, l);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, (int) d);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, b);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, zephir_get_intval(&s));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, c);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, zephir_get_intval(&arr));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, ((i + 1)));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, zephir_get_intval(dyn));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "issue1841helper", NULL, 36);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, zephir_get_intval(&_4));
	zephir_array_fast_append(return_value, &_3);
	RETURN_MM();
}

/**
 * `uint`/`ulong` had no branch at all, so every source failed to compile.
 * Only non-negative values are exercised: the C width of `unsigned long`
 * differs between LP64 and LLP64, so wraparound is not portable.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToUnsigned)
{
	char c;
	zval s;
	double d;
	zend_long i;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *dyn, dyn_sub, _0, _1, _2;

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&s);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	d =  (5.5);
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "12abc");
	c = 'z';
	zephir_create_array(return_value, 19, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) 5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) 5.5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "12abc");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) zephir_get_intval(&_1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) i);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) d);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) zephir_get_intval(&s));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) c);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (zend_ulong) zephir_get_intval(dyn));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned long) 5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned long) 5.5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "12abc");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned long) zephir_get_intval(&_2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned long) i);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned long) c);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned long) zephir_get_intval(dyn));
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToLong)
{
	zval arr, _2;
	char c;
	zval s;
	zend_bool b;
	double d;
	long l;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i;
	zval *dyn, dyn_sub, _0, _1, _3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	l = 6;
	d =  (5.5);
	b = 1;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "12abc");
	c = 'z';
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_create_array(return_value, 17, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (long) 5.5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "12abc");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(&_1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(&_2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, i);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, l);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (long) d);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(&s));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, c);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(&arr));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, ((i + 1)));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(dyn));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "issue1841helper", NULL, 36);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_get_intval(&_3));
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToFloat)
{
	zval arr, _2;
	char c;
	zval s;
	zend_bool b;
	double d;
	long l;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i;
	zval *dyn, dyn_sub, _0, _1, _3, _4;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	l = 6;
	d =  (5.5);
	b = 1;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "12abc");
	c = 'z';
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_create_array(return_value, 19, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, (double) 5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 5.5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "5.5abc");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, zephir_get_doubleval(&_1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, (double) 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_2, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, zephir_get_doubleval(&_2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	array_init(&_0);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_DOUBLE(&_3, zephir_get_doubleval(&_0));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, (double) i);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, (double) l);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, d);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, b);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, zephir_get_doubleval(&s));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, (double) c);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, zephir_get_doubleval(&arr));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, (double) ((i + 1)));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, zephir_get_doubleval(dyn));
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_CALL_METHOD(&_4, this_ptr, "issue1841helper", NULL, 36);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_DOUBLE(&_3, zephir_get_doubleval(&_4));
	zephir_array_fast_append(return_value, &_3);
	RETURN_MM();
}

/**
 * A C cast truncates, so `(bool) 0.4` must not go through one: PHP treats
 * every non-zero double as true.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToBool)
{
	zval arr, _4;
	char c;
	zval s, zero;
	zend_bool b;
	double d;
	long l;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i;
	zval *dyn, dyn_sub, _0, _1, _2, _3, _5;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&zero);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	l = 6;
	d =  (5.5);
	b = 1;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "12abc");
	ZEPHIR_INIT_VAR(&zero);
	ZVAL_STRING(&zero, "0");
	c = 'z';
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_create_array(return_value, 23, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) (0 != (0.4)));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) (0 != (0.0)));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "0");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&_1));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRING(&_2, "");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&_2));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "abc");
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&_3));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_4);
	zephir_create_array(&_4, 2, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_4, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_4, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&_4));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	array_init(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&_0));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) i);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) l);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) (0 != (d)));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, b);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&s));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&zero));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, (zend_bool) c);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&arr));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(dyn));
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_CALL_METHOD(&_5, this_ptr, "issue1841helper", NULL, 36);
	zephir_check_call_status();
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_BOOL(&_0, zephir_get_boolval(&_5));
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

/**
 * Zephir's `char`/`uchar` is a byte, so a cast to it follows `(int)` and
 * boxes as the integer byte value — PHP has no character type.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1629
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToChar)
{
	unsigned char _7;
	zval s;
	char c, _2, _4, _5, _6;
	zend_long i;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *dyn, dyn_sub, _0, _1, _3;

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&s);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	c = 'z';
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "65");
	zephir_create_array(return_value, 19, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, (char) 65);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (char) 122.9);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (char) 1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "A");
	_2 = (char) zephir_get_intval(&_1);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, _2);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "65");
	_4 = (char) zephir_get_intval(&_3);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, _4);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (char) i);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, c);
	zephir_array_fast_append(return_value, &_0);
	_5 = (char) zephir_get_intval(&s);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, _5);
	zephir_array_fast_append(return_value, &_0);
	_6 = (char) zephir_get_intval(dyn);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, _6);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (char) 321);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned char) 65);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned char) 200);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, (unsigned char) i);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, c);
	zephir_array_fast_append(return_value, &_0);
	_7 = (unsigned char) zephir_get_intval(dyn);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, _7);
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToString)
{
	char c;
	zval s, _1, _3, _5, _7, _9, _11, _14, _16, _18, _20, _21, _22, _24, _25, _27;
	zend_bool b;
	double d;
	long l;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i;
	zval *dyn, dyn_sub, _0, _2, _4, _6, _8, _10, _12, _13, _15, _17, _19, _23, _26;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_12);
	ZVAL_UNDEF(&_13);
	ZVAL_UNDEF(&_15);
	ZVAL_UNDEF(&_17);
	ZVAL_UNDEF(&_19);
	ZVAL_UNDEF(&_23);
	ZVAL_UNDEF(&_26);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_11);
	ZVAL_UNDEF(&_14);
	ZVAL_UNDEF(&_16);
	ZVAL_UNDEF(&_18);
	ZVAL_UNDEF(&_20);
	ZVAL_UNDEF(&_21);
	ZVAL_UNDEF(&_22);
	ZVAL_UNDEF(&_24);
	ZVAL_UNDEF(&_25);
	ZVAL_UNDEF(&_27);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	l = 6;
	d =  (5.5);
	b = 1;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "12abc");
	c = 'z';
	zephir_create_array(return_value, 17, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_cast_to_string(&_1, &_0);
	zephir_array_fast_append(return_value, &_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, 5.0);
	zephir_cast_to_string(&_3, &_2);
	zephir_array_fast_append(return_value, &_3);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_DOUBLE(&_4, 5.5);
	zephir_cast_to_string(&_5, &_4);
	zephir_array_fast_append(return_value, &_5);
	ZEPHIR_INIT_VAR(&_6);
	ZVAL_BOOL(&_6, 1);
	zephir_cast_to_string(&_7, &_6);
	zephir_array_fast_append(return_value, &_7);
	ZEPHIR_INIT_VAR(&_8);
	ZVAL_BOOL(&_8, 0);
	zephir_cast_to_string(&_9, &_8);
	zephir_array_fast_append(return_value, &_9);
	ZEPHIR_INIT_VAR(&_10);
	ZVAL_NULL(&_10);
	zephir_cast_to_string(&_11, &_10);
	zephir_array_fast_append(return_value, &_11);
	ZEPHIR_INIT_VAR(&_12);
	ZVAL_STRING(&_12, "abc");
	zephir_array_fast_append(return_value, &_12);
	ZEPHIR_INIT_NVAR(&_12);
	ZVAL_STRING(&_12, "z");
	zephir_array_fast_append(return_value, &_12);
	ZEPHIR_INIT_VAR(&_13);
	ZVAL_LONG(&_13, i);
	zephir_cast_to_string(&_14, &_13);
	zephir_array_fast_append(return_value, &_14);
	ZEPHIR_INIT_VAR(&_15);
	ZVAL_LONG(&_15, l);
	zephir_cast_to_string(&_16, &_15);
	zephir_array_fast_append(return_value, &_16);
	ZEPHIR_INIT_VAR(&_17);
	ZVAL_DOUBLE(&_17, d);
	zephir_cast_to_string(&_18, &_17);
	zephir_array_fast_append(return_value, &_18);
	ZEPHIR_INIT_VAR(&_19);
	ZVAL_BOOL(&_19, b);
	zephir_cast_to_string(&_20, &_19);
	zephir_array_fast_append(return_value, &_20);
	zephir_cast_to_string(&_21, &s);
	zephir_array_fast_append(return_value, &_21);
	ZEPHIR_INIT_VAR(&_22);
	ZVAL_STRINGL(&_22, &c, 1);
	zephir_array_fast_append(return_value, &_22);
	ZEPHIR_INIT_VAR(&_23);
	ZVAL_LONG(&_23, ((i + 1)));
	zephir_cast_to_string(&_24, &_23);
	zephir_array_fast_append(return_value, &_24);
	zephir_cast_to_string(&_25, dyn);
	zephir_array_fast_append(return_value, &_25);
	ZEPHIR_CALL_METHOD(&_26, this_ptr, "issue1841helper", NULL, 36);
	zephir_check_call_status();
	zephir_cast_to_string(&_27, &_26);
	zephir_array_fast_append(return_value, &_27);
	RETURN_MM();
}

/**
 * Kept apart from issue1841ToString(): PHP raises "Array to string
 * conversion" here, so the caller has to silence it.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToStringFromArray)
{
	zval _2, _3;
	zval _0;
	zval arr, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_1, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_1, &_0);
	zephir_cast_to_string(&_2, &_1);
	zephir_array_fast_append(return_value, &_2);
	zephir_cast_to_string(&_3, &arr);
	zephir_array_fast_append(return_value, &_3);
	RETURN_MM();
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToArray)
{
	zval arr, _2, _4, _6, _8, _10, _12, _13, _15, _17, _19, _21, _23;
	char c;
	zval s;
	zend_long i;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *dyn, dyn_sub, _0, _1, _3, _5, _7, _9, _11, _14, _16, _18, _20, _22;

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&_11);
	ZVAL_UNDEF(&_14);
	ZVAL_UNDEF(&_16);
	ZVAL_UNDEF(&_18);
	ZVAL_UNDEF(&_20);
	ZVAL_UNDEF(&_22);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_12);
	ZVAL_UNDEF(&_13);
	ZVAL_UNDEF(&_15);
	ZVAL_UNDEF(&_17);
	ZVAL_UNDEF(&_19);
	ZVAL_UNDEF(&_21);
	ZVAL_UNDEF(&_23);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "12abc");
	c = 'z';
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_create_array(return_value, 13, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_get_arrval(&_2, &_1);
	zephir_array_fast_append(return_value, &_2);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_DOUBLE(&_3, 5.5);
	zephir_get_arrval(&_4, &_3);
	zephir_array_fast_append(return_value, &_4);
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_BOOL(&_5, 1);
	zephir_get_arrval(&_6, &_5);
	zephir_array_fast_append(return_value, &_6);
	ZEPHIR_INIT_VAR(&_7);
	ZVAL_NULL(&_7);
	zephir_get_arrval(&_8, &_7);
	zephir_array_fast_append(return_value, &_8);
	ZEPHIR_INIT_VAR(&_9);
	ZVAL_STRING(&_9, "abc");
	zephir_get_arrval(&_10, &_9);
	zephir_array_fast_append(return_value, &_10);
	ZEPHIR_INIT_VAR(&_11);
	ZVAL_LONG(&_11, 'z');
	zephir_get_arrval(&_12, &_11);
	zephir_array_fast_append(return_value, &_12);
	ZEPHIR_INIT_VAR(&_13);
	zephir_create_array(&_13, 2, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_13, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_13, &_0);
	zephir_array_fast_append(return_value, &_13);
	ZEPHIR_INIT_NVAR(&_0);
	array_init(&_0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_14);
	ZVAL_LONG(&_14, i);
	zephir_get_arrval(&_15, &_14);
	zephir_array_fast_append(return_value, &_15);
	ZEPHIR_CPY_WRT(&_16, &s);
	zephir_get_arrval(&_17, &_16);
	zephir_array_fast_append(return_value, &_17);
	ZEPHIR_INIT_VAR(&_18);
	ZVAL_LONG(&_18, c);
	zephir_get_arrval(&_19, &_18);
	zephir_array_fast_append(return_value, &_19);
	ZEPHIR_CPY_WRT(&_20, &arr);
	zephir_get_arrval(&_21, &_20);
	zephir_array_fast_append(return_value, &_21);
	ZEPHIR_CPY_WRT(&_22, dyn);
	zephir_get_arrval(&_23, &_22);
	zephir_array_fast_append(return_value, &_23);
	RETURN_MM();
}

/**
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToObject)
{
	zval _2;
	char c;
	zval s;
	zend_long i;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *dyn, dyn_sub, _0, _1, _3, _4, _5, _6, _7, _8, _9;

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_8);
	ZVAL_UNDEF(&_9);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "abc");
	c = 'z';
	zephir_create_array(return_value, 9, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_convert_to_object(&_0);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_NULL(&_1);
	zephir_convert_to_object(&_1);
	zephir_array_fast_append(return_value, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_LONG(&_3, 1);
	zephir_array_fast_append(&_2, &_3);
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_LONG(&_3, 2);
	zephir_array_fast_append(&_2, &_3);
	zephir_convert_to_object(&_2);
	zephir_array_fast_append(return_value, &_2);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "x");
	zephir_convert_to_object(&_4);
	zephir_array_fast_append(return_value, &_4);
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_LONG(&_5, 'z');
	zephir_convert_to_object(&_5);
	zephir_array_fast_append(return_value, &_5);
	ZEPHIR_INIT_VAR(&_6);
	ZVAL_LONG(&_6, i);
	zephir_convert_to_object(&_6);
	zephir_array_fast_append(return_value, &_6);
	ZEPHIR_CPY_WRT(&_7, &s);
	zephir_convert_to_object(&_7);
	zephir_array_fast_append(return_value, &_7);
	ZEPHIR_INIT_VAR(&_8);
	ZVAL_LONG(&_8, c);
	zephir_convert_to_object(&_8);
	zephir_array_fast_append(return_value, &_8);
	ZEPHIR_CPY_WRT(&_9, dyn);
	zephir_convert_to_object(&_9);
	zephir_array_fast_append(return_value, &_9);
	RETURN_MM();
}

/**
 * `var` accepts anything, so `(var)` is a no-op that has to preserve the
 * value and its type rather than fail with "Cannot cast: X to variable".
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841ToVar)
{
	zval arr, _1;
	char c;
	zval s;
	zend_long i;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *dyn, dyn_sub, __$true, __$null, _0;

	ZVAL_UNDEF(&dyn_sub);
	ZVAL_BOOL(&__$true, 1);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(dyn)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &dyn);
	i = 5;
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "abc");
	c = 'z';
	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&arr, &_0);
	zephir_create_array(return_value, 12, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 5);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 5.5);
	zephir_array_fast_append(return_value, &_0);
	zephir_array_fast_append(return_value, &__$true);
	zephir_array_fast_append(return_value, &__$null);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "abc");
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 'z');
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_create_array(&_1, 2, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&_1, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&_1, &_0);
	zephir_array_fast_append(return_value, &_1);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, i);
	zephir_array_fast_append(return_value, &_0);
	zephir_array_fast_append(return_value, &s);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, c);
	zephir_array_fast_append(return_value, &_0);
	zephir_array_fast_append(return_value, &arr);
	zephir_array_fast_append(return_value, dyn);
	RETURN_MM();
}

/**
 * `(array)`/`(object)` lower to kernel conversions that run in place, so
 * casting a variable used to overwrite that variable as a side effect.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841CastKeepsOperandIntact)
{
	zval _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, asArray, asObject, _0, _2;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&asArray);
	ZVAL_UNDEF(&asObject);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	ZEPHIR_CPY_WRT(&_0, value);
	zephir_get_arrval(&_1, &_0);
	ZEPHIR_CPY_WRT(&asArray, &_1);
	ZEPHIR_CPY_WRT(&_2, value);
	zephir_convert_to_object(&_2);
	ZEPHIR_CPY_WRT(&asObject, &_2);
	zephir_create_array(return_value, 3, 0);
	zephir_array_fast_append(return_value, value);
	zephir_array_fast_append(return_value, &asArray);
	zephir_array_fast_append(return_value, &asObject);
	RETURN_MM();
}

/**
 * The reported symptoms: a cast (or a parenthesized expression) used as a
 * variable declaration's default value reached the assignment as the AST
 * node type, failing with "Unknown type: cast" / "Unknown type: list".
 *
 * @see https://github.com/zephir-lang/zephir/issues/1841
 */
PHP_METHOD(Stub_Cast, issue1841DeclarationInitializers)
{
	unsigned long w;
	zend_ulong u;
	zval _1, _3;
	double y;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, s, _0, _2, _4;
	zend_long a, b, x, z, n;

	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	x = ((a - b));
	y = (double) (((a - b)));
	z = ((a * b));
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, ((a + b)));
	zephir_cast_to_string(&_1, &_0);
	ZEPHIR_CPY_WRT(&s, &_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, a);
	zephir_cast_to_string(&_3, &_2);
	n = zephir_get_intval(&_3);
	u = (zend_ulong) ((a + b));
	w = (unsigned long) ((a + b));
	zephir_create_array(return_value, 7, 0);
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_LONG(&_4, x);
	zephir_array_fast_append(return_value, &_4);
	ZEPHIR_INIT_NVAR(&_4);
	ZVAL_DOUBLE(&_4, y);
	zephir_array_fast_append(return_value, &_4);
	ZEPHIR_INIT_NVAR(&_4);
	ZVAL_LONG(&_4, z);
	zephir_array_fast_append(return_value, &_4);
	zephir_array_fast_append(return_value, &s);
	ZEPHIR_INIT_NVAR(&_4);
	ZVAL_LONG(&_4, n);
	zephir_array_fast_append(return_value, &_4);
	ZEPHIR_INIT_NVAR(&_4);
	ZVAL_LONG(&_4, u);
	zephir_array_fast_append(return_value, &_4);
	ZEPHIR_INIT_NVAR(&_4);
	ZVAL_LONG(&_4, w);
	zephir_array_fast_append(return_value, &_4);
	RETURN_MM();
}

PHP_METHOD(Stub_Cast, issue1841Helper)
{

	RETURN_LONG(3);
}

