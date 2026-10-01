
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
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 *
 * PHP's `%` converts both operands to int and yields an int, and throws
 * TypeError for an operand it cannot treat as a number. Every shape
 * ModOperator can emit is here, plus the `%=` forms, and the test compares
 * each against plain PHP. The typed-local `/=` forms share the `%=` rewrite.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2676)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2676, stub, issue2676, stub_issue2676_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2676, modLongLong)
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

PHP_METHOD(Stub_Issue2676, modLongVar)
{
	zval *a_param = NULL, *b, b_sub;
	zend_long a;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b);
	zephir_mod_long_zval(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2676, modVarLong)
{
	zend_long b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	zephir_mod_zval_long(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2676, modVarVar)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	mod_function(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2676, modLongDouble)
{
	double b;
	zval *a_param = NULL, *b_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_long_double(a, b));
}

PHP_METHOD(Stub_Issue2676, modDoubleLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_double_long(a, b));
}

PHP_METHOD(Stub_Issue2676, modDoubleDouble)
{
	zval *a_param = NULL, *b_param = NULL;
	double a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_double_double(a, b));
}

PHP_METHOD(Stub_Issue2676, modVarDouble)
{
	double b;
	zval *a, a_sub, *b_param = NULL;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b_param);
	zephir_mod_zval_double(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2676, modDoubleVar)
{
	zval *a_param = NULL, *b, b_sub;
	double a;

	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b);
	zephir_mod_double_zval(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Issue2676, modVarLiteralDouble)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	zephir_mod_zval_double(return_value, a, 2.5);
	return;
}

PHP_METHOD(Stub_Issue2676, modVarLiteralLong)
{
	zval *a, a_sub;

	ZVAL_UNDEF(&a_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a);
	zephir_mod_zval_long(return_value, a, 4);
	return;
}

PHP_METHOD(Stub_Issue2676, modLiteral)
{

	RETURN_LONG(zephir_safe_mod_long_long(7, 3));
}

PHP_METHOD(Stub_Issue2676, modLongBool)
{
	zend_bool b;
	zval *a_param = NULL, *b_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_long_long(a, (zend_long) b));
}

PHP_METHOD(Stub_Issue2676, modBoolLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_long_long((zend_long) a, b));
}

PHP_METHOD(Stub_Issue2676, modVarBool)
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
	mod_function(return_value, a, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676, modBoolVar)
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
	mod_function(return_value, &_0, b);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676, modDoubleBool)
{
	zend_bool b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_double_long(a, (zend_long) b));
}

PHP_METHOD(Stub_Issue2676, modBoolDouble)
{
	double b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_long_double((zend_long) a, b));
}

PHP_METHOD(Stub_Issue2676, modBoolBool)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(zephir_safe_mod_long_long((zend_long) a, (zend_long) b));
}

PHP_METHOD(Stub_Issue2676, modLongByTrue)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_LONG(zephir_safe_mod_long_long(a, (zend_long) 1));
}

PHP_METHOD(Stub_Issue2676, modTrueByLong)
{
	zval *b_param = NULL;
	zend_long b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b_param);
	RETURN_LONG(zephir_safe_mod_long_long((zend_long) 1, b));
}

PHP_METHOD(Stub_Issue2676, modVarByTrue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, 1);
	mod_function(return_value, a, &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676, modInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *a, a_sub, *b_param = NULL, x;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b_param);
	ZEPHIR_INIT_VAR(&x);
	zephir_mod_zval_long(&x, a, b);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676, modTypedLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b, k;
	zval *a, a_sub, *b_param = NULL, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_mod_zval_long(&_0, a, b);
	k = zephir_get_intval(&_0);
	RETURN_MM_LONG(k);
}

PHP_METHOD(Stub_Issue2676, modAssignVarLong)
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
	zephir_mod_zval_long(&_0, &x, b);
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676, modAssignVarVar)
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
	mod_function(&_0, &x, b);
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676, modAssignInferredLocal)
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
	zephir_mod_zval_long(&_0, &x, b);
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676, modAssignTypedLong)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_long a, b, x = 0;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	x = a;
	x = zephir_safe_mod_long_long(x, b);
	RETURN_LONG(x);
}

PHP_METHOD(Stub_Issue2676, modAssignTypedLongVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b, b_sub, _0;
	zend_long a, x = 0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b);
	x = a;
	ZEPHIR_INIT_VAR(&_0);
	zephir_mod_long_zval(&_0, x, b);
	x = zephir_get_intval(&_0);
	RETURN_MM_LONG(x);
}

PHP_METHOD(Stub_Issue2676, modAssignTypedDouble)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	double a, x = 0;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	x =  a;
	x = (double) (zephir_safe_mod_double_long(x, b));
	RETURN_DOUBLE(x);
}

PHP_METHOD(Stub_Issue2676, modAssignProperty)
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
	zephir_mod_zval_long(&_1, &_0, b);
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_1);
	zephir_memory_observe(&_2);
	zephir_read_property_cached(&_2, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_Issue2676, modAssignPropertyVar)
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
	mod_function(&_1, &_0, b);
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_1);
	zephir_memory_observe(&_2);
	zephir_read_property_cached(&_2, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_Issue2676, modAssignPropertyLiteral)
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
	zephir_mod_zval_long(&_1, &_0, 4);
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_1);
	zephir_memory_observe(&_2);
	zephir_read_property_cached(&_2, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_Issue2676, divAssignTypedLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, _0;
	zend_long a, b, x = 0;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	x = a;
	ZEPHIR_INIT_VAR(&_0);
	zephir_div_long_long(&_0, x, b);
	x = zephir_get_intval(&_0);
	RETURN_MM_LONG(x);
}

PHP_METHOD(Stub_Issue2676, divAssignTypedDouble)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	double a, x = 0;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	x =  a;
	x =  (zephir_safe_div_double_long(x, b));
	RETURN_DOUBLE(x);
}

