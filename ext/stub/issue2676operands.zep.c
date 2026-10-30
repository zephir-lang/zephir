
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


/**
 * @issue https://github.com/zephir-lang/zephir/issues/2676
 * @issue https://github.com/zephir-lang/zephir/issues/2677
 *
 * Every arithmetic operator with an operand that is not a native number:
 * a string or array local, or a string, null or array literal, which PHP
 * coerces or rejects with TypeError. The bool shapes pin `+ - *` to the
 * integer 0 or 1, as `/` and `%` already are. The test compares each one
 * against plain PHP.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2676Operands)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2676Operands, stub, issue2676operands, stub_issue2676operands_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2676Operands, addStringLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_add_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addLongString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *b = NULL;
	zval *a_param = NULL, b_zv, _0;
	zend_long a;

	ZVAL_UNDEF(&b_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a_param = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&b_zv);
	ZVAL_STR_COPY(&b_zv, b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_add_function(return_value, &_0, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addStringDouble)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	double b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_add_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addStringBool)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, b);
	zephir_add_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addStringVar)
{
	zval a_zv, *b, b_sub;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	b = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	zephir_add_function(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, addStringString)
{
	zval a_zv, b_zv;
	zend_string *a = NULL, *b = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&a_zv, a);
	ZVAL_STR(&b_zv, b);
	zephir_add_function(return_value, &a_zv, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, addArrayLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *a_param = NULL, *b_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_add_function(return_value, &a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addArrayVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b, b_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b);
	zephir_get_arrval(&a, a_param);
	zephir_add_function(return_value, &a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	zephir_add_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addLiteralAbc)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abc");
	zephir_add_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addLiteralLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0, _1;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, b);
	zephir_add_function(return_value, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addNullVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_add_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addNullLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0, _1;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, b);
	zephir_add_function(return_value, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addLiteralArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _1;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	zephir_add_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, addTrueLong)
{
	zval *b_param = NULL;
	zend_long b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b_param);
	RETURN_LONG(((zend_long) 1 + b));
}

PHP_METHOD(Stub_Issue2676Operands, addLongTrue)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_LONG((a + (zend_long) 1));
}

PHP_METHOD(Stub_Issue2676Operands, addBoolLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(((zend_long) a + b));
}

PHP_METHOD(Stub_Issue2676Operands, addBoolBool)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(((zend_long) a + (zend_long) b));
}

PHP_METHOD(Stub_Issue2676Operands, addDoubleBool)
{
	zend_bool b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_DOUBLE((a + (double) ((zend_long) b)));
}

PHP_METHOD(Stub_Issue2676Operands, addBoolDouble)
{
	double b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_DOUBLE(((double) ((zend_long) a) + b));
}

PHP_METHOD(Stub_Issue2676Operands, addDoubleTrue)
{
	zval *a_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE((a + (double) ((zend_long) 1)));
}

PHP_METHOD(Stub_Issue2676Operands, addLongLiteralDouble)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE(((double) (a) + 1.5));
}

PHP_METHOD(Stub_Issue2676Operands, addInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, x, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	ZEPHIR_INIT_VAR(&x);
	zephir_add_function(&x, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, addAssignVarLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	zephir_add_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, addAssignVarString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *s = NULL;
	zval *a, a_sub, s_zv, x, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	zephir_add_function(&_0, &x, &s_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, addAssignVarNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_add_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, addAssignPropertyLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_STRING(&_1, "10");
	ZEPHIR_INIT_VAR(&_2);
	zephir_add_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, addAssignPropertyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_NULL(&_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_add_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, subStringLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_sub_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subLongString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *b = NULL;
	zval *a_param = NULL, b_zv, _0;
	zend_long a;

	ZVAL_UNDEF(&b_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a_param = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&b_zv);
	ZVAL_STR_COPY(&b_zv, b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	zephir_sub_function(return_value, &_0, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subStringDouble)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	double b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, b);
	zephir_sub_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subStringBool)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, b);
	zephir_sub_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subStringVar)
{
	zval a_zv, *b, b_sub;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	b = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	zephir_sub_function(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, subStringString)
{
	zval a_zv, b_zv;
	zend_string *a = NULL, *b = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&a_zv, a);
	ZVAL_STR(&b_zv, b);
	zephir_sub_function(return_value, &a_zv, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, subArrayLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *a_param = NULL, *b_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	zephir_sub_function(return_value, &a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subArrayVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b, b_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b);
	zephir_get_arrval(&a, a_param);
	zephir_sub_function(return_value, &a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	zephir_sub_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subLiteralAbc)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abc");
	zephir_sub_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subLiteralLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0, _1;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, b);
	zephir_sub_function(return_value, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subNullVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_sub_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subNullLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0, _1;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, b);
	zephir_sub_function(return_value, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subLiteralArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _1;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	zephir_sub_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, subTrueLong)
{
	zval *b_param = NULL;
	zend_long b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b_param);
	RETURN_LONG(((zend_long) 1 - b));
}

PHP_METHOD(Stub_Issue2676Operands, subLongTrue)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_LONG((a - (zend_long) 1));
}

PHP_METHOD(Stub_Issue2676Operands, subBoolLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(((zend_long) a - b));
}

PHP_METHOD(Stub_Issue2676Operands, subBoolBool)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(((zend_long) a - (zend_long) b));
}

PHP_METHOD(Stub_Issue2676Operands, subDoubleBool)
{
	zend_bool b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_DOUBLE((a - (double) ((zend_long) b)));
}

PHP_METHOD(Stub_Issue2676Operands, subBoolDouble)
{
	double b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_DOUBLE(((double) ((zend_long) a) - b));
}

PHP_METHOD(Stub_Issue2676Operands, subDoubleTrue)
{
	zval *a_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE((a - (double) ((zend_long) 1)));
}

PHP_METHOD(Stub_Issue2676Operands, subLongLiteralDouble)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE(((double) (a) - 1.5));
}

PHP_METHOD(Stub_Issue2676Operands, subInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, x, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	ZEPHIR_INIT_VAR(&x);
	zephir_sub_function(&x, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, subAssignVarLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	zephir_sub_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, subAssignVarString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *s = NULL;
	zval *a, a_sub, s_zv, x, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	zephir_sub_function(&_0, &x, &s_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, subAssignVarNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	zephir_sub_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, subAssignPropertyLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_STRING(&_1, "10");
	ZEPHIR_INIT_VAR(&_2);
	zephir_sub_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, subAssignPropertyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_NULL(&_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_sub_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, mulStringLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	mul_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulLongString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *b = NULL;
	zval *a_param = NULL, b_zv, _0;
	zend_long a;

	ZVAL_UNDEF(&b_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a_param = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&b_zv);
	ZVAL_STR_COPY(&b_zv, b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	mul_function(return_value, &_0, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulStringDouble)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	double b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, b);
	mul_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulStringBool)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, b);
	mul_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulStringVar)
{
	zval a_zv, *b, b_sub;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	b = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	mul_function(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, mulStringString)
{
	zval a_zv, b_zv;
	zend_string *a = NULL, *b = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&a_zv, a);
	ZVAL_STR(&b_zv, b);
	mul_function(return_value, &a_zv, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, mulArrayLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *a_param = NULL, *b_param = NULL, _0;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	zephir_get_arrval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	mul_function(return_value, &a, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulArrayVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b, b_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b);
	zephir_get_arrval(&a, a_param);
	mul_function(return_value, &a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	mul_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulLiteralAbc)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abc");
	mul_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulLiteralLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0, _1;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, b);
	mul_function(return_value, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulNullVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	mul_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulNullLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0, _1;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, b);
	mul_function(return_value, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulLiteralArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _1;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	mul_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, mulTrueLong)
{
	zval *b_param = NULL;
	zend_long b;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &b_param);
	RETURN_LONG(((zend_long) 1 * b));
}

PHP_METHOD(Stub_Issue2676Operands, mulLongTrue)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_LONG((a * (zend_long) 1));
}

PHP_METHOD(Stub_Issue2676Operands, mulBoolLong)
{
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(((zend_long) a * b));
}

PHP_METHOD(Stub_Issue2676Operands, mulBoolBool)
{
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG(((zend_long) a * (zend_long) b));
}

PHP_METHOD(Stub_Issue2676Operands, mulDoubleBool)
{
	zend_bool b;
	zval *a_param = NULL, *b_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_DOUBLE(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_DOUBLE((a * (double) ((zend_long) b)));
}

PHP_METHOD(Stub_Issue2676Operands, mulBoolDouble)
{
	double b;
	zval *a_param = NULL, *b_param = NULL;
	zend_bool a;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_BOOL(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_DOUBLE(((double) ((zend_long) a) * b));
}

PHP_METHOD(Stub_Issue2676Operands, mulDoubleTrue)
{
	zval *a_param = NULL;
	double a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE((a * (double) ((zend_long) 1)));
}

PHP_METHOD(Stub_Issue2676Operands, mulLongLiteralDouble)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_DOUBLE(((double) (a) * 1.5));
}

PHP_METHOD(Stub_Issue2676Operands, mulInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, x, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, b);
	ZEPHIR_INIT_VAR(&x);
	mul_function(&x, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, mulAssignVarLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	mul_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, mulAssignVarString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *s = NULL;
	zval *a, a_sub, s_zv, x, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	mul_function(&_0, &x, &s_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, mulAssignVarNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	mul_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, mulAssignPropertyLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_STRING(&_1, "10");
	ZEPHIR_INIT_VAR(&_2);
	mul_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, mulAssignPropertyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_NULL(&_1);
	ZEPHIR_INIT_VAR(&_2);
	mul_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, divStringLong)
{
	zend_long b;
	zval a_zv, *b_param = NULL;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	b_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	zephir_div_zval_long(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, divLongString)
{
	zend_string *b = NULL;
	zval *a_param = NULL, b_zv;
	zend_long a;

	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	a_param = ZEND_CALL_ARG(execute_data, 1);
	ZVAL_STR(&b_zv, b);
	zephir_div_long_zval(return_value, a, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, divStringDouble)
{
	double b;
	zval a_zv, *b_param = NULL;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	b_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	zephir_div_zval_double(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, divStringBool)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, b);
	div_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divStringVar)
{
	zval a_zv, *b, b_sub;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	b = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	div_function(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, divStringString)
{
	zval a_zv, b_zv;
	zend_string *a = NULL, *b = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&a_zv, a);
	ZVAL_STR(&b_zv, b);
	div_function(return_value, &a_zv, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, divArrayLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	zval a;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	zephir_get_arrval(&a, a_param);
	zephir_div_zval_long(return_value, &a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divArrayVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b, b_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b);
	zephir_get_arrval(&a, a_param);
	div_function(return_value, &a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	div_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divLiteralAbc)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abc");
	div_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divLiteralLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	zephir_div_zval_long(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divNullVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	div_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divNullLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_div_zval_long(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divLiteralArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _1;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	div_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, divInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, x;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&x);
	zephir_div_zval_long(&x, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, divAssignVarLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	div_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, divAssignVarString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *s = NULL;
	zval *a, a_sub, s_zv, x, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	div_function(&_0, &x, &s_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, divAssignVarNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	div_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, divAssignPropertyLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_STRING(&_1, "10");
	ZEPHIR_INIT_VAR(&_2);
	div_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, divAssignPropertyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_NULL(&_1);
	ZEPHIR_INIT_VAR(&_2);
	div_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, modStringLong)
{
	zend_long b;
	zval a_zv, *b_param = NULL;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	b_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	zephir_mod_zval_long(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, modLongString)
{
	zend_string *b = NULL;
	zval *a_param = NULL, b_zv;
	zend_long a;

	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	a_param = ZEND_CALL_ARG(execute_data, 1);
	ZVAL_STR(&b_zv, b);
	zephir_mod_long_zval(return_value, a, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, modStringDouble)
{
	double b;
	zval a_zv, *b_param = NULL;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_DOUBLE(b)
	ZEND_PARSE_PARAMETERS_END();
	b_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	zephir_mod_zval_double(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, modStringBool)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool b;
	zval a_zv, *b_param = NULL, _0;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_BOOL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_0, b);
	mod_function(return_value, &a_zv, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modStringVar)
{
	zval a_zv, *b, b_sub;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	b = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&a_zv, a);
	mod_function(return_value, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, modStringString)
{
	zval a_zv, b_zv;
	zend_string *a = NULL, *b = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_STR(b)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&a_zv, a);
	ZVAL_STR(&b_zv, b);
	mod_function(return_value, &a_zv, &b_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2676Operands, modArrayLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval *a_param = NULL, *b_param = NULL;
	zval a;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	zephir_get_arrval(&a, a_param);
	zephir_mod_zval_long(return_value, &a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modArrayVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b, b_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b);
	zephir_get_arrval(&a, a_param);
	mod_function(return_value, &a, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	mod_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modLiteralAbc)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abc");
	mod_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modLiteralLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	zephir_mod_zval_long(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modNullVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _0;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	mod_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modNullLong)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b_param = NULL, _0;
	zend_long b;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	zephir_mod_zval_long(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modLiteralArray)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *b, b_sub, _1;

	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &b);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 1, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	mod_function(return_value, &_0, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2676Operands, modInferredLocal)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long b;
	zval a_zv, *b_param = NULL, x;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&x);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	b_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&x);
	zephir_mod_zval_long(&x, &a_zv, b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, modAssignVarLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "10");
	ZEPHIR_INIT_VAR(&_1);
	mod_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, modAssignVarString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_string *s = NULL;
	zval *a, a_sub, s_zv, x, _0;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_STR(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	a = ZEND_CALL_ARG(execute_data, 1);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	mod_function(&_0, &x, &s_zv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_0);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, modAssignVarNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, x, _0, _1;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&x);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a);
	ZEPHIR_CPY_WRT(&x, a);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_NULL(&_0);
	ZEPHIR_INIT_VAR(&_1);
	mod_function(&_1, &x, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_CPY_WRT(&x, &_1);
	RETURN_CCTOR(&x);
}

PHP_METHOD(Stub_Issue2676Operands, modAssignPropertyLiteralString)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_STRING(&_1, "10");
	ZEPHIR_INIT_VAR(&_2);
	mod_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

PHP_METHOD(Stub_Issue2676Operands, modAssignPropertyNull)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *obj, obj_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&obj_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
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
	ZVAL_NULL(&_1);
	ZEPHIR_INIT_VAR(&_2);
	mod_function(&_2, &_0, &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_update_property_zval_cached(obj, _zephir_prop_0, 0, &_2);
	zephir_memory_observe(&_3);
	zephir_read_property_cached(&_3, obj, _zephir_prop_0, 0, PH_NOISY_CC);
	RETURN_CCTOR(&_3);
}

