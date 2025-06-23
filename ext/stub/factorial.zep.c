
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
#include "kernel/object.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Factorial)
{
	ZEPHIR_REGISTER_CLASS(Stub, Factorial, stub, factorial, stub_factorial_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Factorial, intIterativeFactorial)
{
	zend_bool _0;
	long result, i = 0;
	zval *n_param = NULL;
	zend_long n, _1, _2;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &n_param);
	result = 1;
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

PHP_METHOD(Stub_Factorial, intRecursiveFactorial)
{
	zend_bool _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *num_param = NULL, _0, _2, _3;
	zend_long num, ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &num_param);
	ZEPHIR_INIT_VAR(&_0);
	_1 = num == 0;
	if (!(_1)) {
		_1 = num == 1;
	}
	if (_1) {
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_LONG(&_0, 1);
	} else {
		ZVAL_LONG(&_3, (num - 1));
		ZEPHIR_CALL_METHOD(&_2, this_ptr, "intrecursivefactorial", NULL, 33, &_3);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_LONG(&_0, (num * zephir_get_numberval(&_2)));
	}
	RETURN_CCTOR(&_0);
}

