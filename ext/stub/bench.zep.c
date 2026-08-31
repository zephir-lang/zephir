
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "ext/spl/spl_array.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/concat.h"
#include "kernel/main.h"
#include "kernel/generator.h"
#include "math.h"


/**
 * Workloads for the PHPBench suite under tests/Benchmark/.
 *
 * Each method runs a tight internal loop so PHPBench's per-iteration cost
 * (revs/iterations setup) stays small relative to the work being measured.
 * Pairs one-to-one with a pure-PHP equivalent in the benchmark class so the
 * reported metric is the Zephir-over-PHP speedup margin.
 */
ZEPHIR_INIT_CLASS(Stub_Bench)
{
	ZEPHIR_REGISTER_CLASS(Stub, Bench, stub, bench, stub_bench_method_entry, 0);

	zend_declare_property_long(stub_bench_ce, SL("propA"), 0, ZEND_ACC_PUBLIC);
	zend_declare_property_string(stub_bench_ce, SL("propB"), "hello", ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(Stub_Bench, __construct)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	static zend_string *_zephir_prop_0 = NULL;
	static zend_string *_zephir_prop_1 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("propA", 5, 1);
	}
	if (UNEXPECTED(!_zephir_prop_1)) {
		_zephir_prop_1 = zend_string_init("propB", 5, 1);
	}
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 42);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 20, &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_1, 21, &_1);
	ZEPHIR_MM_RESTORE();
}

/**
 * For-over-array baseline. The compiler skips the IS_ARRAY branch wrapper
 * since `arr` is known to be array (see issue #1878).
 */
PHP_METHOD(Stub_Bench, forOverArray)
{
	zend_bool _2;
	zend_long sum;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *arr_param = NULL, v, *_0, _1;
	zval arr;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(arr, arr_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &arr_param);
	ZEPHIR_OBS_COPY_OR_DUP(&arr, arr_param);
	sum = 0;
	zephir_is_iterable(&arr, 0, "stub/bench.zep", 33);
	if (Z_TYPE_P(&arr) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&arr), _0)
		{
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _0);
			sum += zephir_get_intval(&v);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &arr, "rewind", NULL, 0);
		zephir_check_call_status();
		_2 = 1;
		while (1) {
			if (_2) {
				_2 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, &arr, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_1, &arr, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_1)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, &arr, "current", NULL, 0);
			zephir_check_call_status();
				sum += zephir_get_intval(&v);
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	RETURN_MM_LONG(sum);
}

/**
 * For-over-Iterator. The compiler skips the IS_ARRAY branch wrapper
 * since the iterand is known to be an object (issue #1878).
 */
PHP_METHOD(Stub_Bench, forOverIterator)
{
	zend_bool _5;
	zval _0;
	zend_long sum;
	zval v, iter, _1, *_2, *_3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&iter);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	sum = 0;
	ZEPHIR_INIT_VAR(&iter);
	object_init_ex(&iter, spl_ce_ArrayIterator);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 10, 0);
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
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 6);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 7);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 8);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 9);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 10);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_CALL_METHOD(NULL, &iter, "__construct", NULL, 8, &_0);
	zephir_check_call_status();
	if (Z_TYPE_P(&iter) == IS_STRING) {
		ZEPHIR_INIT_NVAR(&_1);
		zephir_string_to_char_array(&_1, &iter);
		_2 = &_1;
	} else {
		_2 = &iter;
	}
	zephir_is_iterable(_2, 0, "stub/bench.zep", 48);
	if (Z_TYPE_P(_2) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_2), _3)
		{
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _3);
			sum += zephir_get_intval(&v);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _2, "rewind", NULL, 0);
		zephir_check_call_status();
		_5 = 1;
		while (1) {
			if (_5) {
				_5 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _2, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_4, _2, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_4)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, _2, "current", NULL, 0);
			zephir_check_call_status();
				sum += zephir_get_intval(&v);
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	RETURN_MM_LONG(sum);
}

/**
 * For-over-Iterator with `continue`, the exact path fixed in issue #2546.
 * The first-iteration guard (zend_bool flag) introduced by the fix adds one
 * extra conditional per iteration; this subject quantifies that overhead
 * against the plain-iterator path and against pure-PHP foreach+continue.
 * Skips every even element (5 of 10 trigger continue).
 */
