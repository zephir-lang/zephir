
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
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/string.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2666
 *
 * Zephir's `long`/`ulong` mapped to a C `long`/`unsigned long`, which is
 * 32-bit under LLP64 (Windows x64) and 64-bit under LP64 (Linux, macOS),
 * while a PHP integer is always a 64-bit `zend_long`. Every method here moves
 * a value that only fits in 64 bits across that boundary, and the test asserts
 * the result against the same expression evaluated in plain PHP.
 *
 * The division and modulo methods cover the second half of the fix: the
 * `zephir_safe_mod_*` family was declared and emitted but mostly undefined,
 * and both families answered a zero divisor with a warning instead of PHP 8's
 * DivisionByZeroError.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2666)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2666, stub, issue2666, stub_issue2666_method_entry, 0);

	return SUCCESS;
}

/**
 * The reproduction from the issue: `Z_PARAM_LONG(number)` stores a
 * `zend_long` through `&number`.
 */
PHP_METHOD(Stub_Issue2666, longParamToString)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL;
	zend_long number;

	ZVAL_UNDEF(&retval);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(number)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &number_param);
	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_long(&retval, number);
	RETURN_CTOR(&retval);
}

PHP_METHOD(Stub_Issue2666, ulongParamToString)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL;
	zend_ulong number;

	ZVAL_UNDEF(&retval);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(number)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &number_param);
	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_long(&retval, number);
	RETURN_CTOR(&retval);
}

PHP_METHOD(Stub_Issue2666, intParamToString)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL;
	zend_long number;

	ZVAL_UNDEF(&retval);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(number)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &number_param);
	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_long(&retval, number);
	RETURN_CTOR(&retval);
}

PHP_METHOD(Stub_Issue2666, longRoundTrip)
{
	zval *number_param = NULL;
	zend_long number;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(number)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &number_param);
	RETURN_LONG(number);
}

PHP_METHOD(Stub_Issue2666, ulongRoundTrip)
{
	zval *number_param = NULL;
	zend_ulong number;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(number)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &number_param);
	RETURN_LONG(number);
}

/**
 * Parameter -> local -> return, so the value passes through a declared
 * local rather than staying in the parameter slot.
 */
PHP_METHOD(Stub_Issue2666, longThroughLocal)
{
	zval *number_param = NULL;
	zend_long number, copy = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(number)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &number_param);
	copy = number;
	RETURN_LONG(copy);
}

/**
 * `zephir_get_intval()`, the funnel every cast shares. Its `_ex` helper
 * returned a C `long`, so this truncated even for Zephir `int`.
 */
PHP_METHOD(Stub_Issue2666, castToInt)
{
	zend_long result = 0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	result = zephir_get_intval(value);
	RETURN_LONG(result);
}

PHP_METHOD(Stub_Issue2666, castToLong)
{
	zend_long result = 0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	result = zephir_get_intval(value);
	RETURN_LONG(result);
}

/**
 * Arithmetic on two 64-bit locals: no kernel helper, but the operands and
 * the result must all be 64-bit wide.
 */
PHP_METHOD(Stub_Issue2666, longAdd)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG((a + b));
}

/** Array key built from a `long`: `zephir_array_update_long()`. */
PHP_METHOD(Stub_Issue2666, longArrayKey)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key_param = NULL, *value, value_sub;
	zend_long key;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(key)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &key_param, &value);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	zephir_array_update_long(&result, key, value, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CTOR(&result);
}

/** `zephir_array_fetch_long()` with a 64-bit key. */
PHP_METHOD(Stub_Issue2666, longArrayFetch)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long key;
	zval *source_param = NULL, *key_param = NULL, _0;
	zval source;

	ZVAL_UNDEF(&source);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(source, source_param)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &source_param, &key_param);
	ZEPHIR_OBS_COPY_OR_DUP(&source, source_param);
	zephir_array_fetch_long(&_0, &source, key, PH_NOISY | PH_READONLY, "stub/issue2666.zep", 103);
	RETURN_CTOR(&_0);
}

/** `zephir_array_isset_long()`. */
PHP_METHOD(Stub_Issue2666, longArrayIsset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long key;
	zval *source_param = NULL, *key_param = NULL;
	zval source;

	ZVAL_UNDEF(&source);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(source, source_param)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &source_param, &key_param);
	ZEPHIR_OBS_COPY_OR_DUP(&source, source_param);
	RETURN_MM_BOOL(zephir_array_isset_value_long(&source, key));
}

