
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
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Factorial) {

	ZEPHIR_REGISTER_CLASS(Test, Factorial, test, factorial, test_factorial_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Factorial, intIterativeFactorial) {

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

PHP_METHOD(Test_Factorial, intRecursiveFactorial) {

	zend_bool _0;
	zval *num_param = NULL, *_1 = NULL, _2;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	_0 = (num == 0);
	if (!(_0)) {
		_0 = (num == 1);
	}
	if (_0) {
		ZVAL_LONG(return_value, 1);
	} else {
		ZEPHIR_SINIT_VAR(_2);
		ZVAL_LONG(&_2, (num - 1));
		ZEPHIR_CALL_METHOD(&_1, this_ptr, "intrecursivefactorial", NULL, &_2);
		zephir_check_call_status();
		ZVAL_LONG(return_value, (num * zephir_get_numberval(_1)));
	}
	RETURN_MM();

}