PHP_METHOD(Stub_Bench, forOverIteratorWithContinue)
{
	zend_bool _5;
	zval _0;
	zend_long sum, iv;
	zval v, iter, _1, *_2, *_3, _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&iter);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	sum = 0;
	iv = 0;
	ZEPHIR_INIT_VAR(&iter);
	object_init_ex(&iter, spl_ce_ArrayIterator);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 10, 0);
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
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 5);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 6);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 7);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 8);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 9);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_LONG(&_1, 10);
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_CALL_METHOD(NULL, &iter, "__construct", NULL, 8, &_0);
	zephir_check_call_status();
	if (Z_TYPE_P(&iter) == IS_STRING) {
		ZEPHIR_INIT_NVAR(&_1);
		zephir_string_to_char_array(&_1, &iter);
		_2 = &_1;
	} else {
		_2 = &iter;
	}
	zephir_is_iterable(_2, 0, "stub/bench.zep", 70);
	if (Z_TYPE_P(_2) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_2), _3)
		{
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _3);
			iv = zephir_get_intval(&v);
			if (zephir_safe_mod_long_long(iv, 2) == 0) {
				continue;
			}
			sum += iv;
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _2, "rewind", NULL, 0);
		zephir_check_call_status();
		_5 = 1;
		while (1) {
			if (_5) {
				_5 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _2, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_4, _2, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_4)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, _2, "current", NULL, 0);
			zephir_check_call_status();
				iv = zephir_get_intval(&v);
				if (zephir_safe_mod_long_long(iv, 2) == 0) {
					continue;
				}
				sum += iv;
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	RETURN_MM_LONG(sum);
}

/**
 * Object property read in a hot loop. Exercises zephir_read_property.
 */
PHP_METHOD(Stub_Bench, propertyReadLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, _0$$3;
	zend_long n, i, sum;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("propA", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_OBS_NVAR(&_0$$3);
		zephir_read_property_cached(&_0$$3, this_ptr, _zephir_prop_0, 20, PH_NOISY_CC);
		sum += zephir_get_intval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * Array fetch by string key in a hot loop. Exercises
 * zephir_array_fetch_string against precomputed-length char* keys.
 */
PHP_METHOD(Stub_Bench, arrayFetchStringLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long n, i, sum;
	zval *arr_param = NULL, *n_param = NULL, _0$$3, _1$$3;
	zval arr;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(arr, arr_param)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &arr_param, &n_param);
	ZEPHIR_OBS_COPY_OR_DUP(&arr, arr_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_OBS_NVAR(&_0$$3);
		zephir_array_fetch_string(&_0$$3, &arr, SL("a"), PH_NOISY, "stub/bench.zep", 94);
		ZEPHIR_OBS_NVAR(&_1$$3);
		zephir_array_fetch_string(&_1$$3, &arr, SL("b"), PH_NOISY, "stub/bench.zep", 94);
		sum += (zephir_get_intval(&_0$$3) + zephir_get_intval(&_1$$3));
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * Method dispatch in a hot loop. Exercises zephir_call_method on `this`.
 */
PHP_METHOD(Stub_Bench, methodDispatchLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, _0$$3;
	zend_long n, i, sum;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&_0$$3, this_ptr, "returnconst", &_1, 0);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

PHP_METHOD(Stub_Bench, returnConst)
{

	RETURN_LONG(7);
}

/**
 * String concatenation in a hot loop. Exercises ZEPHIR_CONCAT_*.
 */
PHP_METHOD(Stub_Bench, stringConcatLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long n, i;
	zval base_zv, *n_param = NULL, out, _0$$3;
	zend_string *base = NULL;

	ZVAL_UNDEF(&base_zv);
	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(base)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	n_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&base_zv);
	ZVAL_STR_COPY(&base_zv, base);
	i = 0;
	ZEPHIR_INIT_VAR(&out);
	ZVAL_STRING(&out, "");
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_0$$3);
		ZEPHIR_CONCAT_VV(&_0$$3, &out, &base_zv);
		ZEPHIR_CPY_WRT(&out, &_0$$3);
		i++;
	}
	RETURN_CCTOR(&out);
}

