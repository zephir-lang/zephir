
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"


/**
 * Range operators (`..` inclusive, `...` exclusive) and range loops.
 */
ZEPHIR_INIT_CLASS(Stub_Range)
{
	ZEPHIR_REGISTER_CLASS(Stub, Range, stub, range, stub_range_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Range, inclusive1)
{
	zval _3;
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 10);
	ZEPHIR_CALL_FUNCTION(&_2, "range", NULL, 93, &_0, &_1);
	zephir_check_call_status();
	zephir_get_arrval(&_3, &_2);
	RETURN_CTOR(&_3);
}

PHP_METHOD(Stub_Range, exclusive1)
{
	zval _3;
	zval _0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZVAL_LONG(&_0, 0);
	ZVAL_LONG(&_1, 10);
	ZEPHIR_CALL_FUNCTION(&_2, "range", NULL, 93, &_0, &_1);
	zephir_check_call_status();
	zephir_get_arrval(&_3, &_2);
	RETURN_CTOR(&_3);
}

/**
 * `for i in a..b` with a typed int loop variable: compiles to an integer
 * counting loop (see #2433). Before the fix this form failed to compile.
 */
PHP_METHOD(Stub_Range, loopInclusive)
{
	zval _3$$3;
	zend_bool _0;
	zend_long i, _1, _2;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_3$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	i = 0;
	_2 = 5;
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
			i = _1;
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_array_append(&result, &_3$$3, PH_SEPARATE, "stub/range.zep", 29);
		}
	}
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_Range, loopExclusive)
{
	zval _3$$3;
	zend_bool _0;
	zend_long i, _1, _2;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_3$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	i = 0;
	_2 = 5;
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
			i = _1;
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_array_append(&result, &_3$$3, PH_SEPARATE, "stub/range.zep", 39);
		}
	}
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_Range, loopReverse)
{
	zval _3$$3;
	zend_bool _0;
	zend_long i, _1, _2;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_3$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	i = 0;
	_2 = 5;
	_1 = _2;
	_0 = 0;
	if (_1 >= 0) {
		while (1) {
			if (_0) {
				_1--;
				if (!(_1 >= 0)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_array_append(&result, &_3$$3, PH_SEPARATE, "stub/range.zep", 49);
		}
	}
	RETURN_CTOR(&result);
}

PHP_METHOD(Stub_Range, loopVarBounds)
{
	zend_bool _0;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *b_param = NULL, _3$$3;
	zend_long a, b, i, _1, _2;

	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&result);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &b_param);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	i = 0;
	_2 = b;
	_1 = a;
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
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_array_append(&result, &_3$$3, PH_SEPARATE, "stub/range.zep", 59);
		}
	}
	RETURN_CTOR(&result);
}

/**
 * Equivalent loop written with an explicit range() call. Used to prove the
 * `..` operator produces identical output to range().
 */
PHP_METHOD(Stub_Range, loopRangeFn)
{
	zval _3$$3;
	zend_bool _0;
	zend_long i, _1, _2;
	zval result;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_3$$3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	i = 0;
	_2 = 5;
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
			i = _1;
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, i);
			zephir_array_append(&result, &_3$$3, PH_SEPARATE, "stub/range.zep", 73);
		}
	}
	RETURN_CTOR(&result);
}

/**
 * Char range: bounds are not int-compatible, so compileRange() bails and the
 * loop falls back to the generic array path. Must still work.
 */
PHP_METHOD(Stub_Range, loopChars)
{
	zend_bool _6;
	zval _3;
	zval ch, _0, _1, _2, *_4, _5;
	zval out;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&ch);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_3);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&out);
	ZVAL_STRING(&out, "");
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "a");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "e");
	ZEPHIR_CALL_FUNCTION(&_2, "range", NULL, 93, &_0, &_1);
	zephir_check_call_status();
	zephir_get_arrval(&_3, &_2);
	zephir_is_iterable(&_3, 0, "stub/range.zep", 89);
	if (Z_TYPE_P(&_3) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&_3), _4)
		{
			ZEPHIR_INIT_NVAR(&ch);
			ZVAL_COPY(&ch, _4);
			zephir_concat_self(&out, &ch);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_3, "rewind", NULL, 0);
		zephir_check_call_status();
		_6 = 1;
		while (1) {
			if (_6) {
				_6 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, &_3, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_5, &_3, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_5)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&ch, &_3, "current", NULL, 0);
			zephir_check_call_status();
				zephir_concat_self(&out, &ch);
		}
	}
	ZEPHIR_INIT_NVAR(&ch);
	RETURN_CTOR(&out);
}

