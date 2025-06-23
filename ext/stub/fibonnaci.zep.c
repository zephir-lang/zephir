
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Fibonnaci)
{
	ZEPHIR_REGISTER_CLASS(Stub, Fibonnaci, stub, fibonnaci, stub_fibonnaci_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Fibonnaci, fibInt)
{
	zend_long x = 0, y = 0, n = 0, z = 0;
	z = 0;
	x = 1;
	y = 2;
	n = 10;
	while (1) {
		if (!(z < n)) {
			break;
		}
		z = (x + y);
		x = y;
		y = z;
	}
	RETURN_LONG(z);
}

PHP_METHOD(Stub_Fibonnaci, fibDouble)
{
	double x = 0, y = 0, n = 0, z = 0;
	z =  (0.0);
	x =  (1.0);
	y =  (2.0);
	n =  (10.0);
	while (1) {
		if (!(z < n)) {
			break;
		}
		z =  ((x + y));
		x =  y;
		y =  z;
	}
	RETURN_DOUBLE(z);
}

PHP_METHOD(Stub_Fibonnaci, fibArray)
{
	zend_long i = 0, n = 0, k = 0, j = 0;
	zval fib, a, b, c, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&fib);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&fib);
	zephir_create_array(&fib, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(&fib, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&fib, &_0);
	i = 2;
	n = 10;
	while (1) {
		if (!(i < n)) {
			break;
		}
		k = (i - 1);
		j = (i - 2);
		ZEPHIR_OBS_NVAR(&a);
		zephir_array_fetch_long(&a, &fib, k, PH_NOISY, "stub/fibonnaci.zep", 51);
		ZEPHIR_OBS_NVAR(&b);
		zephir_array_fetch_long(&b, &fib, j, PH_NOISY, "stub/fibonnaci.zep", 52);
		ZEPHIR_INIT_NVAR(&c);
		zephir_add_function(&c, &a, &b);
		zephir_array_update_long(&fib, i, &c, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		i++;
	}
	RETURN_CCTOR(&fib);
}

PHP_METHOD(Stub_Fibonnaci, fibArray2)
{
	zend_long i = 0, n = 0;
	zval fib, _0, _1$$3, _2$$3, _3$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&fib);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&fib);
	zephir_create_array(&fib, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 0);
	zephir_array_fast_append(&fib, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, 1);
	zephir_array_fast_append(&fib, &_0);
	i = 2;
	n = 10;
	while (1) {
		if (!(i < n)) {
			break;
		}
		zephir_array_fetch_long(&_1$$3, &fib, (i - 1), PH_NOISY | PH_READONLY, "stub/fibonnaci.zep", 67);
		zephir_array_fetch_long(&_2$$3, &fib, (i - 2), PH_NOISY | PH_READONLY, "stub/fibonnaci.zep", 67);
		ZEPHIR_INIT_NVAR(&_3$$3);
		zephir_add_function(&_3$$3, &_1$$3, &_2$$3);
		zephir_array_update_long(&fib, i, &_3$$3, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		i++;
	}
	RETURN_CCTOR(&fib);
}

PHP_METHOD(Stub_Fibonnaci, fibonacciRecursive)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_2 = NULL;
	zval *n_param = NULL, _0$$6, _1$$6, _3$$6;
	zend_long n, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$6);
	ZVAL_UNDEF(&_1$$6);
	ZVAL_UNDEF(&_3$$6);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	if (n == 0) {
		RETURN_MM_LONG(0);
	} else {
		if (n == 1) {
			RETURN_MM_LONG(1);
		} else {
			ZVAL_LONG(&_1$$6, (n - 1));
			ZEPHIR_CALL_METHOD(&_0$$6, this_ptr, "fibonaccirecursive", &_2, 42, &_1$$6);
			zephir_check_call_status();
			ZVAL_LONG(&_1$$6, (n - 2));
			ZEPHIR_CALL_METHOD(&_3$$6, this_ptr, "fibonaccirecursive", &_2, 42, &_1$$6);
			zephir_check_call_status();
			zephir_add_function(return_value, &_0$$6, &_3$$6);
			RETURN_MM();
		}
	}
}

PHP_METHOD(Stub_Fibonnaci, fibonacciFinalRecursive)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_2 = NULL;
	zval *n_param = NULL, _0$$6, _1$$6, _3$$6;
	zend_long n, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$6);
	ZVAL_UNDEF(&_1$$6);
	ZVAL_UNDEF(&_3$$6);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	if (n == 0) {
		RETURN_MM_LONG(0);
	} else {
		if (n == 1) {
			RETURN_MM_LONG(1);
		} else {
			ZVAL_LONG(&_1$$6, (n - 1));
			ZEPHIR_CALL_METHOD(&_0$$6, this_ptr, "fibonaccifinalrecursive", &_2, 43, &_1$$6);
			zephir_check_call_status();
			ZVAL_LONG(&_1$$6, (n - 2));
			ZEPHIR_CALL_METHOD(&_3$$6, this_ptr, "fibonaccifinalrecursive", &_2, 43, &_1$$6);
			zephir_check_call_status();
			zephir_add_function(return_value, &_0$$6, &_3$$6);
			RETURN_MM();
		}
	}
}

