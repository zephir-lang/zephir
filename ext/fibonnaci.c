
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Fibonnaci) {

	ZEPHIR_REGISTER_CLASS(Test, Fibonnaci, fibonnaci, test_fibonnaci_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Fibonnaci, fibInt) {

	int x, y, n, z;


	z = 0;
	x = 1;
	y = 2;
	n = 10;
	while (1) {
		if (!(((z < n)))) {
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
		if (!(((z < n)))) {
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
	zval *fib, *a = NULL, *b = NULL, *c = NULL, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(fib);
	array_init(fib);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	zephir_array_append(&fib, _0, 0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_append(&fib, _0, 0);
	i = 2;
	n = 10;
	while (1) {
		if (!(((i < n)))) {
			break;
		}
		k = (i - 1);
		j = (i - 2);
		ZEPHIR_OBS_NVAR(a);
		zephir_array_fetch_long(&a, fib, k, PH_NOISY);
		ZEPHIR_OBS_NVAR(b);
		zephir_array_fetch_long(&b, fib, j, PH_NOISY);
		ZEPHIR_INIT_NVAR(c);
		zephir_add_function(c, a, b TSRMLS_CC);
		zephir_array_update_long(&fib, i, &c, PH_COPY | PH_SEPARATE);
		i++;
	}
	RETURN_CCTOR(fib);

}

