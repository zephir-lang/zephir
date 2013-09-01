
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
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Fannkuch) {

	ZEPHIR_REGISTER_CLASS(Test, Fannkuch, fannkuch, test_fannkuch_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Fannkuch, process) {

	zval *n_param = NULL, *s, *p = NULL, *q = NULL, *t = NULL, *q0 = NULL, *qq = NULL, *sx = NULL, _0 = zval_used_for_init, _1 = zval_used_for_init, *_2 = NULL, *_3 = NULL, _4 = zval_used_for_init, _5 = zval_used_for_init, _6 = zval_used_for_init, _7 = zval_used_for_init, *_8 = NULL, *_9 = NULL;
	int n, i, j, m, maxflips, checksum, sign, flips;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

		n = zephir_get_intval(n_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_LONG(&_1, n - 1);
	ZEPHIR_INIT_VAR(s);
	zephir_call_func_p2(s, "range", &_0, &_1);
	i = 0;
	maxflips = 0;
	checksum = 0;
	sign = 1;
	m = n - 1;
	ZEPHIR_CPY_WRT(q, s);
	ZEPHIR_CPY_WRT(p, s);
	while (1) {
		ZEPHIR_OBS_NVAR(q0);
		zephir_array_fetch_long(&q0, p, 0, PH_NOISY);
		if (ZEPHIR_IS_LONG(q0, 0)) {
			ZEPHIR_CPY_WRT(q, p);
			flips = 1;
			while (1) {
				ZEPHIR_OBS_NVAR(qq);
				zephir_array_fetch(&qq, q, q0, PH_NOISY);
				if (ZEPHIR_IS_LONG(qq, 0)) {
					checksum += sign * flips;
					if (flips > maxflips) {
						maxflips = flips;
					}
					break;
				}
				zephir_array_update_zval(&q, q0, &q0, PH_COPY | PH_SEPARATE);
				if (ZEPHIR_IS_LONG(q0, 3)) {
					i = 1;
					j = zephir_get_numberval(q0) - 1;
					while (i < j) {
						ZEPHIR_OBS_NVAR(t);
						zephir_array_fetch_long(&t, q, i, PH_NOISY);
						ZEPHIR_OBS_NVAR(_2);
						zephir_array_fetch_long(&_2, q, j, PH_NOISY);
						zephir_array_update_long(&q, i, &_2, PH_COPY | PH_SEPARATE);
						zephir_array_update_long(&q, j, &t, PH_COPY | PH_SEPARATE);
						i++;
						j--;
					}
				}
				ZEPHIR_CPY_WRT(q0, qq);
				flips++;
			}
		}
		if (sign == 1) {
			ZEPHIR_OBS_NVAR(t);
			zephir_array_fetch_long(&t, p, 1, PH_NOISY);
			ZEPHIR_OBS_NVAR(_2);
			zephir_array_fetch_long(&_2, p, 0, PH_NOISY);
			zephir_array_update_long(&p, 1, &_2, PH_COPY | PH_SEPARATE);
			zephir_array_update_long(&p, 0, &t, PH_COPY | PH_SEPARATE);
			sign = -1;
		} else {
			ZEPHIR_OBS_NVAR(t);
			zephir_array_fetch_long(&t, p, 1, PH_NOISY);
			ZEPHIR_OBS_NVAR(_3);
			zephir_array_fetch_long(&_3, p, 2, PH_NOISY);
			zephir_array_update_long(&p, 1, &_3, PH_COPY | PH_SEPARATE);
			zephir_array_update_long(&p, 2, &t, PH_COPY | PH_SEPARATE);
			sign = 1;
			i = 2;
			while (i < n) {
				ZEPHIR_OBS_NVAR(sx);
				zephir_array_fetch_long(&sx, s, i, PH_NOISY);
				if (ZEPHIR_IS_LONG(sx, 0)) {
					ZEPHIR_SEPARATE(sx);
					zephir_decrement(sx);
					zephir_array_update_long(&s, i, &sx, PH_COPY | PH_SEPARATE);
					break;
				}
				if (i == m) {
					ZEPHIR_SINIT_NVAR(_4);
					ZVAL_STRING(&_4, "%d\\nPfannkuchen(%d) = %d\\n", 0);
					ZEPHIR_SINIT_NVAR(_5);
					ZVAL_LONG(&_5, checksum);
					ZEPHIR_SINIT_NVAR(_6);
					ZVAL_LONG(&_6, n);
					ZEPHIR_SINIT_NVAR(_7);
					ZVAL_LONG(&_7, maxflips);
					zephir_call_func_p4(return_value, "printf", &_4, &_5, &_6, &_7);
					RETURN_MM();
				}
				ZEPHIR_INIT_NVAR(_8);
				ZVAL_LONG(_8, i);
				zephir_array_update_long(&s, i, &_8, PH_COPY | PH_SEPARATE);
				ZEPHIR_OBS_NVAR(t);
				zephir_array_fetch_long(&t, p, 0, PH_NOISY);
				j = 0;
				while (j <= i) {
					ZEPHIR_OBS_NVAR(_9);
					zephir_array_fetch_long(&_9, p, j, PH_NOISY);
					zephir_array_update_long(&p, j, &_9, PH_COPY | PH_SEPARATE);
					j++;
				}
				i++;
				zephir_array_update_long(&p, i, &t, PH_COPY | PH_SEPARATE);
			}
		}
	}
	ZEPHIR_MM_RESTORE();

}

