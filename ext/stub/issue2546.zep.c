
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
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/object.h"


/**
 * Regression coverage for https://github.com/zephir-lang/zephir/issues/2546
 *
 * `continue` inside a `for ... in` loop over a PHP Iterator/Traversable was
 * not calling next() on the iterator. The generated while(1) placed next()
 * after the user body, so C `continue` jumped past it, producing an infinite
 * loop on any element that triggered the continue.
 */
ZEPHIR_INIT_CLASS(Stub_Issue2546)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2546, stub, issue2546, stub_issue2546_method_entry, 0);

	return SUCCESS;
}

/**
 * Collect items from a Traversable, skipping any equal to `skip`.
 * `continue` must advance the iterator; without the fix it loops forever.
 */
PHP_METHOD(Stub_Issue2546, collectExcluding)
{
	zend_bool _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *items, items_sub, *skip, skip_sub, item, result, *_0, _1, *_2, _3;

	ZVAL_UNDEF(&items_sub);
	ZVAL_UNDEF(&skip_sub);
	ZVAL_UNDEF(&item);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(items)
		Z_PARAM_ZVAL(skip)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &items, &skip);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	if (Z_TYPE_P(items) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, items);
		_0 = &_1;
	} else {
		_0 = items;
	}
	zephir_is_iterable(_0, 0, "stub/issue2546.zep", 29);
	if (Z_TYPE_P(_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_0), _2)
		{
			ZEPHIR_INIT_NVAR(&item);
			ZVAL_COPY(&item, _2);
			if (ZEPHIR_IS_IDENTICAL(&item, skip)) {
				continue;
			}
			zephir_array_append(&result, &item, PH_SEPARATE, "stub/issue2546.zep", 26);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _0, "rewind", NULL, 0);
		zephir_check_call_status();
		_4 = 1;
		while (1) {
			if (_4) {
				_4 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_3, _0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&item, _0, "current", NULL, 0);
			zephir_check_call_status();
				if (ZEPHIR_IS_IDENTICAL(&item, skip)) {
					continue;
				}
				zephir_array_append(&result, &item, PH_SEPARATE, "stub/issue2546.zep", 26);
		}
	}
	ZEPHIR_INIT_NVAR(&item);
	RETURN_CCTOR(&result);
}

/**
 * Collect items until `stop` is encountered; verifies `break` still works.
 */
PHP_METHOD(Stub_Issue2546, collectUntil)
{
	zend_bool _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *items, items_sub, *stop, stop_sub, item, result, *_0, _1, *_2, _3;

	ZVAL_UNDEF(&items_sub);
	ZVAL_UNDEF(&stop_sub);
	ZVAL_UNDEF(&item);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(items)
		Z_PARAM_ZVAL(stop)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &items, &stop);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	if (Z_TYPE_P(items) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, items);
		_0 = &_1;
	} else {
		_0 = items;
	}
	zephir_is_iterable(_0, 0, "stub/issue2546.zep", 47);
	if (Z_TYPE_P(_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_0), _2)
		{
			ZEPHIR_INIT_NVAR(&item);
			ZVAL_COPY(&item, _2);
			if (ZEPHIR_IS_IDENTICAL(&item, stop)) {
				break;
			}
			zephir_array_append(&result, &item, PH_SEPARATE, "stub/issue2546.zep", 44);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _0, "rewind", NULL, 0);
		zephir_check_call_status();
		_4 = 1;
		while (1) {
			if (_4) {
				_4 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_3, _0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&item, _0, "current", NULL, 0);
			zephir_check_call_status();
				if (ZEPHIR_IS_IDENTICAL(&item, stop)) {
					break;
				}
				zephir_array_append(&result, &item, PH_SEPARATE, "stub/issue2546.zep", 44);
		}
	}
	ZEPHIR_INIT_NVAR(&item);
	RETURN_CCTOR(&result);
}

/**
 * Collect key-value pairs, skipping those whose key equals `skipKey`.
 * Exercises the key+value path with continue.
 */
PHP_METHOD(Stub_Issue2546, collectKeysExcluding)
{
	zend_bool _6;
	zend_string *_4;
	zend_ulong _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *items, items_sub, *skipKey, skipKey_sub, k, v, result, *_0, _1, *_2, _5;

	ZVAL_UNDEF(&items_sub);
	ZVAL_UNDEF(&skipKey_sub);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_5);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(items)
		Z_PARAM_ZVAL(skipKey)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &items, &skipKey);
	ZEPHIR_INIT_VAR(&result);
	array_init(&result);
	if (Z_TYPE_P(items) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, items);
		_0 = &_1;
	} else {
		_0 = items;
	}
	zephir_is_iterable(_0, 0, "stub/issue2546.zep", 66);
	if (Z_TYPE_P(_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_0), _3, _4, _2)
		{
			ZEPHIR_INIT_NVAR(&k);
			if (_4 != NULL) { 
				ZVAL_STR_COPY(&k, _4);
			} else {
				ZVAL_LONG(&k, _3);
			}
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _2);
			if (ZEPHIR_IS_IDENTICAL(&k, skipKey)) {
				continue;
			}
			zephir_array_update_zval(&result, &k, &v, PH_COPY | PH_SEPARATE);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _0, "rewind", NULL, 0);
		zephir_check_call_status();
		_6 = 1;
		while (1) {
			if (_6) {
				_6 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_5, _0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_5)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&k, _0, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&v, _0, "current", NULL, 0);
			zephir_check_call_status();
				if (ZEPHIR_IS_IDENTICAL(&k, skipKey)) {
					continue;
				}
				zephir_array_update_zval(&result, &k, &v, PH_COPY | PH_SEPARATE);
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	ZEPHIR_INIT_NVAR(&k);
	RETURN_CCTOR(&result);
}

