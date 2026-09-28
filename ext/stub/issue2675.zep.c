
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
#include "kernel/object.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2675
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 *
 * PHP's `/` returns an `int` when both operands are integers and the division
 * is exact, and a `float` otherwise. Every shape DivOperator can emit is here,
 * plus the `/=` forms, and the test compares each against plain PHP.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2675)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2675, stub, issue2675, stub_issue2675_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2675, divLongLong)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	zephir_div_long_long(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divLongVar)
{
	zval *a_param = NULL, *b, b_sub;
	zend_long a;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b);
	zephir_div_long_zval(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divVarLong)
{
	zend_long b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	zephir_div_zval_long(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divVarVar)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	div_function(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divLongDouble)
{
	double b;
	zval *a_param = NULL, *b_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_ZVAL(b_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	b = zephir_get_doubleval(b_param);
	RETURN_DOUBLE(zephir_safe_div_long_double(a, b));
}

PHP_METHOD(Stub_Issue2675, divDoubleLong)
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

PHP_METHOD(Stub_Issue2675, divDoubleDouble)
{
	zval *a_param = NULL, *b_param = NULL;
	double a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a_param)
		Z_PARAM_ZVAL(b_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	a = zephir_get_doubleval(a_param);
	b = zephir_get_doubleval(b_param);
	RETURN_DOUBLE(zephir_safe_div_double_double(a, b));
}

PHP_METHOD(Stub_Issue2675, divVarDouble)
{
	double b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	b = zephir_get_doubleval(b_param);
	zephir_div_zval_double(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divDoubleVar)
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
	zephir_div_double_zval(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divVarLiteralDouble)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	zephir_div_zval_double(return_value, a, 2.0);
	return;
}

PHP_METHOD(Stub_Issue2675, divLiteralExact)
{

	zephir_div_long_long(return_value, 4, 2);
	return;
}

PHP_METHOD(Stub_Issue2675, divLiteralInexact)
{

	zephir_div_long_long(return_value, 7, 2);
	return;
}

PHP_METHOD(Stub_Issue2675, divLongBool)
{
	zend_bool b;
	zval *a_param = NULL, *b_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	zephir_div_long_long(return_value, a, (zend_long) b);
	return;
}

PHP_METHOD(Stub_Issue2675, divBoolLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	zephir_div_long_long(return_value, (zend_long) a, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divVarBool)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool b;
	zval *a, a_sub, *b_param = NULL, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, b);
	div_function(return_value, a, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2675, divBoolVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b, b_sub, _0;
	zend_bool a;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, a);
	div_function(return_value, &_0, b);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2675, divDoubleBool)
{
	zend_bool b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a_param)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	a = zephir_get_doubleval(a_param);
	RETURN_DOUBLE(zephir_safe_div_double_long(a, (zend_long) b));
}

PHP_METHOD(Stub_Issue2675, divBoolDouble)
{
	double b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_ZVAL(b_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	b = zephir_get_doubleval(b_param);
	RETURN_DOUBLE(zephir_safe_div_long_double((zend_long) a, b));
}

PHP_METHOD(Stub_Issue2675, divBoolBool)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	zephir_div_long_long(return_value, (zend_long) a, (zend_long) b);
	return;
}

PHP_METHOD(Stub_Issue2675, divLongByTrue)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	zephir_div_long_long(return_value, a, (zend_long) 1);
	return;
}

PHP_METHOD(Stub_Issue2675, divTrueByLong)
{
	zval *b_param = NULL;
	zend_long b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b_param);
	zephir_div_long_long(return_value, (zend_long) 1, b);
	return;
}

PHP_METHOD(Stub_Issue2675, divInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, x;
	zend_long a, b;

	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	ZEPHIR_INIT_VAR(&x);
	zephir_div_long_long(&x, a, b);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2675, divReturnDouble)
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

PHP_METHOD(Stub_Issue2675, divTypedDouble)
{
	double d;
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	d =  (zephir_safe_div_long_long(a, b));
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Issue2675, divTypedLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, _0;
	zend_long a, b, k;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_div_long_long(&_0, a, b);
	k = zephir_get_numberval(&_0);
	RETURN_MM_LONG(k);
}

PHP_METHOD(Stub_Issue2675, divChained)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, _0, _1;
	zend_long a, b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_div_long_long(&_0, a, b);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 2);
	mul_function(return_value, &_0, &_1);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2675, divAssignVarLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *a, a_sub, *b_param = NULL, x, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b_param);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	zephir_div_zval_long(&_0, &x, b);
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2675, divAssignVarVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, *b, b_sub, x, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	div_function(&_0, &x, b);
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2675, divAssignInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, x, _0;
	zend_long b;

	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&x);
	ZVAL_LONG(&x, 42);
	ZEPHIR_INIT_VAR(&_0);
	zephir_div_zval_long(&_0, &x, b);
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2675, divAssignProperty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *obj, obj_sub, *b_param = NULL, _0, _1, _2;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(obj)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &obj, &b_param);
	ZEPHIR_SEPARATE_PARAM(obj);
	zephir_read_property_cached(&_0, obj, _zephir_prop_0, 0, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	zephir_div_zval_long(&_1, &_0, b);
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_1);
	zephir_memory_observe(&_2);
	zephir_read_property_cached(&_2, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_Issue2675, divAssignPropertyVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, *b, b_sub, _0, _1, _2;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(obj)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &obj, &b);
	ZEPHIR_SEPARATE_PARAM(obj);
	zephir_read_property_cached(&_0, obj, _zephir_prop_0, 0, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	div_function(&_1, &_0, b);
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_1);
	zephir_memory_observe(&_2);
	zephir_read_property_cached(&_2, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_Issue2675, divAssignPropertyLiteral)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("value", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(obj)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &obj);
	ZEPHIR_SEPARATE_PARAM(obj);
	zephir_read_property_cached(&_0, obj, _zephir_prop_0, 0, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_INIT_VAR(&_1);
	zephir_div_zval_long(&_1, &_0, 2);
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_1);
	zephir_memory_observe(&_2);
	zephir_read_property_cached(&_2, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_2);
}