/**
 * Object instantiation + immediate method call.
 */
PHP_METHOD(Stub_Bench, newInstanceLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_2 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, obj, _1$$3;
	zend_long n, i, sum;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&obj);
		object_init_ex(&obj, stub_bench_ce);
		ZEPHIR_CALL_METHOD(NULL, &obj, "__construct", &_0, 9);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1$$3, &obj, "returnconst", &_2, 10);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_1$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * Variadic callee (issue #2025). Sums every trailing argument, exercising
 * the zephir_get_args_from() collection emitted for `...` parameters.
 */
PHP_METHOD(Stub_Bench, variadicSum)
{
	zend_long sum;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval numbers, v, *_0, _1, *_2;

	ZVAL_UNDEF(&numbers);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(0, -1)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	ZEPHIR_INIT_VAR(&numbers);
	zephir_get_args_from(&numbers, 0);
	sum = 0;
	if (Z_TYPE_P(&numbers) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, &numbers);
		_0 = &_1;
	} else {
		_0 = &numbers;
	}
	zephir_is_iterable(_0, 0, "stub/bench.zep", 159);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_0), _2)
	{
		ZEPHIR_INIT_NVAR(&v);
		ZVAL_COPY(&v, _2);
		sum += zephir_get_intval(&v);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&v);
	RETURN_MM_LONG(sum);
}

/**
 * Hot loop calling the variadic method with a 5-argument tail. Measures
 * the per-call cost of collecting the trailing arguments into an array.
 */
PHP_METHOD(Stub_Bench, variadicSumNarrowLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_6 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, _0$$3, _1$$3, _2$$3, _3$$3, _4$$3, _5$$3;
	zend_long n, i, sum;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZVAL_LONG(&_1$$3, 1);
		ZVAL_LONG(&_2$$3, 2);
		ZVAL_LONG(&_3$$3, 3);
		ZVAL_LONG(&_4$$3, 4);
		ZVAL_LONG(&_5$$3, 5);
		ZEPHIR_CALL_METHOD(&_0$$3, this_ptr, "variadicsum", &_6, 0, &_1$$3, &_2$$3, &_3$$3, &_4$$3, &_5$$3);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * Same as variadicSumNarrowLoop but with a 15-argument tail, so the report
 * shows how the collection cost scales with the number of variadic args.
 */
PHP_METHOD(Stub_Bench, variadicSumWideLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_16 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, _0$$3, _1$$3, _2$$3, _3$$3, _4$$3, _5$$3, _6$$3, _7$$3, _8$$3, _9$$3, _10$$3, _11$$3, _12$$3, _13$$3, _14$$3, _15$$3;
	zend_long n, i, sum;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$3);
	ZVAL_UNDEF(&_9$$3);
	ZVAL_UNDEF(&_10$$3);
	ZVAL_UNDEF(&_11$$3);
	ZVAL_UNDEF(&_12$$3);
	ZVAL_UNDEF(&_13$$3);
	ZVAL_UNDEF(&_14$$3);
	ZVAL_UNDEF(&_15$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZVAL_LONG(&_1$$3, 1);
		ZVAL_LONG(&_2$$3, 2);
		ZVAL_LONG(&_3$$3, 3);
		ZVAL_LONG(&_4$$3, 4);
		ZVAL_LONG(&_5$$3, 5);
		ZVAL_LONG(&_6$$3, 6);
		ZVAL_LONG(&_7$$3, 7);
		ZVAL_LONG(&_8$$3, 8);
		ZVAL_LONG(&_9$$3, 9);
		ZVAL_LONG(&_10$$3, 10);
		ZVAL_LONG(&_11$$3, 11);
		ZVAL_LONG(&_12$$3, 12);
		ZVAL_LONG(&_13$$3, 13);
		ZVAL_LONG(&_14$$3, 14);
		ZVAL_LONG(&_15$$3, 15);
		ZEPHIR_CALL_METHOD(&_0$$3, this_ptr, "variadicsum", &_16, 0, &_1$$3, &_2$$3, &_3$$3, &_4$$3, &_5$$3, &_6$$3, &_7$$3, &_8$$3, &_9$$3, &_10$$3, &_11$$3, &_12$$3, &_13$$3, &_14$$3, &_15$$3);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * The pre-variadic idiom that `...` replaces: a fixed-arity method reading
 * its arguments through func_get_args(). Paired with variadicSum so the
 * report contrasts the `...` operator against the old workaround.
 */
PHP_METHOD(Stub_Bench, funcGetArgsSum)
{
	zend_long sum;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, *c, c_sub, *d, d_sub, *e, e_sub, args, v, *_0, _1, *_2;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&c_sub);
	ZVAL_UNDEF(&d_sub);
	ZVAL_UNDEF(&e_sub);
	ZVAL_UNDEF(&args);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(5, 5)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
		Z_PARAM_ZVAL(c)
		Z_PARAM_ZVAL(d)
		Z_PARAM_ZVAL(e)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 5, 0, &a, &b, &c, &d, &e);
	sum = 0;
	ZEPHIR_INIT_VAR(&args);
	zephir_get_args(&args);
	if (Z_TYPE_P(&args) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, &args);
		_0 = &_1;
	} else {
		_0 = &args;
	}
	zephir_is_iterable(_0, 0, "stub/bench.zep", 203);
	ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_0), _2)
	{
		ZEPHIR_INIT_NVAR(&v);
		ZVAL_COPY(&v, _2);
		sum += zephir_get_intval(&v);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&v);
	RETURN_MM_LONG(sum);
}

