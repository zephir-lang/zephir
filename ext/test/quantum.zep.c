
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
#include "kernel/fcall.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Quantum) {

	ZEPHIR_REGISTER_CLASS(Test, Quantum, test, quantum, test_quantum_method_entry, 0);

	zend_declare_class_constant_double(test_quantum_ce, SL("PI"), 3.14159265358979323846 TSRMLS_CC);

	zend_declare_class_constant_long(test_quantum_ce, SL("MAX"), 751 TSRMLS_CC);

	return SUCCESS;

}

/*
 */
PHP_METHOD(Test_Quantum, Harmos) {

	zend_bool _14, _17, _33, _36;
	zval *_11 = NULL, *_12 = NULL, *_13 = NULL;
	zephir_nts_static zephir_fcall_cache_entry *_3 = NULL, *_5 = NULL, *_7 = NULL, *_9 = NULL, *_37 = NULL;
	int i, j, n, ZEPHIR_LAST_CALL_STATUS, _15, _16, _18, _19, _34, _35;
	zval *x_param = NULL, *psr, *psi, *p2, *v, *paramater, *fp = NULL, *_0 = NULL, _1 = zval_used_for_init, _2 = zval_used_for_init, *_4 = NULL, *_6 = NULL, *_8 = NULL, *_10 = NULL, *tmp, *_20, *_21, *_22, *_23, *_24, *_25, *_26, *_27, _28 = zval_used_for_init, *_29, *_30, *_31, *_32;
	double x, dt, dx, k0, item_psr, item_psi;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &x_param);

	x = zephir_get_doubleval(x_param);


	dx = 0.02;
	k0 = (3.0 * 3.14159265358979323846);
	dt = ((dx * dx) / 4.0);
	ZEPHIR_INIT_VAR(paramater);
	array_init_size(paramater, 6);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, dx);
	zephir_array_fast_append(paramater, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, k0);
	zephir_array_fast_append(paramater, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, dt);
	zephir_array_fast_append(paramater, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, x);
	zephir_array_fast_append(paramater, _0);
	i = 0;
	ZEPHIR_INIT_VAR(psr);
	array_init(psr);
	ZEPHIR_INIT_VAR(psi);
	array_init(psi);
	ZEPHIR_INIT_VAR(p2);
	array_init(p2);
	ZEPHIR_INIT_VAR(v);
	array_init(v);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "harmos.txt", 0);
	ZEPHIR_SINIT_VAR(_2);
	ZVAL_STRING(&_2, "w", 0);
	ZEPHIR_CALL_FUNCTION(&fp, "fopen", &_3, &_1, &_2);
	zephir_check_call_status();
	if (!(zephir_is_true(fp))) {
		RETURN_MM_LONG(1);
	}
	while (1) {
		if (!(i <= 751)) {
			break;
		}
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, (k0 * x));
		ZEPHIR_CALL_FUNCTION(&_4, "sin", &_5, &_1);
		zephir_check_call_status();
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_6, "exp", &_7, &_1);
		zephir_check_call_status();
		ZEPHIR_INIT_LNVAR(_8);
		div_function(_8, _4, _6 TSRMLS_CC);
		item_psi = zephir_get_numberval(_8);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, (k0 * x));
		ZEPHIR_CALL_FUNCTION(&_4, "cos", &_9, &_1);
		zephir_check_call_status();
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_6, "exp", &_7, &_1);
		zephir_check_call_status();
		ZEPHIR_INIT_LNVAR(_10);
		div_function(_10, _4, _6 TSRMLS_CC);
		item_psr = zephir_get_numberval(_10);
		ZEPHIR_INIT_NVAR(_11);
		array_init_size(_11, 2);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, item_psr);
		zephir_array_fast_append(_11, _0);
		zephir_array_update_long(&psr, i, &_11, PH_COPY | PH_SEPARATE, "test/quantum.zep", 38);
		ZEPHIR_INIT_NVAR(_12);
		array_init_size(_12, 2);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, item_psi);
		zephir_array_fast_append(_12, _0);
		zephir_array_update_long(&psi, i, &_12, PH_COPY | PH_SEPARATE, "test/quantum.zep", 39);
		ZEPHIR_INIT_NVAR(_13);
		array_init_size(_13, 2);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, (double) ((double) (5.0 * x) * x));
		zephir_array_fast_append(_13, _0);
		zephir_array_update_long(&v, i, &_13, PH_COPY | PH_SEPARATE, "test/quantum.zep", 40);
		x = (x + dx);
		i++;
	}
	i = 1;
	j = 1;
	ZEPHIR_INIT_VAR(tmp);
	array_init_size(tmp, 2);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_DOUBLE(_0, 2.0);
	zephir_array_fast_append(tmp, _0);
	_16 = 20000;
	_15 = 0;
	_14 = 0;
	if (_15 <= _16) {
		while (1) {
			if (_14) {
				_15++;
				if (!(_15 <= _16)) {
					break;
				}
			} else {
				_14 = 1;
			}
			n = _15;
			_19 = (751 - 1);
			_18 = 1;
			_17 = 0;
			if (_18 <= _19) {
				while (1) {
					if (_17) {
						_18++;
						if (!(_18 <= _19)) {
							break;
						}
					} else {
						_17 = 1;
					}
					i = _18;
					zephir_array_fetch_long(&_20, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _20, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_22, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_23, psi, (i + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _23, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_25, psi, (i - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_26, _25, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function(&_1, _24, _26 TSRMLS_CC);
					zephir_array_fetch_long(&_24, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_26, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_27, _26, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _24, _27 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_8);
					zephir_sub_function(_8, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _22, _8 TSRMLS_CC);
					zephir_array_fetch_long(&_22, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_24, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_8);
					mul_function(_8, _22, _24 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _8 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_sub_function(&_1, _21, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_22, v, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _22, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _21, _24 TSRMLS_CC);
					zephir_array_fetch_long(&_21, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _21, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_28);
					mul_function(&_28, &_2, _24 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_8);
					zephir_add_function(_8, &_1, &_28 TSRMLS_CC);
					zephir_array_update_multi(&psr, &_8 TSRMLS_CC, SL("ll"), 2, i, 1);
					zephir_array_fetch_long(&_24, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_27, _24, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_29, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_30, _29, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _27, _30 TSRMLS_CC);
					zephir_array_fetch_long(&_27, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_30, _27, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_31, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_32, _31, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _30, _32 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_8);
					zephir_add_function(_8, &_1, &_2 TSRMLS_CC);
					zephir_array_update_long(&p2, i, &_8, PH_COPY | PH_SEPARATE, "test/quantum.zep", 53);
				}
			}
			_35 = (751 - 1);
			_34 = 1;
			_33 = 0;
			if (_34 <= _35) {
				while (1) {
					if (_33) {
						_34++;
						if (!(_34 <= _35)) {
							break;
						}
					} else {
						_33 = 1;
					}
					j = _34;
					ZEPHIR_INIT_NVAR(_0);
					ZVAL_LONG(_0, 0);
					zephir_array_update_multi(&psr, &_0 TSRMLS_CC, SL("ll"), 2, 0, 1);
					ZEPHIR_INIT_NVAR(_0);
					ZVAL_LONG(_0, 0);
					zephir_array_update_multi(&psr, &_0 TSRMLS_CC, SL("ll"), 2, 751, 1);
					zephir_array_fetch_long(&_20, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _20, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_22, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_23, psr, (j + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _23, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_25, psr, (j - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_26, _25, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function(&_1, _24, _26 TSRMLS_CC);
					zephir_array_fetch_long(&_24, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_26, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_27, _26, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _24, _27 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_10);
					zephir_sub_function(_10, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _22, _10 TSRMLS_CC);
					zephir_array_fetch_long(&_22, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_24, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_10);
					mul_function(_10, _22, _24 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _10 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function(&_1, _21, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_22, v, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _22, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _21, _24 TSRMLS_CC);
					zephir_array_fetch_long(&_21, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _21, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_28);
					mul_function(&_28, &_2, _24 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_10);
					zephir_sub_function(_10, &_1, &_28 TSRMLS_CC);
					zephir_array_update_multi(&psi, &_10 TSRMLS_CC, SL("ll"), 2, j, 1);
				}
			}
			_36 = n == 0;
			if (!(_36)) {
				_36 = (n % 2000) == 0;
			}
			if (_36) {
				i = 1;
				while (1) {
					if (!(i < (751 - 1))) {
						break;
					}
					zephir_array_fetch_long(&_20, p2, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 67 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					ZVAL_STRING(&_1, "%16.8lf %16.8lf %16.8lf \n", 0);
					ZEPHIR_SINIT_NVAR(_2);
					ZVAL_DOUBLE(&_2, ((double) i * dx));
					ZEPHIR_SINIT_NVAR(_28);
					ZVAL_DOUBLE(&_28, ((double) n * dt));
					ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_37, fp, &_1, &_2, &_28, _20);
					zephir_check_call_status();
					i = (i + 10);
				}
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_STRING(&_1, "\n", 0);
				ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_37, fp, &_1);
				zephir_check_call_status();
			}
			j = 1;
			while (1) {
				if (!(j < (751 - 1))) {
					break;
				}
				zephir_array_fetch_long(&_20, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_fetch_long(&_21, _20, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_update_multi(&psi, &_21 TSRMLS_CC, SL("ll"), 2, j, 0);
				zephir_array_fetch_long(&_21, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_fetch_long(&_22, _21, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_update_multi(&psr, &_22 TSRMLS_CC, SL("ll"), 2, j, 0);
				j++;
			}
		}
	}
	RETURN_MM_BOOL(1);

}

PHP_METHOD(Test_Quantum, testString) {


	RETURN_STRING("I love Phalcon and Zephir", 1);

}

