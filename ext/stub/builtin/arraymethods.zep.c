
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"


ZEPHIR_INIT_CLASS(Stub_BuiltIn_ArrayMethods)
{
	ZEPHIR_REGISTER_CLASS(Stub\\BuiltIn, ArrayMethods, stub, builtin_arraymethods, stub_builtin_arraymethods_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_BuiltIn_ArrayMethods, getJoin1)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
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
	zephir_fast_join_str(&_1, SL("-"), &_0);
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_ArrayMethods, getReversed1)
{
	zval _1, _2;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_LONG(&_1, 1);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 2);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 3);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_CALL_FUNCTION(&_2, "array_reverse", NULL, 8, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_2);
}

PHP_METHOD(Stub_BuiltIn_ArrayMethods, getMap1)
{
	zval _1, _2;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
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
	ZEPHIR_INIT_NVAR(&_1);
	zephir_create_closure_ex(&_1, NULL, stub_0__closure_ce, SL("__invoke"));
	ZEPHIR_CALL_FUNCTION(&_2, "array_map", NULL, 9, &_1, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_2);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/733
 */
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733BuiltInJoinSpecialChars)
{
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_6 = NULL;
	zval a_zv, *b = NULL, b_sub, __$null, _3, _4, _5, _7, _1$$3, _2$$4;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&b_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$4);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(a)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 1) {
		b = ZEND_CALL_ARG(execute_data, 2);
	}
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	if (!b) {
		b = &b_sub;
		ZEPHIR_CPY_WRT(b, &__$null);
	} else {
		ZEPHIR_SEPARATE_PARAM(b);
	}
	_0 = Z_TYPE_P(b) == IS_NULL;
	if (!(_0)) {
		_0 = ZEPHIR_IS_FALSE_IDENTICAL(b);
	}
	if (_0) {
		ZEPHIR_INIT_VAR(&_1$$3);
		zephir_fast_trim(&_1$$3, &a_zv, NULL , ZEPHIR_TRIM_LEFT);
		RETURN_CCTOR(&_1$$3);
	}
	if (Z_TYPE_P(b) == IS_ARRAY) {
		ZEPHIR_INIT_VAR(&_2$$4);
		zephir_fast_join_str(&_2$$4, SL(""), b);
		ZEPHIR_CPY_WRT(b, &_2$$4);
	}
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "#[-\\[\\]:\\\\^/]#");
	ZEPHIR_INIT_VAR(&_4);
	ZVAL_STRING(&_4, "\\\\$0");
	ZEPHIR_CALL_FUNCTION(&_5, "preg_replace", &_6, 10, &_3, &_4, b);
	zephir_check_call_status();
	ZEPHIR_CPY_WRT(b, &_5);
	ZEPHIR_INIT_VAR(&_7);
	ZEPHIR_CONCAT_SVS(&_7, "/^[", b, "]+/u");
	ZEPHIR_INIT_NVAR(&_3);
	ZVAL_STRING(&_3, "");
	ZEPHIR_RETURN_CALL_FUNCTION("preg_replace", &_6, 10, &_7, &_3, &a_zv);
	zephir_check_call_status();
	RETURN_MM();
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/733
 * Exercises the bare-minimum case: a `var` holding an array, joined directly.
 */
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733JoinOnVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *input, input_sub, _0;

	ZVAL_UNDEF(&input_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(input)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &input);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_join_str(&_0, SL(","), input);
	RETURN_CCTOR(&_0);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/733
 * `reversed()` is also in the whitelist — verifies the fix isn't `join`-only.
 */
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733ReversedOnVar)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *input, input_sub, _0;

	ZVAL_UNDEF(&input_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(input)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &input);
	ZEPHIR_CALL_FUNCTION(&_0, "array_reverse", NULL, 8, input);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/733
 * Ensures the standard happy path (typed array literal) still works after
 * the dispatch change.
 */
PHP_METHOD(Stub_BuiltIn_ArrayMethods, issue733JoinOnLiteralArray)
{
	zval _1;
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 3, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "a");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "b");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "c");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	zephir_fast_join_str(&_1, SL("|"), &_0);
	RETURN_CCTOR(&_1);
}

