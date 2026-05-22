
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
#include "kernel/concat.h"


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
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, 42);
	zephir_update_property_zval(this_ptr, ZEND_STRL("propA"), &_0);
	ZEPHIR_INIT_VAR(&_1);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "hello");
	zephir_update_property_zval(this_ptr, ZEND_STRL("propB"), &_1);
	ZEPHIR_MM_RESTORE();
}

/**
 * For-over-array baseline. The compiler skips the IS_ARRAY branch wrapper
 * since `arr` is known to be array (see issue #1878).
 */
PHP_METHOD(Stub_Bench, forOverArray)
{
	long sum;
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
		while (1) {
			ZEPHIR_CALL_METHOD(&_1, &arr, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_1)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&v, &arr, "current", NULL, 0);
			zephir_check_call_status();
				sum += zephir_get_intval(&v);
			ZEPHIR_CALL_METHOD(NULL, &arr, "next", NULL, 0);
			zephir_check_call_status();
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
	zval _0;
	long sum;
	zval v, iter, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&iter);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
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
	zephir_is_iterable(&iter, 0, "stub/bench.zep", 48);
	ZEPHIR_CALL_METHOD(NULL, &iter, "rewind", NULL, 0);
	zephir_check_call_status();
	while (1) {
		ZEPHIR_CALL_METHOD(&_2, &iter, "valid", NULL, 0);
		zephir_check_call_status();
		if (!zend_is_true(&_2)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&v, &iter, "current", NULL, 0);
		zephir_check_call_status();
			sum += zephir_get_intval(&v);
		ZEPHIR_CALL_METHOD(NULL, &iter, "next", NULL, 0);
		zephir_check_call_status();
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
	long n, i, sum;
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
		ZEPHIR_OBS_NVAR(&_0$$3);
		zephir_read_property(&_0$$3, this_ptr, ZEND_STRL("propA"), PH_NOISY_CC);
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
	long n, i, sum;
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
		zephir_array_fetch_string(&_0$$3, &arr, SL("a"), PH_NOISY, "stub/bench.zep", 72);
		ZEPHIR_OBS_NVAR(&_1$$3);
		zephir_array_fetch_string(&_1$$3, &arr, SL("b"), PH_NOISY, "stub/bench.zep", 72);
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
	long n, i, sum;
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
	long n, i;
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
	long n, i, sum;

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

