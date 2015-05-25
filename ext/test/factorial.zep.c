
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
			result *= i;
		}
	}
	RETURN_LONG(result);

}

PHP_METHOD(Test_Factorial, intRecursiveFactorial) {

	zend_bool _1;
	zval *num_param = NULL, *_0, *_2 = NULL, _3;
	int num, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	ZEPHIR_INIT_VAR(_0);
	_1 = num == 0;
	if (!(_1)) {
		_1 = num == 1;
	}
	if (_1) {
		ZVAL_LONG(_0, 1);
	} else {
		ZEPHIR_SINIT_VAR(_3);
		ZVAL_LONG(&_3, (num - 1));
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "intrecursivefactorial", NULL, 29, &_3);
		zephir_check_call_status();
		ZVAL_LONG(_0, (num * zephir_get_numberval(_2)));
	}
	RETURN_CCTOR(_0);

}