PHP_METHOD(Stub_Bench, funcGetArgsSumLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_6 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, _0$$3, _1$$3, _2$$3, _3$$3, _4$$3, _5$$3;
	zend_long n, i, sum;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZVAL_LONG(&_1$$3, 1);
		ZVAL_LONG(&_2$$3, 2);
		ZVAL_LONG(&_3$$3, 3);
		ZVAL_LONG(&_4$$3, 4);
		ZVAL_LONG(&_5$$3, 5);
		ZEPHIR_CALL_METHOD(&_0$$3, this_ptr, "funcgetargssum", &_6, 0, &_1$$3, &_2$$3, &_3$$3, &_4$$3, &_5$$3);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * Call-dispatch micro-workloads for the FastCall investigation (#1510).
 * Each loops a trivial callee so the measured cost is dominated by the
 * call machinery (cache lookup + fcic setup + zend_call_function), not the
 * callee body. Three dispatch flavours exercise distinct kernel paths.
 */
PHP_METHOD(Stub_Bench, finalReturnConst)
{

	RETURN_LONG(7);
}

PHP_METHOD(Stub_Bench, staticReturnConst)
{

	RETURN_LONG(7);
}

PHP_METHOD(Stub_Bench, finalDispatchLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, _0$$3;
	zend_long n, i, sum;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&_0$$3, this_ptr, "finalreturnconst", &_1, 11);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

PHP_METHOD(Stub_Bench, staticDispatchLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_1 = NULL;
	zval *n_param = NULL, _0$$3;
	zend_long n, i, sum;

	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_CALL_SELF(&_0$$3, "staticreturnconst", &_1, 0);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_0$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * Generator (yield) workloads — issue #1849. Each producer is paired
 * with an array producer of identical shape so the benchmarks compare
 * lazy suspension against eager materialization, plus an Iterator relay.
 */
PHP_METHOD(Stub_Bench, generatorRange)
{
	zval *n_param = NULL;
	zend_long n;
	zval *this_ptr = getThis();

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	zephir_generator_create(return_value, this_ptr, stub_bench_ce, zep_Stub_Bench_zephir_gen_step_generatorRange, 1);
	zephir_generator_slot_set_long(return_value, 0, (zend_long) n);
	return;
}

PHP_METHOD(Stub_Bench, arrayRange)
{
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, _0$$3;
	zend_long n, i;

	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	i = 1;
	while (1) {
		if (!(i <= n)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_0$$3);
		ZVAL_LONG(&_0$$3, i);
		zephir_array_append(&result, &_0$$3, PH_SEPARATE, "stub/bench.zep", 271);
		i = (i + 1);
	}
	RETURN_CTOR(&result);
}

/**
 * Generator relaying an array: exercises the suspension-safe
 * HashPosition/snapshot for-in codegen (one suspension per element).
 */
PHP_METHOD(Stub_Bench, generatorOverArray)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *items_param = NULL;
	zval items;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&items);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(items, items_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &items_param);
	zephir_get_arrval(&items, items_param);
	zephir_generator_create(return_value, this_ptr, stub_bench_ce, zep_Stub_Bench_zephir_gen_step_generatorOverArray, 1);
	zephir_generator_slot_set(return_value, 0, &items);
	ZEPHIR_MM_RESTORE();
	return;
}

/**
 * Produce and consume entirely inside the extension: the C-to-C cost of
 * one suspension+resume per element, no PHP userland in the loop.
 */
PHP_METHOD(Stub_Bench, sumViaGenerator)
{
	zend_bool _6;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, v, _0, _1, *_2, _3, *_4, _5;
	zend_long n, total;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	total = 0;
	ZVAL_LONG(&_1, n);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "generatorrange", NULL, 0, &_1);
	zephir_check_call_status();
	if (Z_TYPE_P(&_0) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_3);
		zephir_string_to_char_array(&_3, &_0);
		_2 = &_3;
	} else {
		_2 = &_0;
	}
	zephir_is_iterable(_2, 0, "stub/bench.zep", 300);
	if (Z_TYPE_P(_2) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_2), _4)
		{
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _4);
			total += zephir_get_intval(&v);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _2, "rewind", NULL, 0);
		zephir_check_call_status();
		_6 = 1;
		while (1) {
			if (_6) {
				_6 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _2, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_5, _2, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_5)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, _2, "current", NULL, 0);
			zephir_check_call_status();
				total += zephir_get_intval(&v);
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	RETURN_MM_LONG(total);
}

