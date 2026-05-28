
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
#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/array.h"


/**
 * Free (non-method) variadic function — the exact form from issue #2025:
 * `function f(...params)`. Exercises the PHP_FUNCTION code path.
 */
ZEPHIR_INIT_CLASS(Stub_Variadic)
{
	ZEPHIR_REGISTER_CLASS(Stub, Variadic, stub, variadic, stub_variadic_method_entry, 0);

	return SUCCESS;
}

/**
 * Variadic only, untyped: collects every argument into an array.
 */
PHP_METHOD(Stub_Variadic, collect)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval args;

	ZVAL_UNDEF(&args);
	ZEND_PARSE_PARAMETERS_START(0, -1)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	ZEPHIR_INIT_VAR(&args);
	zephir_get_args_from(&args, 0);
	RETVAL_ZVAL(&args, 1, 0);
	RETURN_MM();
}

/**
 * Fixed required parameter followed by a variadic one.
 */
PHP_METHOD(Stub_Variadic, sum)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *first_param = NULL, rest, n, *_0;
	zend_long first, ZEPHIR_LAST_CALL_STATUS, total;

	ZVAL_UNDEF(&rest);
	ZVAL_UNDEF(&n);
	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_LONG(first)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params_variadic(1, 1, 0, &first_param);
	ZEPHIR_INIT_VAR(&rest);
	zephir_get_args_from(&rest, 1);
	total = first;
	zephir_is_iterable(&rest, 0, "stub/variadic.zep", 41);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&rest), _0)
	{
		ZEPHIR_INIT_NVAR(&n);
		ZVAL_COPY(&n, _0);
		total += zephir_get_numberval(&n);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&n);
	RETURN_MM_LONG(total);
}

/**
 * Counts only the variadic (trailing) arguments.
 */
PHP_METHOD(Stub_Variadic, countExtra)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *required, required_sub, rest;

	ZVAL_UNDEF(&required_sub);
	ZVAL_UNDEF(&rest);
	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_ZVAL(required)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params_variadic(1, 1, 0, &required);
	ZEPHIR_INIT_VAR(&rest);
	zephir_get_args_from(&rest, 1);
	RETURN_MM_LONG(zephir_fast_count_int(&rest));
}

/**
 * Typed variadic parameter.
 */
PHP_METHOD(Stub_Variadic, joinStrings)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval glue_zv, parts;
	zend_string *glue = NULL;

	ZVAL_UNDEF(&glue_zv);
	ZVAL_UNDEF(&parts);
	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_STR(glue)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&glue_zv);
	ZVAL_STR_COPY(&glue_zv, glue);
	ZEPHIR_INIT_VAR(&parts);
	zephir_get_args_from(&parts, 1);
	zephir_fast_join(return_value, &glue_zv, &parts);
	RETURN_MM();
}

/**
 * Required + optional fixed parameter followed by a variadic one
 * (the example from issue #2025).
 */
PHP_METHOD(Stub_Variadic, describe)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *req_param = NULL, *opt_param = NULL, params, _0;
	zend_long req, opt;

	ZVAL_UNDEF(&params);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, -1)
		Z_PARAM_LONG(req)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(opt)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params_variadic(1, 1, 1, &req_param, &opt_param);
	if (!opt_param) {
		opt = 0;
	} else {
		}
	ZEPHIR_INIT_VAR(&params);
	zephir_get_args_from(&params, 2);
	zephir_create_array(return_value, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, req);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, opt);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, zephir_fast_count_int(&params));
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

/**
 * Static variadic method — exercises the static-context code path.
 */
PHP_METHOD(Stub_Variadic, staticCollect)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval args;

	ZVAL_UNDEF(&args);
	ZEND_PARSE_PARAMETERS_START(0, -1)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	ZEPHIR_INIT_VAR(&args);
	zephir_get_args_from(&args, 0);
	RETVAL_ZVAL(&args, 1, 0);
	RETURN_MM();
}

/**
 * Array-typed variadic: each argument is an array; returns the total
 * number of elements across all of them.
 */
PHP_METHOD(Stub_Variadic, totalLength)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, total;
	zval rows, row, *_0;

	ZVAL_UNDEF(&rows);
	ZVAL_UNDEF(&row);
	ZEND_PARSE_PARAMETERS_START(0, -1)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	ZEPHIR_INIT_VAR(&rows);
	zephir_get_args_from(&rows, 0);
	total = 0;
	zephir_is_iterable(&rows, 0, "stub/variadic.zep", 90);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&rows), _0)
	{
		ZEPHIR_INIT_NVAR(&row);
		ZVAL_COPY(&row, _0);
		total += zephir_fast_count_int(&row);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&row);
	RETURN_MM_LONG(total);
}

/**
 * Collects objects and returns their class names. Exercises a variadic
 * that carries object values.
 */
PHP_METHOD(Stub_Variadic, classNames)
{
	zval names;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval objects, obj, *_0, _1$$3;

	ZVAL_UNDEF(&objects);
	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&names);
	ZEND_PARSE_PARAMETERS_START(0, -1)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	ZEPHIR_INIT_VAR(&objects);
	zephir_get_args_from(&objects, 0);
	ZEPHIR_INIT_VAR(&names);
	array_init(&names);
	zephir_is_iterable(&objects, 0, "stub/variadic.zep", 106);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&objects), _0)
	{
		ZEPHIR_INIT_NVAR(&obj);
		ZVAL_COPY(&obj, _0);
		ZEPHIR_INIT_NVAR(&_1$$3);
		zephir_get_class(&_1$$3, &obj, 0);
		zephir_array_append(&names, &_1$$3, PH_SEPARATE, "stub/variadic.zep", 103);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&obj);
	RETURN_CTOR(&names);
}

PHP_FUNCTION(f_Stub_variadicSum) {
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, total;
	zval numbers, n, *_0;
		ZVAL_UNDEF(&numbers);
	ZVAL_UNDEF(&n);
	ZEND_PARSE_PARAMETERS_START(0, -1)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	ZEPHIR_INIT_VAR(&numbers);
	zephir_get_args_from(&numbers, 0);
	total = 0;
	zephir_is_iterable(&numbers, 0, "stub/variadic.zep", 16);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&numbers), _0)
	{
		ZEPHIR_INIT_NVAR(&n);
		ZVAL_COPY(&n, _0);
		total += zephir_get_numberval(&n);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&n);
	RETURN_MM_LONG(total);
}

