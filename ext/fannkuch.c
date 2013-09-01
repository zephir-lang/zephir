
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/array.h"


/**
 * Test\Fannkuch
 *
 * Fannkuch Refux in Zephir
 *
 * @see http://disciple-devel.blogspot.mx/2010/11/shootout-fannkuch-redux.html
 */
ZEPHIR_INIT_CLASS(Test_Fannkuch) {

	ZEPHIR_REGISTER_CLASS(Test, Fannkuch, fannkuch, test_fannkuch_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Fannkuch, process) {

	zend_bool _0, _3, _7;
	zval *n_param = NULL, *perm, *perm0 = NULL, *perm1, *count, *temp = NULL, *_2 = NULL, *_5 = NULL, *_6 = NULL, *_9 = NULL, *_10 = NULL, *_11 = NULL, *_12 = NULL;
	int n, maxFlipsCount = 0, permCount = 0, checksum = 0, i, j, flipsCount, k, r, k2, _1, _4, _8;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

		n = zephir_get_intval(n_param);


	ZEPHIR_INIT_VAR(perm);
	array_init(perm);
	ZEPHIR_INIT_VAR(perm1);
	array_init(perm1);
	ZEPHIR_INIT_VAR(count);
	array_init(count);
	_1 = 0;
	_0 = 0;
	while (1 && (_1 <= n)) {
		if (_0) {
			_1++;
			if (!(_1 <= n)) {
				break;
			}
		} else {
			_0 = 1;
		}
		i = _1;
		ZEPHIR_INIT_NVAR(_2);
		ZVAL_LONG(_2, i);
		zephir_array_update_long(&perm1, i, &_2, PH_COPY | PH_SEPARATE);
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
			ZEPHIR_INIT_NVAR(_2);
			ZVAL_LONG(_2, r);
			zephir_array_update_long(&count, r - 1, &_2, PH_COPY | PH_SEPARATE);
			r--;
		}
		_4 = 0;
		_3 = 0;
		while (1 && (_4 <= n)) {
			if (_3) {
				_4++;
				if (!(_4 <= n)) {
					break;
				}
			} else {
				_3 = 1;
			}
			i = _4;
			ZEPHIR_OBS_NVAR(_5);
			zephir_array_fetch_long(&_5, perm1, i, PH_NOISY);
			zephir_array_update_long(&perm, i, &_5, PH_COPY | PH_SEPARATE);
		}
		flipsCount = 0;
		k = 0;
		while (1) {
			ZEPHIR_OBS_NVAR(_5);
			zephir_array_fetch_long(&_5, perm, 0, PH_NOISY);
			if (!(!ZEPHIR_IS_LONG(_5, 0))) {
				break;
			}
			ZEPHIR_OBS_NVAR(_6);
			zephir_array_fetch_long(&_6, perm, 0, PH_NOISY);
			k = zephir_get_intval(_6);
			k2 = ((k + 1) / 2) - 1;
			_8 = 0;
			_7 = 0;
			while (1 && (_8 <= k2)) {
				if (_7) {
					_8++;
					if (!(_8 <= k2)) {
						break;
					}
				} else {
					_7 = 1;
				}
				i = _8;
				ZEPHIR_OBS_NVAR(temp);
				zephir_array_fetch_long(&temp, perm, i, PH_NOISY);
				ZEPHIR_OBS_NVAR(_9);
				zephir_array_fetch_long(&_9, perm, k - i, PH_NOISY);
				zephir_array_update_long(&perm, i, &_9, PH_COPY | PH_SEPARATE);
				zephir_array_update_long(&perm, k - i, &temp, PH_COPY | PH_SEPARATE);
			}
			flipsCount++;
		}
		if (maxFlipsCount < flipsCount) {
			maxFlipsCount = flipsCount;
		}
		if (permCount % 2 == 0) {
			checksum += flipsCount;
		} else {
			checksum -= flipsCount;
		}
		while (1) {
			if (r == n) {
				array_init(return_value);
				ZEPHIR_INIT_NVAR(_10);
				ZVAL_LONG(_10, checksum);
				zephir_array_append(&return_value, _10, 0);
				ZEPHIR_INIT_NVAR(_11);
				ZVAL_LONG(_11, n);
				zephir_array_append(&return_value, _11, 0);
				ZEPHIR_INIT_NVAR(_12);
				ZVAL_LONG(_12, maxFlipsCount);
				zephir_array_append(&return_value, _12, 0);
				RETURN_MM();
			}
			ZEPHIR_OBS_NVAR(perm0);
			zephir_array_fetch_long(&perm0, perm1, 0, PH_NOISY);
			i = 0;
			while (1) {
				if (!(i < r)) {
					break;
				}
				j = i + 1;
				ZEPHIR_OBS_NVAR(_5);
				zephir_array_fetch_long(&_5, perm1, j, PH_NOISY);
				zephir_array_update_long(&perm1, i, &_5, PH_COPY | PH_SEPARATE);
				i = j;
			}
			zephir_array_update_long(&perm1, r, &perm0, PH_COPY | PH_SEPARATE);
			ZEPHIR_OBS_NVAR(_6);
			zephir_array_fetch_long(&_6, count, r, PH_NOISY);
			ZEPHIR_INIT_NVAR(_10);
			ZVAL_LONG(_10, zephir_get_numberval(_6) - 1);
			zephir_array_update_long(&count, r, &_10, PH_COPY | PH_SEPARATE);
			ZEPHIR_OBS_NVAR(_9);
			zephir_array_fetch_long(&_9, count, r, PH_NOISY);
			if (ZEPHIR_GT_LONG(_9, 0)) {
				break;
			}
			r++;
		}
		permCount++;
	}
	ZEPHIR_MM_RESTORE();

}