PHP_METHOD(Stub_Bench, sumViaArray)
{
	zend_bool _6;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, v, _0, _1, *_2, _3, *_4, _5;
	zend_long n, total;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	total = 0;
	ZVAL_LONG(&_1, n);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "arrayrange", NULL, 0, &_1);
	zephir_check_call_status();
	if (Z_TYPE_P(&_0) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_3);
		zephir_string_to_char_array(&_3, &_0);
		_2 = &_3;
	} else {
		_2 = &_0;
	}
	zephir_is_iterable(_2, 0, "stub/bench.zep", 310);
	if (Z_TYPE_P(_2) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_2), _4)
		{
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _4);
			total += zephir_get_intval(&v);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _2, "rewind", NULL, 0);
		zephir_check_call_status();
		_6 = 1;
		while (1) {
			if (_6) {
				_6 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _2, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_5, _2, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_5)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, _2, "current", NULL, 0);
			zephir_check_call_status();
				total += zephir_get_intval(&v);
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	RETURN_MM_LONG(total);
}

/**
 * `for i in 0..n` over the `..` range operator (#2433). Compiles to an
 * integer counting loop; previously it materialised an n-element array.
 * Paired with sumRangeFn (explicit range() call) and the pure-PHP baseline
 * so the report shows the operator now matches range() and pure PHP.
 */
PHP_METHOD(Stub_Bench, sumRangeOperator)
{
	zend_long _1;
	zend_bool _0;
	zval *n_param = NULL;
	zend_long n, i, total, _2;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	i = 0;
	total = 0;
	_2 = n;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			total += i;
		}
	}
	RETURN_LONG(total);
}

PHP_METHOD(Stub_Bench, sumRangeFn)
{
	zend_long _1;
	zend_bool _0;
	zval *n_param = NULL;
	zend_long n, i, total, _2;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	i = 0;
	total = 0;
	_2 = n;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			total += i;
		}
	}
	RETURN_LONG(total);
}

/**
 * Builds an (n+1) x (n+1) matrix via `let output[i][j] = 1` in nested
 * loops (the issue #1884 shape). Isolates the multi-dimensional array
 * write cost; paired with a pure-PHP nested-`for` baseline in the bench.
 */