/** `zephir_less_long()` / `zephir_greater_long()`: zval against a native. */
PHP_METHOD(Stub_Issue2666, varLessThanLong)
{
	zend_long limit;
	zval *value, value_sub, *limit_param = NULL;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(value)
		Z_PARAM_LONG(limit)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &value, &limit_param);
	RETURN_BOOL(ZEPHIR_LT_LONG(value, limit));
}

PHP_METHOD(Stub_Issue2666, varGreaterThanLong)
{
	zend_long limit;
	zval *value, value_sub, *limit_param = NULL;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(value)
		Z_PARAM_LONG(limit)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &value, &limit_param);
	RETURN_BOOL(ZEPHIR_GT_LONG(value, limit));
}

/** `zephir_substr()` took `long from, long length`. */
PHP_METHOD(Stub_Issue2666, substrWithLongOffset)
{
	zend_long from, length;
	zval source_zv, *from_param = NULL, *length_param = NULL, _0, _1;
	zend_string *source = NULL;

	ZVAL_UNDEF(&source_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(source)
		Z_PARAM_LONG(from)
		Z_PARAM_LONG(length)
	ZEND_PARSE_PARAMETERS_END();
	from_param = ZEND_CALL_ARG(execute_data, 2);
	length_param = ZEND_CALL_ARG(execute_data, 3);
	ZVAL_STR(&source_zv, source);
	ZVAL_LONG(&_0, from);
	ZVAL_LONG(&_1, length);
	zephir_substr(return_value, &source_zv, zephir_get_intval(&_0), zephir_get_intval(&_1), 0);
	return;
}

/** `zephir_fast_explode()` took a `long limit`. */
PHP_METHOD(Stub_Issue2666, explodeWithLongLimit)
{
	zend_long limit;
	zval source_zv, *limit_param = NULL, _0;
	zend_string *source = NULL;

	ZVAL_UNDEF(&source_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(source)
		Z_PARAM_LONG(limit)
	ZEND_PARSE_PARAMETERS_END();
	limit_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&source_zv, source);
	ZVAL_LONG(&_0, limit);
	zephir_fast_explode_str(return_value, SL(","), &source_zv, zephir_get_intval(&_0) );
	return;
}

/**
 * The eight `zephir_safe_div_*` / `zephir_safe_mod_*` shapes the compiler
 * can actually reach. A double *literal* on the right is the only way to
 * reach the `*_double` variants: a `double` variable there is rejected by
 * ModOperator/DivOperator.
 */
PHP_METHOD(Stub_Issue2666, divLongLong)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_DOUBLE(zephir_safe_div_long_long(a, b));
}

PHP_METHOD(Stub_Issue2666, divLongDouble)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE(zephir_safe_div_long_double(a, 2.5));
}

PHP_METHOD(Stub_Issue2666, divDoubleLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a_param)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	a = zephir_get_doubleval(a_param);
	RETURN_DOUBLE(zephir_safe_div_double_long(a, b));
}

PHP_METHOD(Stub_Issue2666, divDoubleDouble)
{
	zval *a_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	a = zephir_get_doubleval(a_param);
	RETURN_DOUBLE(zephir_safe_div_double_double(a, 2.5));
}

PHP_METHOD(Stub_Issue2666, divVarLong)
{
	zend_long b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	RETURN_DOUBLE(zephir_safe_div_zval_long(a, b));
}

PHP_METHOD(Stub_Issue2666, divVarDouble)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_DOUBLE(zephir_safe_div_zval_double(a, 2.5));
}

PHP_METHOD(Stub_Issue2666, divLongVar)
{
	zval *a_param = NULL, *b, b_sub;
	zend_long a;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b);
	RETURN_DOUBLE(zephir_safe_div_long_zval(a, b));
}

PHP_METHOD(Stub_Issue2666, divDoubleVar)
{
	zval *a_param = NULL, *b, b_sub;
	double a;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a_param)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b);
	a = zephir_get_doubleval(a_param);
	RETURN_DOUBLE(zephir_safe_div_double_zval(a, b));
}

