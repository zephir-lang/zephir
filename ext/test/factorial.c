
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
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Factorial) {

	ZEPHIR_REGISTER_CLASS(Test, Factorial, test, factorial, test_factorial_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Factorial, intFactorial) {

	zend_bool _0;
	long result = 1, i;
	zval *n_param = NULL;
	int n, _1, _2;

	zephir_fetch_params(0, 1, 0, &n_param);

		n = zephir_get_intval(n_param);


	_2 = n;
	_1 = 2;
	_0 = 0;
	if ((_1 <= _2)) {
		while (1) {
			if (_0) {
				_1++;
				if (!((_1 <= _2))) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			result *= i;
		}
	}
	RETURN_LONG(result);

}