PHP_METHOD(Stub_Bench, buildMatrix)
{
	zend_long _1, _5$$3;
	zend_bool _0, _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, output, i, j, _3$$3, _7$$4;
	zend_long n, _2, _6$$3;

	ZVAL_UNDEF(&output);
	ZVAL_UNDEF(&i);
	ZVAL_UNDEF(&j);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_7$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&output);
	array_init(&output);
	ZEPHIR_INIT_VAR(&i);
	ZVAL_LONG(&i, 0);
	ZEPHIR_INIT_VAR(&j);
	ZVAL_LONG(&j, 0);
	_2 = n;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			ZEPHIR_INIT_NVAR(&_3$$3);
			array_init(&_3$$3);
			zephir_array_update_zval(&output, &i, &_3$$3, PH_COPY | PH_SEPARATE);
			_6$$3 = n;
			_5$$3 = 0;
			_4$$3 = 0;
			if (_5$$3 <= _6$$3) {
				while (1) {
					if (_4$$3) {
						_5$$3++;
						if (!(_5$$3 <= _6$$3)) {
							break;
						}
					} else {
						_4$$3 = 1;
					}
					ZEPHIR_INIT_NVAR(&j);
					ZVAL_LONG(&j, _5$$3);
					ZEPHIR_INIT_NVAR(&_7$$4);
					ZVAL_LONG(&_7$$4, 1);
					zephir_array_update_multi(&output, &_7$$4, SL("zz"), 2, &i, &j);
				}
			}
		}
	}
	RETURN_CCTOR(&output);
}

/**
 * Array `+=` union with a literal right-hand side (issue #1280). Compiler
 * path: emits zephir_add_function(&a, &a, &<literal>), an in-place union.
 * `a` is reset each step so the measured cost is one fixed-size union.
 */
PHP_METHOD(Stub_Bench, addAssignUnionLiteral)
{
	zval _4$$3;
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *count_param = NULL, a, _3$$3;
	zend_long count, i, _2;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &count_param);
	i = 0;
	_2 = count;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			ZEPHIR_INIT_NVAR(&a);
			zephir_create_array(&a, 5, 0);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 1);
			zephir_array_fast_append(&a, &_3$$3);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 2);
			zephir_array_fast_append(&a, &_3$$3);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 3);
			zephir_array_fast_append(&a, &_3$$3);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 4);
			zephir_array_fast_append(&a, &_3$$3);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 5);
			zephir_array_fast_append(&a, &_3$$3);
			ZEPHIR_INIT_NVAR(&_4$$3);
			zephir_create_array(&_4$$3, 5, 0);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 6);
			zephir_array_fast_append(&_4$$3, &_3$$3);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 7);
			zephir_array_fast_append(&_4$$3, &_3$$3);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, 8);
			zephir_array_fast_append(&_4$$3, &_3$$3);
			add_assoc_long_ex(&_4$$3, SL("k1"), 1);
			add_assoc_long_ex(&_4$$3, SL("k2"), 2);
			zephir_add_function(&a, &a, &_4$$3);
		}
	}
	ZEPHIR_MM_RESTORE();
}

/**
 * Array `+=` union with an untyped `var += var` right-hand side (#1280).
 * Runtime path: the ZEPHIR_ADD_ASSIGN macro dispatches to an in-place
 * hash merge. Same fixed-size union per step as addAssignUnionLiteral.
 */
PHP_METHOD(Stub_Bench, addAssignUnionVar)
{
	zend_long _2;
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *count_param = NULL, a, b, _0, _4$$3;
	zend_long count, i, _3;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_4$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &count_param);
	i = 0;
	ZEPHIR_INIT_VAR(&b);
	zephir_create_array(&b, 5, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 6);
	zephir_array_fast_append(&b, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 7);
	zephir_array_fast_append(&b, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 8);
	zephir_array_fast_append(&b, &_0);
	add_assoc_long_ex(&b, SL("k1"), 1);
	add_assoc_long_ex(&b, SL("k2"), 2);
	_3 = count;
	_2 = 1;
	_1 = 0;
	if (_2 <= _3) {
		while (1) {
			if (_1) {
				_2++;
				if (!(_2 <= _3)) {
					break;
				}
			} else {
				_1 = 1;
			}
			i = _2;
			ZEPHIR_INIT_NVAR(&a);
			zephir_create_array(&a, 5, 0);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZVAL_LONG(&_4$$3, 1);
			zephir_array_fast_append(&a, &_4$$3);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZVAL_LONG(&_4$$3, 2);
			zephir_array_fast_append(&a, &_4$$3);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZVAL_LONG(&_4$$3, 3);
			zephir_array_fast_append(&a, &_4$$3);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZVAL_LONG(&_4$$3, 4);
			zephir_array_fast_append(&a, &_4$$3);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZVAL_LONG(&_4$$3, 5);
			zephir_array_fast_append(&a, &_4$$3);
			ZEPHIR_ADD_ASSIGN(&a, &b);
		}
	}
	ZEPHIR_MM_RESTORE();
}

