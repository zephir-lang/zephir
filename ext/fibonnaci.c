
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Fibonnaci) {

	ZEPHIR_REGISTER_CLASS(Test, Fibonnaci, fibonnaci, test_fibonnaci_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Fibonnaci, fibArray) {

	long i, n, k, j;
	zval *fib, *a = NULL, *b = NULL, *c, *_0, *_1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(fib);
	array_init(fib);

	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 0);
	zephir_array_append(&fib, _0, 0);

	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_append(&fib, _1, 0);

	i = 2;

	n = 10;

	while ((i < n)) {

		k = i - 1;

		j = i - 2;

		ZEPHIR_OBS_NVAR(a);
		zephir_array_fetch_long(&a, fib, k, PH_NOISY);

		ZEPHIR_OBS_NVAR(b);
		zephir_array_fetch_long(&b, fib, j, PH_NOISY);

		zephir_add_function(c, a, b);

		zephir_array_update_long(&fib, i, &c, PH_COPY | PH_SEPARATE);

		i++;

	}

	RETURN_CCTOR(fib);


}

