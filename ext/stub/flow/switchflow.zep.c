
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Flow_SwitchFlow)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Flow, SwitchFlow, stub, flow_switchflow, stub_flow_switchflow_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch1)
{
	zend_long a = 0;
	a = 10;
	RETURN_LONG(a);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch2)
{
	zend_long a = 0;
	a = 10;
	if (a == 10) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
		RETURN_BOOL(1);
	zephir_switch_0_end: ;

	RETURN_BOOL(0);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch3)
{
	zend_long a = 0;
	a = 10;
	if (a == 5) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
		RETURN_BOOL(1);
	zephir_switch_0_end: ;

	RETURN_BOOL(0);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch4)
{
	zend_long a = 0;
	a = 10;
	if (a == 5) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		RETURN_LONG(1);
	zephir_switch_0_clause_1: ;
		RETURN_LONG(2);

	RETURN_LONG(3);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch5)
{
	zend_long a = 0;
	a = 10;
	if (a == 10) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		RETURN_LONG(1);
	zephir_switch_0_clause_1: ;
		RETURN_LONG(2);

	RETURN_LONG(3);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch6)
{
	zend_long a = 0;
	a = 10;
	if (a == 10) { goto zephir_switch_0_clause_0; }
	if (a == 4) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		RETURN_LONG(1);
	zephir_switch_0_end: ;

	RETURN_LONG(2);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch7)
{
	zend_long a = 0;
	a = 4;
	if (a == 10) { goto zephir_switch_0_clause_0; }
	if (a == 4) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		RETURN_LONG(1);
	zephir_switch_0_end: ;

	RETURN_LONG(2);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch8)
{
	zend_long a = 0;
	a = 5;
	if (a == 10) { goto zephir_switch_0_clause_0; }
	if (a == 4) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		RETURN_LONG(1);
	zephir_switch_0_end: ;

	RETURN_LONG(2);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch9)
{
	zend_long a = 0;
	a = 5;
	if (a == 3) { goto zephir_switch_0_clause_0; }
	if (a == 4) { goto zephir_switch_0_clause_1; }
	if (a == 10) { goto zephir_switch_0_clause_2; }
	if (a == 4) { goto zephir_switch_0_clause_3; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		RETURN_LONG(0);
	zephir_switch_0_clause_2: ;
	zephir_switch_0_clause_3: ;
		RETURN_LONG(1);
	zephir_switch_0_end: ;

	RETURN_LONG(2);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch10)
{
	zend_long a = 0;
	a = 5;
	if (a == 3) { goto zephir_switch_0_clause_0; }
	if (a == 4) { goto zephir_switch_0_clause_1; }
	if (a == 10) { goto zephir_switch_0_clause_2; }
	if (a == 4) { goto zephir_switch_0_clause_3; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		RETURN_LONG(0);
	zephir_switch_0_clause_2: ;
	zephir_switch_0_clause_3: ;
		RETURN_LONG(1);
	zephir_switch_0_end: ;

	RETURN_LONG(2);
}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch11)
{
	zend_long a = 0;
	a = 1;
	if (a == 1) { goto zephir_switch_0_clause_0; }
	if (a == 2) { goto zephir_switch_0_clause_2; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		RETURN_LONG(1);
	zephir_switch_0_clause_2: ;
		RETURN_LONG(2);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch12)
{
	zend_bool _0, _1, _2;
	zval *var1, var1_sub, *var2, var2_sub;

	ZVAL_UNDEF(&var1_sub);
	ZVAL_UNDEF(&var2_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(var1)
		Z_PARAM_ZVAL(var2)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &var1, &var2);
	_0 = 1;
	_1 = ZEPHIR_GT(var1, var2);
	if (_1) {
		_1 = ZEPHIR_GT_LONG(var2, 5);
	}
	if (_0 == _1) { goto zephir_switch_0_clause_0; }
	_2 = ZEPHIR_LT(var1, var2);
	if (_2) {
		_2 = ZEPHIR_LT_LONG(var1, 5);
	}
	if (_0 == _2) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_clause_2;
	zephir_switch_0_clause_0: ;
		RETURN_LONG(1);
	zephir_switch_0_clause_1: ;
		RETURN_LONG(2);
	zephir_switch_0_clause_2: ;
		RETURN_LONG(0);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch13)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a_param = NULL, _0, _1, _2;
	zend_long a;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZVAL_LONG(&_0, 1);
	ZVAL_LONG(&_1, 2);
	ZEPHIR_CALL_FUNCTION(&_2, "mt_rand", NULL, 44, &_0, &_1);
	zephir_check_call_status();
	if (ZEPHIR_IS_LONG(&_2, 100)) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		RETURN_MM_LONG(1);
	zephir_switch_0_clause_1: ;
		RETURN_MM_LONG(0);

}

PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch14)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *result_type = NULL, result_type_sub, ret, _0;

	ZVAL_UNDEF(&result_type_sub);
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL(result_type)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 0, 1, &result_type);
	if (!result_type) {
		result_type = &result_type_sub;
		ZEPHIR_INIT_VAR(result_type);
		ZVAL_LONG(result_type, 1);
	}
	if (ZEPHIR_IS_LONG(result_type, 1)) { goto zephir_switch_0_clause_0; }
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_GET_CONSTANT(&_0, "MYSQL_ASSOC");
	if (ZEPHIR_IS_EQUAL(result_type, &_0)) { goto zephir_switch_0_clause_1; }
	if (ZEPHIR_IS_LONG(result_type, 1)) { goto zephir_switch_0_clause_2; }
	goto zephir_switch_0_clause_3;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
	zephir_switch_0_clause_2: ;
		ZEPHIR_INIT_VAR(&ret);
		ZVAL_STRING(&ret, "aaa");
		goto zephir_switch_0_end;
	zephir_switch_0_clause_3: ;
		ZEPHIR_INIT_NVAR(&ret);
		ZVAL_STRING(&ret, "bbb");
		goto zephir_switch_0_end;
	zephir_switch_0_end: ;

	RETURN_CCTOR(&ret);
}