/**
 * Array `+=` accumulation via untyped `var += var`: grows `a` by one key
 * per step. Confirms the macro merges in place (O(n) amortized) instead of
 * duplicating the accumulator each step.
 */
PHP_METHOD(Stub_Bench, addAssignAccumulate)
{
	zend_long _1;
	zend_bool _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *count_param = NULL, a, b, _3$$3;
	zend_long count, i, _2;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &count_param);
	i = 0;
	ZEPHIR_INIT_VAR(&a);
	array_init(&a);
	_2 = count;
	_1 = 1;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			ZEPHIR_INIT_NVAR(&b);
			array_init(&b);
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_array_update_long(&b, i, &_3$$3, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
			ZEPHIR_ADD_ASSIGN(&a, &b);
		}
	}
	RETURN_CCTOR(&a);
}

/**
 * count() with a single argument (#2468). CountOptimizer inlines
 * zephir_fast_count_int, so no Zend function-table dispatch happens.
 */
PHP_METHOD(Stub_Bench, countOptimizedLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long n, i, sum;
	zval *arr_param = NULL, *n_param = NULL;
	zval arr;

	ZVAL_UNDEF(&arr);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(arr, arr_param)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &arr_param, &n_param);
	ZEPHIR_OBS_COPY_OR_DUP(&arr, arr_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		sum += zephir_fast_count_int(&arr);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * count() with an explicit COUNT_NORMAL (0) mode argument (#2468). Same
 * result as countOptimizedLoop, but CountOptimizer declines on arity, so
 * this compiles to a runtime ZEPHIR_CALL_FUNCTION. The delta between the
 * two subjects is pure call-dispatch overhead.
 */
PHP_METHOD(Stub_Bench, countUnoptimizedLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_2 = NULL;
	zend_long n, i, sum;
	zval *arr_param = NULL, *n_param = NULL, _0$$3, _1$$3;
	zval arr;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(arr, arr_param)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &arr_param, &n_param);
	ZEPHIR_OBS_COPY_OR_DUP(&arr, arr_param);
	i = 0;
	sum = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZVAL_LONG(&_0$$3, 0);
		ZEPHIR_CALL_FUNCTION(&_1$$3, "count", &_2, 12, &arr, &_0$$3);
		zephir_check_call_status();
		sum += zephir_get_numberval(&_1$$3);
		i++;
	}
	RETURN_MM_LONG(sum);
}

/**
 * implode() with a glue argument (#2468): ImplodeOptimizer inlines
 * zephir_fast_join.
 */
PHP_METHOD(Stub_Bench, implodeOptimizedLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long n, i;
	zval *arr_param = NULL, *n_param = NULL, out;
	zval arr;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&out);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(arr, arr_param)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &arr_param, &n_param);
	ZEPHIR_OBS_COPY_OR_DUP(&arr, arr_param);
	ZEPHIR_INIT_VAR(&out);
	ZVAL_STRING(&out, "");
	i = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&out);
		zephir_fast_join_str(&out, SL(""), &arr);
		i++;
	}
	RETURN_CCTOR(&out);
}

/**
 * implode() without a glue argument (#2468). Produces the same string as
 * implodeOptimizedLoop, but the optimizer requires exactly two parameters,
 * so this falls back to a runtime call.
 */
