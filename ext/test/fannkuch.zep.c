
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


/**
 * Test\Fannkuch
 *
 * Fannkuch Redux in Zephir
 *
 * @see http://disciple-devel.blogspot.mx/2010/11/shootout-fannkuch-redux.html
 */
ZEPHIR_INIT_CLASS(Test_Fannkuch) {

	ZEPHIR_REGISTER_CLASS(Test, Fannkuch, test, fannkuch, test_fannkuch_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Fannkuch, process) {

	zend_bool _0, _4, _9;
	zval *n_param = NULL, *perm, *perm0, *perm1, *count, *temp, *_3 = NULL, *_7, *_8 = NULL, *_12, *_13 = NULL, *_14;
	int n, maxFlipsCount = 0, permCount = 0, checksum = 0, i, j, flipsCount, k, r, k2, _1, _2, _5, _6, _10, _11;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	ZEPHIR_INIT_VAR(perm);
	array_init(perm);
	ZEPHIR_INIT_VAR(perm1);
	array_init(perm1);
	ZEPHIR_INIT_VAR(count);
	array_init(count);
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
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_LONG(_3, i);
			zephir_array_update_long(&perm1, i, &_3, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 27);
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
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_LONG(_3, r);
			zephir_array_update_long(&count, (r - 1), &_3, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 39);
			r--;
		}
		_6 = n;
		_5 = 0;
		_4 = 0;
		if (_5 <= _6) {
			while (1) {
				if (_4) {
					_5++;
					if (!(_5 <= _6)) {
						break;
					}
				} else {
					_4 = 1;
				}
				i = _5;
				zephir_array_fetch_long(&_7, perm1, i, PH_NOISY | PH_READONLY, "test/fannkuch.zep", 43 TSRMLS_CC);
				zephir_array_update_long(&perm, i, &_7, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 43);
			}
		}
		flipsCount = 0;
		k = 0;
		while (1) {
			zephir_array_fetch_long(&_7, perm, 0, PH_NOISY | PH_READONLY, "test/fannkuch.zep", 48 TSRMLS_CC);
			if (!(!ZEPHIR_IS_LONG(_7, 0))) {
				break;
			}
			ZEPHIR_OBS_NVAR(_8);
			zephir_array_fetch_long(&_8, perm, 0, PH_NOISY, "test/fannkuch.zep", 49 TSRMLS_CC);
			k = zephir_get_intval(_8);
			k2 = (long) (((zephir_safe_div_long_long(((k + 1)), 2 TSRMLS_CC)) - (double) (1)));
			_11 = k2;
			_10 = 0;
			_9 = 0;
			if (_10 <= _11) {
				while (1) {
					if (_9) {
						_10++;
						if (!(_10 <= _11)) {
							break;
						}
					} else {
						_9 = 1;
					}
					i = _10;
					zephir_array_fetch_long(&temp, perm, i, PH_NOISY | PH_READONLY, "test/fannkuch.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_12, perm, (k - i), PH_NOISY | PH_READONLY, "test/fannkuch.zep", 51 TSRMLS_CC);
					zephir_array_update_long(&perm, i, &_12, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 51);
					zephir_array_update_long(&perm, (k - i), &temp, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 51);
				}
			}
			flipsCount++;
		}
		if (maxFlipsCount < flipsCount) {
			maxFlipsCount = flipsCount;
		}
		if (zephir_safe_mod_long_long(permCount, 2 TSRMLS_CC) == 0) {
			checksum += flipsCount;
		} else {
			checksum -= flipsCount;
		}
		while (1) {
			if (r == n) {
				zephir_create_array(return_value, 3, 0 TSRMLS_CC);
				ZEPHIR_INIT_NVAR(_13);
				ZVAL_LONG(_13, checksum);
				zephir_array_fast_append(return_value, _13);
				ZEPHIR_INIT_NVAR(_13);
				ZVAL_LONG(_13, n);
				zephir_array_fast_append(return_value, _13);
				ZEPHIR_INIT_NVAR(_13);
				ZVAL_LONG(_13, maxFlipsCount);
				zephir_array_fast_append(return_value, _13);
				RETURN_MM();
			}
			zephir_array_fetch_long(&perm0, perm1, 0, PH_NOISY | PH_READONLY, "test/fannkuch.zep", 75 TSRMLS_CC);
			i = 0;
			while (1) {
				if (!(i < r)) {
					break;
				}
				j = (i + 1);
				zephir_array_fetch_long(&_7, perm1, j, PH_NOISY | PH_READONLY, "test/fannkuch.zep", 78 TSRMLS_CC);
				zephir_array_update_long(&perm1, i, &_7, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 78);
				i = j;
			}
			zephir_array_update_long(&perm1, r, &perm0, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 82);
			zephir_array_fetch_long(&_12, count, r, PH_NOISY | PH_READONLY, "test/fannkuch.zep", 82 TSRMLS_CC);
			ZEPHIR_INIT_NVAR(_13);
			ZVAL_LONG(_13, (zephir_get_numberval(_12) - 1));
			zephir_array_update_long(&count, r, &_13, PH_COPY | PH_SEPARATE, "test/fannkuch.zep", 82);
			zephir_array_fetch_long(&_14, count, r, PH_NOISY | PH_READONLY, "test/fannkuch.zep", 83 TSRMLS_CC);
			if (ZEPHIR_GT_LONG(_14, 0)) {
				break;
			}
			r++;
		}
		permCount++;
	}
	ZEPHIR_MM_RESTORE();

}

