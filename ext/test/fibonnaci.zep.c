
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Fibonnaci) {

	ZEPHIR_REGISTER_CLASS(Test, Fibonnaci, test, fibonnaci, test_fibonnaci_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Fibonnaci, fibInt) {

	zend_long x = 0, y = 0, n = 0, z = 0;
	zval *this_ptr = getThis();



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

	double x = 0, y = 0, n = 0, z = 0;
	zval *this_ptr = getThis();



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

	zend_long i = 0, n = 0, k = 0, j = 0;
	zval fib, a, b, c, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&fib);
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&b);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&fib);
	zephir_create_array(&fib, 2, 0 TSRMLS_CC);
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
		zephir_array_fetch_long(&a, &fib, k, PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 51 TSRMLS_CC);
		zephir_array_fetch_long(&b, &fib, j, PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 52 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(&c);
		zephir_add_function(&c, &a, &b);
		zephir_array_update_long(&fib, i, &c, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		i++;
	}
	RETURN_CCTOR(&fib);

}

PHP_METHOD(Test_Fibonnaci, fibArray2) {

	zend_long i = 0, n = 0;
	zval fib, _0, _1$$3, _2$$3, _3$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&fib);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&fib);
	zephir_create_array(&fib, 2, 0 TSRMLS_CC);
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
		zephir_array_fetch_long(&_1$$3, &fib, (i - 1), PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 67 TSRMLS_CC);
		zephir_array_fetch_long(&_2$$3, &fib, (i - 2), PH_NOISY | PH_READONLY, "test/fibonnaci.zep", 67 TSRMLS_CC);
		ZEPHIR_INIT_LNVAR(_3$$3);
		zephir_add_function(&_3$$3, &_1$$3, &_2$$3);
		zephir_array_update_long(&fib, i, &_3$$3, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		i++;
	}
	RETURN_CCTOR(&fib);

}

PHP_METHOD(Test_Fibonnaci, fibonacciRecursive) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *n_param = NULL, _0$$6, _1$$6, _3$$6;
	zend_long n, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$6);
	ZVAL_UNDEF(&_1$$6);
	ZVAL_UNDEF(&_3$$6);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	if (n == 0) {
		RETURN_MM_LONG(0);
	} else {
		if (n == 1) {
			RETURN_MM_LONG(1);
		} else {
			ZVAL_LONG(&_1$$6, (n - 1));
			ZEPHIR_CALL_METHOD(&_0$$6, this_ptr, "fibonaccirecursive", &_2, 39, &_1$$6);
			zephir_check_call_status();
			ZVAL_LONG(&_1$$6, (n - 2));
			ZEPHIR_CALL_METHOD(&_3$$6, this_ptr, "fibonaccirecursive", &_2, 39, &_1$$6);
			zephir_check_call_status();
			zephir_add_function(return_value, &_0$$6, &_3$$6);
			RETURN_MM();
		}
	}

}

PHP_METHOD(Test_Fibonnaci, fibonacciFinalRecursive) {

	zephir_fcall_cache_entry *_2 = NULL;
	zval *n_param = NULL, _0$$6, _1$$6, _3$$6;
	zend_long n, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0$$6);
	ZVAL_UNDEF(&_1$$6);
	ZVAL_UNDEF(&_3$$6);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	if (n == 0) {
		RETURN_MM_LONG(0);
	} else {
		if (n == 1) {
			RETURN_MM_LONG(1);
		} else {
			ZVAL_LONG(&_1$$6, (n - 1));
			ZEPHIR_CALL_METHOD(&_0$$6, this_ptr, "fibonaccifinalrecursive", &_2, 40, &_1$$6);
			zephir_check_call_status();
			ZVAL_LONG(&_1$$6, (n - 2));
			ZEPHIR_CALL_METHOD(&_3$$6, this_ptr, "fibonaccifinalrecursive", &_2, 40, &_1$$6);
			zephir_check_call_status();
			zephir_add_function(return_value, &_0$$6, &_3$$6);
			RETURN_MM();
		}
	}

}