/**
 * Issue #1704: PHP fall-through. A clause without `break` runs on into the
 * next clause's body, and on into `default`.
 */
PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch15)
{
	zval r;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL;
	zend_long a;

	ZVAL_UNDEF(&r);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZEPHIR_INIT_VAR(&r);
	ZVAL_STRING(&r, "");
	if (a == 1) { goto zephir_switch_0_clause_0; }
	if (a == 2) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_clause_2;
	zephir_switch_0_clause_0: ;
		zephir_concat_self_str(&r, "a", sizeof("a") - 1);
	zephir_switch_0_clause_1: ;
		zephir_concat_self_str(&r, "b", sizeof("b") - 1);
	zephir_switch_0_clause_2: ;
		zephir_concat_self_str(&r, "d", sizeof("d") - 1);

	RETURN_CTOR(&r);
}

/**
 * Issue #1704: a `default` clause in the middle falls through into the
 * `case` written after it.
 */
PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch16)
{
	zval r;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL;
	zend_long a;

	ZVAL_UNDEF(&r);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZEPHIR_INIT_VAR(&r);
	ZVAL_STRING(&r, "");
	if (a == 1) { goto zephir_switch_0_clause_0; }
	if (a == 2) { goto zephir_switch_0_clause_2; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		zephir_concat_self_str(&r, "one", sizeof("one") - 1);
		goto zephir_switch_0_end;
	zephir_switch_0_clause_1: ;
		zephir_concat_self_str(&r, "def", sizeof("def") - 1);
	zephir_switch_0_clause_2: ;
		zephir_concat_self_str(&r, "two", sizeof("two") - 1);
	zephir_switch_0_end: ;

	RETURN_CTOR(&r);
}

/**
 * Issue #1704: `continue` inside a `switch` is equivalent to `break`, as
 * in PHP - the loop body after the `switch` still runs.
 */
PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch17)
{
	zend_long _1, _2;
	zend_bool _0;
	zval i;
	zval r, _3$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&r);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&i);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&r);
	ZVAL_STRING(&r, "");
	_2 = 3;
	_1 = 0;
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
			ZEPHIR_INIT_NVAR(&i);
			ZVAL_LONG(&i, _1);
			if (ZEPHIR_IS_LONG(&i, 1)) { goto zephir_switch_0_clause_0; }
			goto zephir_switch_0_clause_1;
			zephir_switch_0_clause_0: ;
				goto zephir_switch_0_end;
			zephir_switch_0_clause_1: ;
				goto zephir_switch_0_end;
			zephir_switch_0_end: ;

			zephir_cast_to_string(&_3$$3, &i);
			zephir_concat_self(&r, &_3$$3);
		}
	}
	RETURN_CTOR(&r);
}

/**
 * Issue #1704: a `break` inside a loop nested in a clause breaks the loop,
 * not the `switch`.
 */
PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch18)
{
	zval _1$$3;
	zend_long n, _0;
	zval r, _2$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&r);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_1$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&r);
	ZVAL_STRING(&r, "");
	n = 0;
	_0 = 1;
	if (_0 == 1) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_end;
	zephir_switch_0_clause_0: ;
		while (1) {
			if (!(1)) {
				break;
			}
			n++;
			if (n > 2) {
				break;
			}
		}
		zephir_concat_self_str(&r, "w", sizeof("w") - 1);
		ZEPHIR_INIT_VAR(&_1$$3);
		ZVAL_LONG(&_1$$3, n);
		zephir_cast_to_string(&_2$$3, &_1$$3);
		zephir_concat_self(&r, &_2$$3);
	zephir_switch_0_end: ;

	RETURN_CTOR(&r);
}

/**
 * Issue #1704: an empty clause falls through into the next body, which
 * then falls through into `default`.
 */
PHP_METHOD(Stub_Flow_SwitchFlow, testSwitch19)
{
	zval r;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL;
	zend_long a;

	ZVAL_UNDEF(&r);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZEPHIR_INIT_VAR(&r);
	ZVAL_STRING(&r, "");
	if (a == 1) { goto zephir_switch_0_clause_0; }
	if (a == 2) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_clause_2;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		zephir_concat_self_str(&r, "x", sizeof("x") - 1);
	zephir_switch_0_clause_2: ;
		zephir_concat_self_str(&r, "y", sizeof("y") - 1);

	RETURN_CTOR(&r);
}

