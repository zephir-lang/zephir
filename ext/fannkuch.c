
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

	zend_bool _0, _4, _9;
	zval *n_param = NULL, *perm, *perm0 = NULL, *perm1, *count, *temp = NULL, *_2, *_3 = NULL, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_11 = NULL, *_12 = NULL, *_13 = NULL, *_14 = NULL, *_15 = NULL;
	int n, maxFlipsCount = 0, permCount = 0, checksum = 0, i, j, flipsCount, k, r, k2, _1, _5, _10;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

		n = zephir_get_intval(n_param);


	ZEPHIR_INIT_VAR(perm);
	array_init(perm);
	ZEPHIR_INIT_VAR(perm1);
	array_init(perm1);
	ZEPHIR_INIT_VAR(count);
	array_init(count);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, n);
	_1 = 0;
	_0 = 0;
	if (ZEPHIR_LE_LONG(_2, _1)) {
		while (1) {
			if (_0) {
				_1++;
				if (!(ZEPHIR_LE_LONG(_2, _1))) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			ZEPHIR_INIT_NVAR(_3);
			ZVAL_LONG(_3, i);
			zephir_array_update_long(&perm1, i, &_3, PH_COPY | PH_SEPARATE);
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
			zephir_array_update_long(&count, r - 1, &_3, PH_COPY | PH_SEPARATE);
			r--;
		}
		ZEPHIR_INIT_NVAR(_6);
		ZVAL_LONG(_6, n);
		_5 = 0;
		_4 = 0;
		if (ZEPHIR_LE_LONG(_6, _5)) {
			while (1) {
				if (_4) {
					_5++;
					if (!(ZEPHIR_LE_LONG(_6, _5))) {
						break;
					}
				} else {
					_4 = 1;
				}
				i = _5;
				ZEPHIR_OBS_NVAR(_7);
				zephir_array_fetch_long(&_7, perm1, i, PH_NOISY);
				zephir_array_update_long(&perm, i, &_7, PH_COPY | PH_SEPARATE);
			}
		}
		flipsCount = 0;
		k = 0;
		while (1) {
			ZEPHIR_OBS_NVAR(_7);
			zephir_array_fetch_long(&_7, perm, 0, PH_NOISY);
			if (!(!ZEPHIR_IS_LONG(_7, 0))) {
				break;
			}
			ZEPHIR_OBS_NVAR(_8);
			zephir_array_fetch_long(&_8, perm, 0, PH_NOISY);
			k = zephir_get_intval(_8);
			k2 = ((k + 1) / 2) - 1;
			ZEPHIR_INIT_NVAR(_11);
			ZVAL_LONG(_11, k2);
			_10 = 0;
			_9 = 0;
			if (ZEPHIR_LE_LONG(_11, _10)) {
				while (1) {
					if (_9) {
						_10++;
						if (!(ZEPHIR_LE_LONG(_11, _10))) {
							break;
						}
					} else {
						_9 = 1;
					}
					i = _10;
					ZEPHIR_OBS_NVAR(temp);
					zephir_array_fetch_long(&temp, perm, i, PH_NOISY);
					ZEPHIR_OBS_NVAR(_12);
					zephir_array_fetch_long(&_12, perm, k - i, PH_NOISY);
					zephir_array_update_long(&perm, i, &_12, PH_COPY | PH_SEPARATE);
					zephir_array_update_long(&perm, k - i, &temp, PH_COPY | PH_SEPARATE);
				}
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
				ZEPHIR_INIT_NVAR(_13);
				ZVAL_LONG(_13, checksum);
				zephir_array_append(&return_value, _13, 0);
				ZEPHIR_INIT_NVAR(_14);
				ZVAL_LONG(_14, n);
				zephir_array_append(&return_value, _14, 0);
				ZEPHIR_INIT_NVAR(_15);
				ZVAL_LONG(_15, maxFlipsCount);
				zephir_array_append(&return_value, _15, 0);
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
				ZEPHIR_OBS_NVAR(_7);
				zephir_array_fetch_long(&_7, perm1, j, PH_NOISY);
				zephir_array_update_long(&perm1, i, &_7, PH_COPY | PH_SEPARATE);
				i = j;
			}
			zephir_array_update_long(&perm1, r, &perm0, PH_COPY | PH_SEPARATE);
			ZEPHIR_OBS_NVAR(_8);
			zephir_array_fetch_long(&_8, count, r, PH_NOISY);
			ZEPHIR_INIT_NVAR(_13);
			ZVAL_LONG(_13, zephir_get_numberval(_8) - 1);
			zephir_array_update_long(&count, r, &_13, PH_COPY | PH_SEPARATE);
			ZEPHIR_OBS_NVAR(_12);
			zephir_array_fetch_long(&_12, count, r, PH_NOISY);
			if (ZEPHIR_GT_LONG(_12, 0)) {
				break;
			}
			r++;
		}
		permCount++;
	}
	ZEPHIR_MM_RESTORE();

}