PHP_METHOD(Stub_Bench, implodeUnoptimizedLoop)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zend_long n, i;
	zval *arr_param = NULL, *n_param = NULL, out;
	zval arr;

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&out);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(arr, arr_param)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &arr_param, &n_param);
	ZEPHIR_OBS_COPY_OR_DUP(&arr, arr_param);
	ZEPHIR_INIT_VAR(&out);
	ZVAL_STRING(&out, "");
	i = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_CALL_FUNCTION(&out, "implode", &_0, 13, &arr);
		zephir_check_call_status();
		i++;
	}
	RETURN_CCTOR(&out);
}

/**
 * acos() over a double-typed local. Reachable optimizer => a bare libm
 * acos() call is emitted inline; unreachable (Linux, before the
 * ACosOptimizer rename) => runtime ZEPHIR_CALL_FUNCTION.
 */
PHP_METHOD(Stub_Bench, acosLoop)
{
	double x, sum;
	zval *n_param = NULL, _0$$3;
	zend_long n, i;

	ZVAL_UNDEF(&_0$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	x =  (0.5);
	sum =  (0.0);
	i = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZVAL_DOUBLE(&_0$$3, x);
		sum +=  (acos(x));
		i++;
	}
	RETURN_DOUBLE(sum);
}

void zep_Stub_Bench_zephir_gen_step_generatorRange(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *zephir_gen, zephir_gen_sub, _0$$3;
	zend_long n, i;
		ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&_0$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 0)) != IS_UNDEF) { n = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 0)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0$$3);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { i = (zend_long) Z_LVAL_P(zephir_generator_slot(zephir_gen, 2)); }
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	i = 1;
	while (1) {
		if (!(i <= n)) {
			break;
		}
		ZEPHIR_INIT_NVAR(&_0$$3);
		ZVAL_LONG(&_0$$3, i);
		zephir_generator_yield(zephir_gen, NULL, &_0$$3, 1);
		zephir_generator_slot_set_long(zephir_gen, 0, (zend_long) n);
		zephir_generator_slot_set(zephir_gen, 1, &_0$$3);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) i);
		ZEPHIR_MM_RESTORE();
		return;
		zephir_yield_resume_1:;
		i = (i + 1);
	}
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

void zep_Stub_Bench_zephir_gen_step_generatorOverArray(int ht, zend_execute_data *execute_data, zval *return_value, zval *this_ptr, int return_value_used, zval *zephir_gen_ext )
{
	HashPosition _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *zephir_gen, zephir_gen_sub, v, _0, *_2;
	zval items;
		ZVAL_UNDEF(&items);
	ZVAL_UNDEF(&zephir_gen_sub);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_gen = zephir_gen_ext;
	/* Generator resume dispatch (issue #1849) */
	zephir_generator_slots_ensure(zephir_gen, 4);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 0, &items);
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 1, &_0);
	if (Z_TYPE_P(zephir_generator_slot(zephir_gen, 2)) != IS_UNDEF) { _1 = (HashPosition) Z_LVAL_P(zephir_generator_slot(zephir_gen, 2)); }
	ZEPHIR_GEN_RESTORE_ZVAL(zephir_gen, 3, &v);
	switch (zephir_generator_get_state(zephir_gen)) {
		case 1: goto zephir_yield_resume_1;
		default: break;
	}
	zephir_is_iterable(&items, 0, "stub/bench.zep", 287);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_COPY(&_0, &items);
	zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(&_0), &_1);
	while (1) {
		_2 = zend_hash_get_current_data_ex(Z_ARRVAL_P(&_0), &_1);
		if (_2 == NULL) {
			break;
		}
		ZEPHIR_INIT_NVAR(&v);
		ZVAL_COPY(&v, _2);
		zend_hash_move_forward_ex(Z_ARRVAL_P(&_0), &_1);
			zephir_generator_yield(zephir_gen, NULL, &v, 1);
			zephir_generator_slot_set(zephir_gen, 0, &items);
		zephir_generator_slot_set(zephir_gen, 1, &_0);
		zephir_generator_slot_set_long(zephir_gen, 2, (zend_long) _1);
		zephir_generator_slot_set(zephir_gen, 3, &v);
			ZEPHIR_MM_RESTORE();
			return;
			zephir_yield_resume_1:;
	}
	ZEPHIR_INIT_NVAR(&v);
	zephir_generator_finish(zephir_gen, NULL);
	ZEPHIR_MM_RESTORE();
}