PHP_METHOD(Stub_Issue2666, modLongLong)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_long_long(a, b));
}

PHP_METHOD(Stub_Issue2666, modLongDouble)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_LONG(zephir_safe_mod_long_double(a, 2.5));
}

PHP_METHOD(Stub_Issue2666, modDoubleLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a_param)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	a = zephir_get_doubleval(a_param);
	RETURN_LONG(zephir_safe_mod_double_long(a, b));
}

PHP_METHOD(Stub_Issue2666, modDoubleDouble)
{
	zval *a_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	a = zephir_get_doubleval(a_param);
	RETURN_LONG(zephir_safe_mod_double_double(a, 2.5));
}

PHP_METHOD(Stub_Issue2666, modVarLong)
{
	zend_long b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	RETURN_LONG(zephir_safe_mod_zval_long(a, b));
}

PHP_METHOD(Stub_Issue2666, modVarDouble)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	RETURN_LONG(zephir_safe_mod_zval_double(a, 2.5));
}

PHP_METHOD(Stub_Issue2666, modLongVar)
{
	zval *a_param = NULL, *b, b_sub;
	zend_long a;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b);
	RETURN_LONG(zephir_safe_mod_long_zval(a, b));
}

PHP_METHOD(Stub_Issue2666, modDoubleVar)
{
	zval *a_param = NULL, *b, b_sub;
	double a;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a_param)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b);
	a = zephir_get_doubleval(a_param);
	RETURN_LONG(zephir_safe_mod_double_zval(a, b));
}

/** Literal double on the left: the only shape reaching `*_double_double`. */
PHP_METHOD(Stub_Issue2666, divLiteralDoubleByDouble)
{
	zval *b_param = NULL;
	double b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b_param);
	b = zephir_get_doubleval(b_param);
	RETURN_DOUBLE(zephir_safe_div_double_double(2.5, b));
}

PHP_METHOD(Stub_Issue2666, modLiteralDoubleByDouble)
{
	zval *b_param = NULL;
	double b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b_param);
	b = zephir_get_doubleval(b_param);
	RETURN_LONG(zephir_safe_mod_double_double(2.5, b));
}

/**
 * Multi-dimensional keys reach `zephir_array_update_multi()` through a
 * variadic slot read back as `zend_long`. An integer literal is a C `int`,
 * so the upper half of the slot was whatever the ABI left there: on Windows
 * `a[0][1]` produced the key 140733193388033. `uint`/`ulong` were worse,
 * passing `&i` and using the pointer as the key.
 */
PHP_METHOD(Stub_Issue2666, multiLiteralKeys)
{
	zval _0;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&result, &_0, SL("ll"), 2, (zend_long) 0, (zend_long) 1);
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_Issue2666, multiIntKey)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key_param = NULL, _0;
	zend_long key;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&result, &_0, SL("ll"), 2, (zend_long) key, (zend_long) 2);
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_Issue2666, multiUintKey)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key_param = NULL, _0;
	zend_ulong key;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&result, &_0, SL("ll"), 2, (zend_long) key, (zend_long) 2);
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_Issue2666, multiLongKey)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key_param = NULL, _0;
	zend_long key;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&result, &_0, SL("ll"), 2, (zend_long) key, (zend_long) 2);
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_Issue2666, multiUlongKey)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key_param = NULL, _0;
	zend_ulong key;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&result, &_0, SL("ll"), 2, (zend_long) key, (zend_long) 2);
	RETURN_CTOR(&result);
}

/** Enough offsets to spill past the register arguments. */
PHP_METHOD(Stub_Issue2666, multiDeepKeys)
{
	zval _0;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&result, &_0, SL("lllllllll"), 9, (zend_long) 0, (zend_long) 1, (zend_long) 2, (zend_long) 3, (zend_long) 4, (zend_long) 5, (zend_long) 6, (zend_long) 7, (zend_long) 8);
	RETURN_CTOR(&result);
}

/** Mixed string and integer offsets in one call. */
PHP_METHOD(Stub_Issue2666, multiMixedKeys)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *key_param = NULL, _0;
	zend_long key;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(key)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &key_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "v");
	zephir_array_update_multi(&result, &_0, SL("sl"), 3, SL("k"), (zend_long) key);
	RETURN_CTOR(&result);
}

