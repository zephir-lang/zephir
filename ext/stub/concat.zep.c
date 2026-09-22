
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
#include "kernel/concat.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_Concat)
{
	ZEPHIR_REGISTER_CLASS(Stub, Concat, stub, concat, stub_concat_method_entry, 0);

	zend_declare_property_null(stub_concat_ce, SL("testProperty"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_string(stub_concat_ce, SL("appended"), "", ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	zend_declare_property_string(stub_concat_ce, SL("store"), "", ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Concat, getTestProperty)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("testProperty"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Concat, testConcatBySelfProperty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval title_zv, _0;
	zend_string *title = NULL;

	ZVAL_UNDEF(&title_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(title)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&title_zv);
	ZVAL_STR_COPY(&title_zv, title);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("testProperty"), PH_NOISY_CC);
	zephir_concat_self(&_0, &title_zv);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("testProperty"), &_0);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Concat, testConcat1)
{
	zval url, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&url);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&url);
	ZVAL_STRING(&url, "test");
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SV(&_0, "append", &url);
	ZEPHIR_CPY_WRT(&url, &_0);
	RETURN_CCTOR(&url);
}

PHP_METHOD(Stub_Concat, testConcat2)
{
	zval _1;
	zval url, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&url);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&url);
	ZVAL_STRING(&url, "test");
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SVS(&_0, "append", &url, "other");
	ZEPHIR_CPY_WRT(&url, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CONCAT_SS(&_1, "append", "other");
	ZEPHIR_CPY_WRT(&url, &_1);
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_CONCAT_SSV(&_0, "append", "other", &url);
	ZEPHIR_CPY_WRT(&url, &_0);
	RETURN_CCTOR(&url);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1573
 */
PHP_METHOD(Stub_Concat, testConcat3)
{
	zval a, b;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "1");
	ZEPHIR_INIT_VAR(&b);
	ZVAL_STRING(&b, "2");
	zephir_concat_self(&b, &a);
	RETURN_CCTOR(&b);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1893
 */
PHP_METHOD(Stub_Concat, testConcat4)
{
	zval query, _1, _3;
	double min = 0, max = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value, value_sub, _0, _2;

	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&query);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &value);
	ZEPHIR_INIT_VAR(&query);
	ZVAL_STRING(&query, "");
	min =  ((zephir_safe_div_zval_long(value, 100) * (double) (25)));
	max =  ((zephir_safe_div_zval_long(value, 100) * (double) (50)));
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, max);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_CONCAT_SV(&_1, "SELECT * FROM TEST WHERE value <= ", &_0);
	zephir_concat_self(&query, &_1);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_DOUBLE(&_2, min);
	ZEPHIR_INIT_VAR(&_3);
	ZEPHIR_CONCAT_SV(&_3, " AND value >= ", &_2);
	zephir_concat_self(&query, &_3);
	RETURN_CTOR(&query);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/1893
 */
PHP_METHOD(Stub_Concat, testConcat5)
{
	zval retval, left;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL, _0;
	double number;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&left);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(number_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &number_param);
	number = zephir_get_doubleval(number_param);
	ZEPHIR_INIT_VAR(&left);
	ZVAL_STRING(&left, "Concatenated string with number ");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, number);
	ZEPHIR_INIT_VAR(&retval);
	ZEPHIR_CONCAT_VV(&retval, &left, &_0);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2660
 */
PHP_METHOD(Stub_Concat, testConcatStringWithIntVar)
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

/**
 * `long` is a `zend_long` on every platform since #2666, so this carries
 * the full 64-bit range exactly like the `int` variant above. It used to
 * be a C `long`, 32-bit under Windows LLP64, and this method held a
 * 32-bit-safe local to keep CI green.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2660
 * @link https://github.com/zephir-lang/zephir/issues/2666
 */
PHP_METHOD(Stub_Concat, testConcatStringWithLongVar)
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

/**
 * @link https://github.com/zephir-lang/zephir/issues/2660
 */
