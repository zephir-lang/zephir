
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Quantum) {

	ZEPHIR_REGISTER_CLASS(Test, Quantum, test, quantum, test_quantum_method_entry, 0);

	zend_declare_class_constant_double(test_quantum_ce, SL("PI"), 3.14159265358979323846 TSRMLS_CC);

	zend_declare_class_constant_long(test_quantum_ce, SL("MAX"), 751 TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Quantum, harmos) {

	zend_bool _9, _12, _29, _32;
	zval *_6 = NULL, *_7 = NULL, *_8 = NULL;
	zephir_fcall_cache_entry *_4 = NULL, *_33 = NULL;
	int i, j, n, ZEPHIR_LAST_CALL_STATUS, _10, _11, _13, _14, _30, _31;
	zval *x_param = NULL, *psr, *psi, *p2, *v, *paramater, *fp = NULL, *tmp, *_0 = NULL, _1 = zval_used_for_init, _2 = zval_used_for_init, *_3 = NULL, *_5 = NULL, *_15, *_16, *_17, *_18, *_19, *_20, *_21, *_22, *_23 = NULL, _24 = zval_used_for_init, *_25, *_26, *_27, *_28;
	double x, dt, dx, k0, item_psr, item_psi;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &x_param);

	x = zephir_get_doubleval(x_param);


	dx = 0.02;
	k0 = (3.0 * 3.14159265358979323846);
	dt = zephir_safe_div_double_long((dx * dx), 4.0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(paramater);
	zephir_create_array(paramater, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_DOUBLE(_0, dx);
	zephir_array_fast_append(paramater, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, k0);
	zephir_array_fast_append(paramater, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, dt);
	zephir_array_fast_append(paramater, _0);
	ZEPHIR_INIT_NVAR(_0);
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
	ZEPHIR_CALL_FUNCTION(&fp, "fopen", NULL, 30, &_1, &_2);
	zephir_check_call_status();
	if (!(zephir_is_true(fp))) {
		RETURN_MM_LONG(1);
	}
	while (1) {
		if (!(i <= 751)) {
			break;
		}
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_3, "exp", &_4, 1, &_1);
		zephir_check_call_status();
		item_psi = zephir_safe_div_double_zval(sin((k0 * x)), _3 TSRMLS_CC);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_5, "exp", &_4, 1, &_1);
		zephir_check_call_status();
		item_psr = zephir_safe_div_double_zval(cos((k0 * x)), _5 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_6);
		zephir_create_array(_6, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, item_psr);
		zephir_array_fast_append(_6, _0);
		zephir_array_update_long(&psr, i, &_6, PH_COPY | PH_SEPARATE, "test/quantum.zep", 39);
		ZEPHIR_INIT_NVAR(_7);
		zephir_create_array(_7, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, item_psi);
		zephir_array_fast_append(_7, _0);
		zephir_array_update_long(&psi, i, &_7, PH_COPY | PH_SEPARATE, "test/quantum.zep", 40);
		ZEPHIR_INIT_NVAR(_8);
		zephir_create_array(_8, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, (double) ((double) (5.0 * x) * x));
		zephir_array_fast_append(_8, _0);
		zephir_array_update_long(&v, i, &_8, PH_COPY | PH_SEPARATE, "test/quantum.zep", 41);
		x = (x + dx);
		i++;
	}
	i = 1;
	j = 1;
	ZEPHIR_INIT_VAR(tmp);
	zephir_create_array(tmp, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_DOUBLE(_0, 2.0);
	zephir_array_fast_append(tmp, _0);
	_11 = 20000;
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
			n = _10;
			_14 = (751 - 1);
			_13 = 1;
			_12 = 0;
			if (_13 <= _14) {
				while (1) {
					if (_12) {
						_13++;
						if (!(_13 <= _14)) {
							break;
						}
					} else {
						_12 = 1;
					}
					i = _13;
					zephir_array_fetch_long(&_15, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_16, _15, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_17, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_18, psi, (i + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_19, _18, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_20, psi, (i - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _20, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _19, _21 TSRMLS_CC);
					zephir_array_fetch_long(&_19, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_21, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_22, _21, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _19, _22 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_23);
					sub_function(_23, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _17, _23 TSRMLS_CC);
					zephir_array_fetch_long(&_17, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_19, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_23);
					mul_function(_23, _17, _19 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _23 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					sub_function(&_1, _16, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_16, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_17, v, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_19, _17, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _16, _19 TSRMLS_CC);
					zephir_array_fetch_long(&_16, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_19, _16, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_24);
					mul_function(&_24, &_2, _19 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_23);
					zephir_add_function_ex(_23, &_1, &_24 TSRMLS_CC);
					zephir_array_update_multi(&psr, &_23 TSRMLS_CC, SL("ll"), 2, i, 1);
					zephir_array_fetch_long(&_19, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_22, _19, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_25, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_26, _25, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _22, _26 TSRMLS_CC);
					zephir_array_fetch_long(&_22, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_26, _22, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_27, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_28, _27, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _26, _28 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_23);
					zephir_add_function_ex(_23, &_1, &_2 TSRMLS_CC);
					zephir_array_update_long(&p2, i, &_23, PH_COPY | PH_SEPARATE, "test/quantum.zep", 53);
				}
			}
			_31 = (751 - 1);
			_30 = 1;
			_29 = 0;
			if (_30 <= _31) {
				while (1) {
					if (_29) {
						_30++;
						if (!(_30 <= _31)) {
							break;
						}
					} else {
						_29 = 1;
					}
					j = _30;
					ZEPHIR_INIT_NVAR(_0);
					ZVAL_LONG(_0, 0);
					zephir_array_update_multi(&psr, &_0 TSRMLS_CC, SL("ll"), 2, 0, 1);
					ZEPHIR_INIT_NVAR(_0);
					ZVAL_LONG(_0, 0);
					zephir_array_update_multi(&psr, &_0 TSRMLS_CC, SL("ll"), 2, 751, 1);
					zephir_array_fetch_long(&_15, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_16, _15, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_17, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_18, psr, (j + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_19, _18, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_20, psr, (j - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _20, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _19, _21 TSRMLS_CC);
					zephir_array_fetch_long(&_19, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_21, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_22, _21, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _19, _22 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_23);
					sub_function(_23, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _17, _23 TSRMLS_CC);
					zephir_array_fetch_long(&_17, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_19, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_23);
					mul_function(_23, _17, _19 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _23 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _16, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_16, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_17, v, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_19, _17, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _16, _19 TSRMLS_CC);
					zephir_array_fetch_long(&_16, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_19, _16, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_24);
					mul_function(&_24, &_2, _19 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_23);
					sub_function(_23, &_1, &_24 TSRMLS_CC);
					zephir_array_update_multi(&psi, &_23 TSRMLS_CC, SL("ll"), 2, j, 1);
				}
			}
			_32 = n == 0;
			if (!(_32)) {
				_32 = zephir_safe_mod_long_long(n, 2000 TSRMLS_CC) == 0;
			}
			if (_32) {
				i = 1;
				while (1) {
					if (!(i < (751 - 1))) {
						break;
					}
					zephir_array_fetch_long(&_15, p2, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 67 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					ZVAL_STRING(&_1, "%16.8lf %16.8lf %16.8lf \n", 0);
					ZEPHIR_SINIT_NVAR(_2);
					ZVAL_DOUBLE(&_2, ((double) i * dx));
					ZEPHIR_SINIT_NVAR(_24);
					ZVAL_DOUBLE(&_24, ((double) n * dt));
					ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_33, 57, fp, &_1, &_2, &_24, _15);
					zephir_check_call_status();
					i = (i + 10);
				}
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_STRING(&_1, "\n", 0);
				ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_33, 57, fp, &_1);
				zephir_check_call_status();
			}
			j = 1;
			while (1) {
				if (!(j < (751 - 1))) {
					break;
				}
				zephir_array_fetch_long(&_15, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_fetch_long(&_16, _15, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_update_multi(&psi, &_16 TSRMLS_CC, SL("ll"), 2, j, 0);
				zephir_array_fetch_long(&_16, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_fetch_long(&_17, _16, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_update_multi(&psr, &_17 TSRMLS_CC, SL("ll"), 2, j, 0);
				j++;
			}
		}
	}
	RETURN_MM_BOOL(1);

}

