
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
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Exercises every cached call-dispatch path for the #1510 safety review:
 * polymorphic this-> (slot 0 + local cache), final/private/self-recursive
 * (global slot candidates). Not a perf bench — a correctness/valgrind probe.
 */
ZEPHIR_INIT_CLASS(Stub_FcallSafetyBase)
{
	ZEPHIR_REGISTER_CLASS(Stub, FcallSafetyBase, stub, fcallsafetybase, stub_fcallsafetybase_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_FcallSafetyBase, poly)
{

	RETURN_STRING("base");
}

PHP_METHOD(Stub_FcallSafetyBase, fin)
{

	RETURN_LONG(1);
}

PHP_METHOD(Stub_FcallSafetyBase, priv)
{

	RETURN_LONG(2);
}

PHP_METHOD(Stub_FcallSafetyBase, callPriv)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "priv", NULL, 1);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_FcallSafetyBase, stat)
{

	RETURN_LONG(3);
}

PHP_METHOD(Stub_FcallSafetyBase, drive)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_0 = NULL, *_2 = NULL, *_4 = NULL, *_6 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, out, _1$$3, _3$$3, _5$$3;
	long n, i;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_5$$3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	ZEPHIR_INIT_VAR(&out);
	ZVAL_STRING(&out, "");
	i = 0;
	while (1) {
		if (!(i < n)) {
			break;
		}
		ZEPHIR_CALL_METHOD(&out, this_ptr, "poly", &_0, 0);
		zephir_check_call_status();
		ZEPHIR_CALL_METHOD(&_1$$3, this_ptr, "fin", &_2, 2);
		zephir_check_call_status();
		i += zephir_get_numberval(&_1$$3);
		ZEPHIR_CALL_METHOD(&_3$$3, this_ptr, "callpriv", &_4, 0);
		zephir_check_call_status();
		i += zephir_get_numberval(&_3$$3);
		ZEPHIR_CALL_SELF(&_5$$3, "stat", &_6, 0);
		zephir_check_call_status();
		i += zephir_get_numberval(&_5$$3);
		i++;
	}
	RETURN_CCTOR(&out);
}

PHP_METHOD(Stub_FcallSafetyBase, fib)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_2 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n_param = NULL, _0, _1, _3;
	long n;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	if (n < 2) {
		RETURN_MM_LONG(n);
	}
	ZVAL_LONG(&_1, (n - 1));
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "fib", &_2, 3, &_1);
	zephir_check_call_status();
	ZVAL_LONG(&_1, (n - 2));
	ZEPHIR_CALL_METHOD(&_3, this_ptr, "fib", &_2, 3, &_1);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_3);
	RETURN_MM();
}

