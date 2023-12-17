
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_Types_MixedType)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Types, MixedType, stub, types_mixedtype, stub_types_mixedtype_method_entry, 0);

	return SUCCESS;
}

/**
 * Mixed only as return type methods
 */
PHP_METHOD(Stub_Types_MixedType, returnMixedObject)
{

	object_init(return_value);
	return;
}

PHP_METHOD(Stub_Types_MixedType, returnMixedArray)
{

	array_init(return_value);
	return;
}

PHP_METHOD(Stub_Types_MixedType, returnMixedString)
{

	RETURN_STRING("mixed string");
}

PHP_METHOD(Stub_Types_MixedType, returnMixedInt)
{

	RETURN_LONG(1);
}

PHP_METHOD(Stub_Types_MixedType, returnMixedFloat)
{

	RETURN_DOUBLE(3.14);
}

PHP_METHOD(Stub_Types_MixedType, returnMixedBool)
{

	RETURN_BOOL(1);
}

PHP_METHOD(Stub_Types_MixedType, returnMixedNull)
{

	RETURN_NULL();
}

PHP_METHOD(Stub_Types_MixedType, returnMixed74)
{
	zval *diff_param = NULL;
	zend_bool diff;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(diff)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &diff_param);
	if (!diff_param) {
		diff = 0;
	} else {
		}
	if (diff) {
		RETURN_STRING("string");
	}
	object_init(return_value);
	return;
}

PHP_METHOD(Stub_Types_MixedType, returnMultiButAlwaysMixed)
{

	RETURN_STRING("ZEND_BEGIN_ARG_INFO_EX");
}

/**
 * Mixed only as parameter in method
 */
PHP_METHOD(Stub_Types_MixedType, paramMixed)
{
	zval val_sub;
	zval *val;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val);
	RETVAL_ZVAL(val, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_MixedType, paramMixedTwo)
{
	zval val1_sub, val2_sub;
	zval *val1, *val2;

	ZVAL_UNDEF(&val1_sub);
	ZVAL_UNDEF(&val2_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(val1)
		Z_PARAM_ZVAL(val2)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &val1, &val2);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, val1);
	zephir_array_fast_append(return_value, val2);
	return;
}

PHP_METHOD(Stub_Types_MixedType, paramMixedWithMulti)
{
	zval *mixedVal;
	zval stringVal;
	zval *intVal_param = NULL, *stringVal_param = NULL, mixedVal_sub, _0;
	zend_long intVal;

	ZVAL_UNDEF(&mixedVal_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&stringVal);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(intVal)
		Z_PARAM_STR(stringVal)
		Z_PARAM_ZVAL(mixedVal)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 3, 0, &intVal_param, &stringVal_param, &mixedVal);
	zephir_get_strval(&stringVal, stringVal_param);
	zephir_create_array(return_value, 3, 0);
	ZVAL_NULL(&_0);
	ZVAL_LONG(&_0, intVal);
	zephir_array_fast_append(return_value, &_0);
	zephir_array_fast_append(return_value, &stringVal);
	zephir_array_fast_append(return_value, mixedVal);
	return;
}

/**
 * Mixed as as parameter and return type in method
 */
PHP_METHOD(Stub_Types_MixedType, paramAndReturnMixed)
{
	zval val_sub;
	zval *val;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val);
	RETVAL_ZVAL(val, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_MixedType, castToStringMixedAndReturnMixed)
{
	zval _0;
	zval val_sub;
	zval *val;

	ZVAL_UNDEF(&val_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &val);
	zephir_cast_to_string(&_0, val);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Types_MixedType, castToStringInternallyMixedAndReturnMixed)
{
	zval _0;
	zval val_sub;
	zval *val = NULL;

	ZVAL_UNDEF(&val_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &val);
	zephir_cast_to_string(&_0, val);
	ZEPHIR_CPY_WRT(val, &_0);
	RETVAL_ZVAL(val, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_MixedType, castToIntMixedAndReturnMixed)
{
	zval val_sub;
	zval *val;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val);
	RETURN_LONG(zephir_get_intval(val));
}

PHP_METHOD(Stub_Types_MixedType, castToIntInternallyMixedAndReturnMixed)
{
	zend_long _0;
	zval val_sub;
	zval *val = NULL;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &val);
	_0 = zephir_get_intval(val);
	ZEPHIR_INIT_NVAR(val);
	ZVAL_LONG(val, _0);
	RETVAL_ZVAL(val, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_MixedType, castToBoolMixedAndReturnMixed)
{
	zval val_sub;
	zval *val;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val);
	RETURN_BOOL(zephir_get_boolval(val));
}

PHP_METHOD(Stub_Types_MixedType, castToBoolInternallyMixedAndReturnMixed)
{
	double _0;
	zval val_sub;
	zval *val = NULL;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &val);
	_0 = zephir_get_boolval(val);
	ZEPHIR_INIT_NVAR(val);
	ZVAL_BOOL(val, _0);
	RETVAL_ZVAL(val, 1, 0);
	return;
}

PHP_METHOD(Stub_Types_MixedType, castToFloatMixedAndReturnMixed)
{
	zval val_sub;
	zval *val;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val);
	RETURN_DOUBLE(zephir_get_doubleval(val));
}

PHP_METHOD(Stub_Types_MixedType, castToFloatInternallyMixedAndReturnMixed)
{
	double _0;
	zval val_sub;
	zval *val = NULL;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &val);
	_0 = zephir_get_doubleval(val);
	ZEPHIR_INIT_NVAR(val);
	ZVAL_DOUBLE(val, _0);
	RETVAL_ZVAL(val, 1, 0);
	return;
}

/**
 * Only used during generation
 */
PHP_METHOD(Stub_Types_MixedType, mixedInCondition)
{
	zval val_sub;
	zval *val;

	ZVAL_UNDEF(&val_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(val)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &val);
	if (UNEXPECTED(zephir_is_true(val))) {
	}
	if (ZEPHIR_IS_LONG_IDENTICAL(val, 1)) {
	}
	if (ZEPHIR_IS_LONG_IDENTICAL(val, 1)) {
	}
	zend_print_zval(val, 0);
}

