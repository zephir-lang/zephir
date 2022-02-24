
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/object.h"


/**
 * Stub\Fannkuch
 *
 * Fannkuch Redux in Zephir
 *
 * @see https://disciple-devel.blogspot.mx/2010/11/shootout-fannkuch-redux.html
 */
ZEPHIR_INIT_CLASS(Stub_Fannkuch)
{
	ZEPHIR_REGISTER_CLASS(Stub, Fannkuch, stub, fannkuch, stub_fannkuch_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Fannkuch, process)
{
	zend_bool _0, _5$$4, _11$$8;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, perm, perm0, perm1, count, temp, _3$$3, _4$$6, _9$$4, _8$$7, _10$$8, _14$$9, _15$$14, _16$$15, _17$$13, _18$$13, _19$$13;
	zend_long n, maxFlipsCount, permCount, checksum, i = 0, j = 0, flipsCount = 0, k = 0, r = 0, k2 = 0, _1, _2, _6$$4, _7$$4, _12$$8, _13$$8;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&perm);
	ZVAL_UNDEF(&perm0);
	ZVAL_UNDEF(&perm1);
	ZVAL_UNDEF(&count);
	ZVAL_UNDEF(&temp);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$6);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_8$$7);
	ZVAL_UNDEF(&_10$$8);
	ZVAL_UNDEF(&_14$$9);
	ZVAL_UNDEF(&_15$$14);
	ZVAL_UNDEF(&_16$$15);
	ZVAL_UNDEF(&_17$$13);
	ZVAL_UNDEF(&_18$$13);
	ZVAL_UNDEF(&_19$$13);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);
	n = zephir_get_intval(n_param);


	maxFlipsCount = 0;
	permCount = 0;
	checksum = 0;
	ZEPHIR_INIT_VAR(&perm);
	array_init(&perm);
	ZEPHIR_INIT_VAR(&perm1);
	array_init(&perm1);
	ZEPHIR_INIT_VAR(&count);
	array_init(&count);
	_2 = n;
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
			zephir_array_update_long(&perm1, i, &_3$$3, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		}
	}
	r = n;
	while (1) {
		if (0) {
			break;
		}
		while (1) {
			if (!(r != 1)) {
				break;
			}
			ZEPHIR_INIT_NVAR(&_4$$6);
			ZVAL_LONG(&_4$$6, r);
			zephir_array_update_long(&count, (r - 1), &_4$$6, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
			r--;
		}
		_7$$4 = n;
		_6$$4 = 0;
		_5$$4 = 0;
		if (_6$$4 <= _7$$4) {
			while (1) {
				if (_5$$4) {
					_6$$4++;
					if (!(_6$$4 <= _7$$4)) {
						break;
					}
				} else {
					_5$$4 = 1;
				}
				i = _6$$4;
				zephir_array_fetch_long(&_8$$7, &perm1, i, PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 43);
				zephir_array_update_long(&perm, i, &_8$$7, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
			}
		}
		flipsCount = 0;
		k = 0;
		while (1) {
			zephir_array_fetch_long(&_9$$4, &perm, 0, PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 48);
			if (!(!ZEPHIR_IS_LONG(&_9$$4, 0))) {
				break;
			}
			ZEPHIR_OBS_NVAR(&_10$$8);
			zephir_array_fetch_long(&_10$$8, &perm, 0, PH_NOISY, "stub/fannkuch.zep", 49);
			k = zephir_get_intval(&_10$$8);
			k2 = (long) (((zephir_safe_div_long_long(((k + 1)), 2)) - (double) (1)));
			_13$$8 = k2;
			_12$$8 = 0;
			_11$$8 = 0;
			if (_12$$8 <= _13$$8) {
				while (1) {
					if (_11$$8) {
						_12$$8++;
						if (!(_12$$8 <= _13$$8)) {
							break;
						}
					} else {
						_11$$8 = 1;
					}
					i = _12$$8;
					zephir_array_fetch_long(&temp, &perm, i, PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 51);
					zephir_array_fetch_long(&_14$$9, &perm, (k - i), PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 51);
					zephir_array_update_long(&perm, i, &_14$$9, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
					zephir_array_update_long(&perm, (k - i), &temp, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
				}
			}
			flipsCount++;
		}
		if (maxFlipsCount < flipsCount) {
			maxFlipsCount = flipsCount;
		}
		if (zephir_safe_mod_long_long(permCount, 2) == 0) {
			checksum += flipsCount;
		} else {
			checksum -= flipsCount;
		}
		while (1) {
			if (r == n) {
				zephir_create_array(return_value, 3, 0);
				ZEPHIR_INIT_NVAR(&_15$$14);
				ZVAL_LONG(&_15$$14, checksum);
				zephir_array_fast_append(return_value, &_15$$14);
				ZEPHIR_INIT_NVAR(&_15$$14);
				ZVAL_LONG(&_15$$14, n);
				zephir_array_fast_append(return_value, &_15$$14);
				ZEPHIR_INIT_NVAR(&_15$$14);
				ZVAL_LONG(&_15$$14, maxFlipsCount);
				zephir_array_fast_append(return_value, &_15$$14);
				RETURN_MM();
			}
			zephir_array_fetch_long(&perm0, &perm1, 0, PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 75);
			i = 0;
			while (1) {
				if (!(i < r)) {
					break;
				}
				j = (i + 1);
				zephir_array_fetch_long(&_16$$15, &perm1, j, PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 78);
				zephir_array_update_long(&perm1, i, &_16$$15, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
				i = j;
			}
			zephir_array_update_long(&perm1, r, &perm0, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
			zephir_array_fetch_long(&_17$$13, &count, r, PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 82);
			ZEPHIR_INIT_NVAR(&_18$$13);
			ZVAL_LONG(&_18$$13, (zephir_get_numberval(&_17$$13) - 1));
			zephir_array_update_long(&count, r, &_18$$13, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
			zephir_array_fetch_long(&_19$$13, &count, r, PH_NOISY | PH_READONLY, "stub/fannkuch.zep", 83);
			if (ZEPHIR_GT_LONG(&_19$$13, 0)) {
				break;
			}
			r++;
		}
		permCount++;
	}
	ZEPHIR_MM_RESTORE();
}

