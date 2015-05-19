
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Fibonnaci) {

	ZEPHIR_REGISTER_CLASS(Test, Fibonnaci, test, fibonnaci, test_fibonnaci_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Fibonnaci, fibInt) {

	int x, y, n, z;


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

PHP_METHOD(Test_Fibonnaci, fibDouble) {

	double x, y, n, z;


	z = 0.0;
	x = 1.0;
	y = 2.0;
	n = 10.0;
	while (1) {
		if (!(z < n)) {
			break;
		}
		z = (x + y);
		x = y;
		y = z;
	}
	RETURN_DOUBLE(z);

}

PHP_METHOD(Test_Fibonnaci, fibArray) {

	int i, n, k, j;
	zval *fib, *a, *b, *c = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(fib);
	zephir_create_array(fib, 2, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	zephir_array_fast_append(fib, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(fib, _0);
	i = 2;
	n = 10;
	while (1) {
		if (!(i < n)) {
			break;
		}
		k = (i - 1);
		j = (i - 2);
		zephir_array_fetch_long(&a, fib, k, PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 51 TSRMLS_CC);
		zephir_array_fetch_long(&b, fib, j, PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 52 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(c);
		zephir_add_function_ex(c, a, b TSRMLS_CC);
		zephir_array_update_long(&fib, i, &c, PH_COPY | PH_SEPARATE, "test/fibonnaci.zep", 54);
		i++;
	}
	RETURN_CCTOR(fib);

}

PHP_METHOD(Test_Fibonnaci, fibArray2) {

	int i, n;
	zval *fib, *_0 = NULL, *_1, *_2, *_3 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(fib);
	zephir_create_array(fib, 2, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	zephir_array_fast_append(fib, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(fib, _0);
	i = 2;
	n = 10;
	while (1) {
		if (!(i < n)) {
			break;
		}
		zephir_array_fetch_long(&_1, fib, (i - 1), PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 67 TSRMLS_CC);
		zephir_array_fetch_long(&_2, fib, (i - 2), PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 67 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_3);
		zephir_add_function_ex(_3, _1, _2 TSRMLS_CC);
		zephir_array_update_long(&fib, i, &_3, PH_COPY | PH_SEPARATE, "test/fibonnaci.zep", 67);
		i++;
	}
	RETURN_CCTOR(fib);

}

PHP_METHOD(Test_Fibonnaci, fibonacciRecursive) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *n_param = NULL, *_0 = NULL, _1 = zval_used_for_init, *_3 = NULL;
	int n, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	if (n == 0) {
		RETURN_MM_LONG(0);
	} else {
		if (n == 1) {
			RETURN_MM_LONG(1);
		} else {
			ZEPHIR_SINIT_VAR(_1);
			ZVAL_LONG(&_1, (n - 1));
			ZEPHIR_CALL_METHOD(&_0, this_ptr, "fibonaccirecursive", &_2, 39, &_1);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, (n - 2));
			ZEPHIR_CALL_METHOD(&_3, this_ptr, "fibonaccirecursive", &_2, 39, &_1);
			zephir_check_call_status();
			zephir_add_function_ex(return_value, _0, _3 TSRMLS_CC);
			RETURN_MM();
		}
	}

}

PHP_METHOD(Test_Fibonnaci, fibonacciFinalRecursive) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *n_param = NULL, *_0 = NULL, _1 = zval_used_for_init, *_3 = NULL;
	int n, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	if (n == 0) {
		RETURN_MM_LONG(0);
	} else {
		if (n == 1) {
			RETURN_MM_LONG(1);
		} else {
			ZEPHIR_SINIT_VAR(_1);
			ZVAL_LONG(&_1, (n - 1));
			ZEPHIR_CALL_METHOD(&_0, this_ptr, "fibonaccifinalrecursive", &_2, 40, &_1);
			zephir_check_call_status();
			ZEPHIR_SINIT_NVAR(_1);
			ZVAL_LONG(&_1, (n - 2));
			ZEPHIR_CALL_METHOD(&_3, this_ptr, "fibonaccifinalrecursive", &_2, 40, &_1);
			zephir_check_call_status();
			zephir_add_function_ex(return_value, _0, _3 TSRMLS_CC);
			RETURN_MM();
		}
	}

}

