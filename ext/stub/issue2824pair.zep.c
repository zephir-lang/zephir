
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/main.h"
#include "kernel/fcall.h"


/**
 * Named arguments are bound by the callee's parameter names at run time.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2824
 */
ZEPHIR_INIT_CLASS(Stub_Issue2824Pair)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2824Pair, stub, issue2824pair, stub_issue2824pair_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2824Pair, pair)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	ZEPHIR_CONCAT_VSV(return_value, a, ",", b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2824Pair, spair)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	ZEPHIR_CONCAT_VSV(return_value, a, ",", b);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	return;
}

PHP_METHOD(Stub_Issue2824Pair, withDefault)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a = NULL, a_sub, *b = NULL, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 2, &a, &b);
	if (!a) {
		a = &a_sub;
		ZEPHIR_INIT_VAR(a);
		ZVAL_STRING(a, "x");
	}
	if (!b) {
		b = &b_sub;
		ZEPHIR_INIT_VAR(b);
		ZVAL_STRING(b, "y");
	}
	ZEPHIR_CONCAT_VSV(return_value, a, ",", b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Pair, withCallable)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *c = NULL, c_sub, *b = NULL, b_sub, __$null, _0;

	ZVAL_UNDEF(&c_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(c)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	if (c && UNEXPECTED(!zephir_verify_union_arg(c, 1))) {
		RETURN_THROWS();
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 2, &c, &b);
	if (!c) {
		c = &c_sub;
		c = &__$null;
	}
	if (!b) {
		b = &b_sub;
		ZEPHIR_INIT_VAR(b);
		ZVAL_STRING(b, "y");
	}
	ZEPHIR_INIT_VAR(&_0);
	if (Z_TYPE_P(c) == IS_NULL) {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_STRING(&_0, "null");
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_STRING(&_0, "set");
	}
	ZEPHIR_CONCAT_VSV(return_value, &_0, ",", b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Pair, withObject)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *o = NULL, o_sub, *b = NULL, b_sub, __$null, _0;

	ZVAL_UNDEF(&o_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(o)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	if (o && UNEXPECTED(!zephir_verify_union_arg(o, 1))) {
		RETURN_THROWS();
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 2, &o, &b);
	if (!o) {
		o = &o_sub;
		o = &__$null;
	}
	if (!b) {
		b = &b_sub;
		ZEPHIR_INIT_VAR(b);
		ZVAL_STRING(b, "y");
	}
	ZEPHIR_INIT_VAR(&_0);
	if (Z_TYPE_P(o) == IS_NULL) {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_STRING(&_0, "null");
	} else {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_STRING(&_0, "set");
	}
	ZEPHIR_CONCAT_VSV(return_value, &_0, ",", b);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Pair, withInt)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, _0, _1;
	zend_long a, b;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(0, 2)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 2, &a_param, &b_param);
	if (!a_param) {
		a = 5;
	} else {
		}
	if (!b_param) {
		b = 6;
	} else {
		}
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, a);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, b);
	ZEPHIR_CONCAT_VSV(return_value, &_0, ",", &_1);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Pair, variadic)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a, a_sub, rest;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&rest);
	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ZVAL(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params_variadic(1, 1, 0, &a);
	ZEPHIR_INIT_VAR(&rest);
	zephir_get_args_from(&rest, 1);
	zephir_create_array(return_value, 2, 0);
	zephir_array_fast_append(return_value, a);
	zephir_array_fast_append(return_value, &rest);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Pair, thisPair)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_TLS zephir_method_cache _zephir_call_cache[1];
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_method, NULL, this_ptr, "pair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Pair, selfPair)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_self, zephir_get_calling_scope(), getThis(), "spair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Issue2824Pair, staticPair)
{
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "2");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "1");
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 2, 0);
	zephir_array_update_string(&_2, SL("b"), &_0, PH_COPY);
	zephir_array_update_string(&_2, SL("a"), &_1, PH_COPY);
	ZEPHIR_RETURN_CALL_CLASS_METHOD_NAMED(zephir_fcall_static, zephir_get_called_scope(EG(current_execute_data)), getThis(), "spair", Z_ARRVAL_P(&_2));
	ZEPHIR_INIT_NVAR(&_2);
	zephir_check_call_status();
	RETURN_MM();
}