PHP_METHOD(Stub_Concat, testConcatStringWithUintVar)
{
	zend_ulong number;
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	number = 5;
	zephir_concat_self_long(&retval, number);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2660
 */
PHP_METHOD(Stub_Concat, testConcatStringWithUlongVar)
{
	zend_ulong number;
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	number = 5;
	zephir_concat_self_long(&retval, number);
	RETURN_CTOR(&retval);
}

/**
 * Fence for the sibling helper: a char operand routes to
 * zephir_concat_self_char(), which is defined.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2660
 */
PHP_METHOD(Stub_Concat, testConcatStringWithCharVar)
{
	char c;
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	c = 'A';
	zephir_concat_self_char(&retval, c);
	RETURN_CTOR(&retval);
}

/**
 * A `var` left-hand side takes a different code path than a `string` one.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2660
 */
PHP_METHOD(Stub_Concat, testConcatVarWithIntVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL, retval;
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
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2660
 */
PHP_METHOD(Stub_Concat, testConcatVarWithCharVar)
{
	char c;
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	c = 'A';
	zephir_concat_self_char(&retval, c);
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2660
 */
PHP_METHOD(Stub_Concat, testConcatVarWithIntLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_long(&retval, 5);
	RETURN_CCTOR(&retval);
}

/**
 * A double must come out exactly as PHP renders it. PHP converts a float
 * with `zend_double_to_str()`, which reads `EG(precision)` at run time, so
 * the operand is boxed and converted by the engine rather than formatted
 * by the compiler.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatStringWithDoubleVar)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL;
	double number;

	ZVAL_UNDEF(&retval);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(number_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &number_param);
	number = zephir_get_doubleval(number_param);
	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_double(&retval, number);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatStringWithDoubleLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_double(&retval, 1.5);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatStringWithBoolVar)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL;
	zend_bool flag;

	ZVAL_UNDEF(&retval);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_bool(&retval, flag);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatStringWithTrueLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_bool(&retval, 1);
	RETURN_CTOR(&retval);
}

/**
 * `false` appends nothing, which is not the same as doing nothing.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatStringWithFalseLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_bool(&retval, 0);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithDoubleVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *number_param = NULL, retval;
	double number;

	ZVAL_UNDEF(&retval);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(number_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &number_param);
	number = zephir_get_doubleval(number_param);
	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_double(&retval, number);
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithDoubleLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_double(&retval, 1.5);
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithBoolVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, retval;
	zend_bool flag;

	ZVAL_UNDEF(&retval);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_bool(&retval, flag);
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithTrueLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_bool(&retval, 1);
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithFalseLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	zephir_concat_self_bool(&retval, 0);
	RETURN_CCTOR(&retval);
}

/**
 * The left operand is an integer, so `.= false` appends nothing yet still
 * converts it: PHP leaves the *string* "5" here.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatIntVarWithFalseLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_LONG(&retval, 5);
	zephir_concat_self_bool(&retval, 0);
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithNullLiteral)
{
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_LONG(&retval, 5);
	zephir_concat_self_str(&retval, SL(""));
	RETURN_CCTOR(&retval);
}

/**
 * PHP raises "Array to string conversion" and appends "Array". A local
 * left-hand side used to reject the operand outright while an object
 * property accepted it, so the two disagreed.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatStringWithArrayLiteral)
{
	zval _1;
	zval _0;
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	zephir_concat_self(&retval, &_0);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatStringWithArrayVar)
{
	zval _0;
	zval a;
	zval retval;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	zephir_concat_self(&retval, &a);
	RETURN_CTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithArrayLiteral)
{
	zval _0;
	zval retval, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	zephir_concat_self(&retval, &_0);
	RETURN_CCTOR(&retval);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatVarWithArrayVar)
{
	zval a;
	zval retval, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&retval);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&a);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&retval);
	ZVAL_STRING(&retval, "n=");
	ZEPHIR_INIT_VAR(&a);
	zephir_create_array(&a, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&a, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 2);
	zephir_array_fast_append(&a, &_0);
	zephir_concat_self(&retval, &a);
	RETURN_CCTOR(&retval);
}

PHP_METHOD(Stub_Concat, getStore)
{

	RETURN_MEMBER_TYPED(getThis(), "store", IS_STRING);
}

PHP_METHOD(Stub_Concat, resetStore)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("store", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * `.=` on a property is a read-modify-write. Every operand type here used
 * to overwrite the property instead of appending to it.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, appendPropertyLiterals)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("store", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_str(&_0, SL("s"));
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_long(&_0, 5);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_double(&_0, 1.5);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_bool(&_0, 1);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_bool(&_0, 0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_str(&_0, SL(""));
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, appendPropertyVariables)
{
	char c;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *text = NULL;
	zend_bool flag;
	double fraction;
	zval *number_param = NULL, *fraction_param = NULL, *flag_param = NULL, text_zv, _0;
	zend_long number;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text_zv);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("store", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_LONG(number)
		Z_PARAM_ZVAL(fraction_param)
		Z_PARAM_BOOL(flag)
		Z_PARAM_STR(text)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	number_param = ZEND_CALL_ARG(execute_data, 1);
	fraction_param = ZEND_CALL_ARG(execute_data, 2);
	flag_param = ZEND_CALL_ARG(execute_data, 3);
	fraction = zephir_get_doubleval(fraction_param);
	zephir_memory_observe(&text_zv);
	ZVAL_STR_COPY(&text_zv, text);
	c = 'A';
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_long(&_0, number);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_double(&_0, fraction);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_bool(&_0, flag);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self_char(&_0, c);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self(&_0, &text_zv);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * PHP raises "Array to string conversion" and appends "Array".
 *
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, appendPropertyArray)
{
	zval _1, _2;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("store", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_OBS_NVAR(&_2);
	zephir_read_property_cached(&_2, this_ptr, _zephir_prop_0, 32, PH_NOISY_CC);
	zephir_concat_self(&_2, &_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 32, &_2);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Concat, getAppended)
{
	zval _0;

	ZVAL_UNDEF(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTORW(&_0);
}

PHP_METHOD(Stub_Concat, resetAppended)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_EMPTY_STRING(&_0);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, appendStaticLiterals)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_str(&_0, SL("s"));
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_long(&_0, 5);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_double(&_0, 1.5);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_bool(&_0, 1);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_bool(&_0, 0);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, appendStaticVariables)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool flag;
	double fraction;
	zval *number_param = NULL, *fraction_param = NULL, *flag_param = NULL, _0;
	zend_long number;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(number)
		Z_PARAM_ZVAL(fraction_param)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &number_param, &fraction_param, &flag_param);
	fraction = zephir_get_doubleval(fraction_param);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_long(&_0, number);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_double(&_0, fraction);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_OBS_NVAR(&_0);
	zephir_read_static_property_ce(&_0, stub_concat_ce, SL("appended"), PH_NOISY_CC);
	zephir_concat_self_bool(&_0, flag);
	zephir_update_static_property_ce(stub_concat_ce, ZEND_STRL("appended"), &_0);
	ZEPHIR_MM_RESTORE();
}

/**
 * The binary `.` operator rejected a bool operand, and a char operand
 * appended nothing usable, although `.=` accepted both.
 *
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatExprWithBoolVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *flag_param = NULL, _0;
	zend_bool flag;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_BOOL(flag)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &flag_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, flag);
	ZEPHIR_CONCAT_SV(return_value, "n=", &_0);
	RETURN_MM();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatExprWithTrueLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	ZEPHIR_CONCAT_SV(return_value, "n=", &_0);
	RETURN_MM();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatExprWithFalseLiteral)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 0);
	ZEPHIR_CONCAT_SV(return_value, "n=", &_0);
	RETURN_MM();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatExprWithCharVar)
{
	zval _0;
	char c;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	c = 'A';
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRINGL(&_0, (char *) &c, 1);
	ZEPHIR_CONCAT_SV(return_value, "n=", &_0);
	RETURN_MM();
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/2664
 */
PHP_METHOD(Stub_Concat, testConcatExprWithCharLiteral)
{

	ZEPHIR_CONCAT_SS(return_value, "n=", "A");
	return;
}

