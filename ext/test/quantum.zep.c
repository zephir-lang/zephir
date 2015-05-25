
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

	zend_bool _13, _16, _32, _35;
	zval *_10 = NULL, *_11 = NULL, *_12 = NULL;
	zephir_fcall_cache_entry *_4 = NULL, *_6 = NULL, *_8 = NULL, *_36 = NULL;
	int i, j, n, ZEPHIR_LAST_CALL_STATUS, _14, _15, _17, _18, _33, _34;
	zval *x_param = NULL, *psr, *psi, *p2, *v, *paramater, *fp = NULL, *tmp, *_0 = NULL, _1 = zval_used_for_init, _2 = zval_used_for_init, *_3 = NULL, *_5 = NULL, *_7 = NULL, *_9 = NULL, *_19, *_20, *_21, *_22, *_23, *_24, *_25, *_26, _27 = zval_used_for_init, *_28, *_29, *_30, *_31;
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
		ZVAL_DOUBLE(&_1, (k0 * x));
		ZEPHIR_CALL_FUNCTION(&_3, "sin", &_4, 10, &_1);
		zephir_check_call_status();
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_5, "exp", &_6, 1, &_1);
		zephir_check_call_status();
		ZEPHIR_INIT_LNVAR(_7);
		div_function(_7, _3, _5 TSRMLS_CC);
		item_psi = zephir_get_numberval(_7);
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, (k0 * x));
		ZEPHIR_CALL_FUNCTION(&_3, "cos", &_8, 11, &_1);
		zephir_check_call_status();
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_5, "exp", &_6, 1, &_1);
		zephir_check_call_status();
		ZEPHIR_INIT_LNVAR(_9);
		div_function(_9, _3, _5 TSRMLS_CC);
		item_psr = zephir_get_numberval(_9);
		ZEPHIR_INIT_NVAR(_10);
		zephir_create_array(_10, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, item_psr);
		zephir_array_fast_append(_10, _0);
		zephir_array_update_long(&psr, i, &_10, PH_COPY | PH_SEPARATE, "test/quantum.zep", 39);
		ZEPHIR_INIT_NVAR(_11);
		zephir_create_array(_11, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, item_psi);
		zephir_array_fast_append(_11, _0);
		zephir_array_update_long(&psi, i, &_11, PH_COPY | PH_SEPARATE, "test/quantum.zep", 40);
		ZEPHIR_INIT_NVAR(_12);
		zephir_create_array(_12, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, (double) ((double) (5.0 * x) * x));
		zephir_array_fast_append(_12, _0);
		zephir_array_update_long(&v, i, &_12, PH_COPY | PH_SEPARATE, "test/quantum.zep", 41);
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
	_15 = 20000;
	_14 = 0;
	_13 = 0;
	if (_14 <= _15) {
		while (1) {
			if (_13) {
				_14++;
				if (!(_14 <= _15)) {
					break;
				}
			} else {
				_13 = 1;
			}
			n = _14;
			_18 = (751 - 1);
			_17 = 1;
			_16 = 0;
			if (_17 <= _18) {
				while (1) {
					if (_16) {
						_17++;
						if (!(_17 <= _18)) {
							break;
						}
					} else {
						_16 = 1;
					}
					i = _17;
					zephir_array_fetch_long(&_19, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_20, _19, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_22, psi, (i + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _22, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_24, psi, (i - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_25, _24, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _23, _25 TSRMLS_CC);
					zephir_array_fetch_long(&_23, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_25, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_26, _25, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _23, _26 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_7);
					sub_function(_7, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _21, _7 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_23, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_7);
					mul_function(_7, _21, _23 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _7 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					sub_function(&_1, _20, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_20, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_21, v, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _21, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _20, _23 TSRMLS_CC);
					zephir_array_fetch_long(&_20, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _20, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_27);
					mul_function(&_27, &_2, _23 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_7);
					zephir_add_function_ex(_7, &_1, &_27 TSRMLS_CC);
					zephir_array_update_multi(&psr, &_7 TSRMLS_CC, SL("ll"), 2, i, 1);
					zephir_array_fetch_long(&_23, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_26, _23, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_28, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_29, _28, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _26, _29 TSRMLS_CC);
					zephir_array_fetch_long(&_26, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_29, _26, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_30, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_31, _30, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _29, _31 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_7);
					zephir_add_function_ex(_7, &_1, &_2 TSRMLS_CC);
					zephir_array_update_long(&p2, i, &_7, PH_COPY | PH_SEPARATE, "test/quantum.zep", 53);
				}
			}
			_34 = (751 - 1);
			_33 = 1;
			_32 = 0;
			if (_33 <= _34) {
				while (1) {
					if (_32) {
						_33++;
						if (!(_33 <= _34)) {
							break;
						}
					} else {
						_32 = 1;
					}
					j = _33;
					ZEPHIR_INIT_NVAR(_0);
					ZVAL_LONG(_0, 0);
					zephir_array_update_multi(&psr, &_0 TSRMLS_CC, SL("ll"), 2, 0, 1);
					ZEPHIR_INIT_NVAR(_0);
					ZVAL_LONG(_0, 0);
					zephir_array_update_multi(&psr, &_0 TSRMLS_CC, SL("ll"), 2, 751, 1);
					zephir_array_fetch_long(&_19, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_20, _19, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_22, psr, (j + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _22, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_24, psr, (j - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_25, _24, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _23, _25 TSRMLS_CC);
					zephir_array_fetch_long(&_23, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_25, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_26, _25, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _23, _26 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_9);
					sub_function(_9, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _21, _9 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_23, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_9);
					mul_function(_9, _21, _23 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _9 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _20, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_20, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_21, v, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _21, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _20, _23 TSRMLS_CC);
					zephir_array_fetch_long(&_20, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _20, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_27);
					mul_function(&_27, &_2, _23 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_9);
					sub_function(_9, &_1, &_27 TSRMLS_CC);
					zephir_array_update_multi(&psi, &_9 TSRMLS_CC, SL("ll"), 2, j, 1);
				}
			}
			_35 = n == 0;
			if (!(_35)) {
				_35 = zephir_safe_mod_long_long(n, 2000 TSRMLS_CC) == 0;
			}
			if (_35) {
				i = 1;
				while (1) {
					if (!(i < (751 - 1))) {
						break;
					}
					zephir_array_fetch_long(&_19, p2, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 67 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					ZVAL_STRING(&_1, "%16.8lf %16.8lf %16.8lf \n", 0);
					ZEPHIR_SINIT_NVAR(_2);
					ZVAL_DOUBLE(&_2, ((double) i * dx));
					ZEPHIR_SINIT_NVAR(_27);
					ZVAL_DOUBLE(&_27, ((double) n * dt));
					ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_36, 57, fp, &_1, &_2, &_27, _19);
					zephir_check_call_status();
					i = (i + 10);
				}
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_STRING(&_1, "\n", 0);
				ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_36, 57, fp, &_1);
				zephir_check_call_status();
			}
			j = 1;
			while (1) {
				if (!(j < (751 - 1))) {
					break;
				}
				zephir_array_fetch_long(&_19, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_fetch_long(&_20, _19, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_update_multi(&psi, &_20 TSRMLS_CC, SL("ll"), 2, j, 0);
				zephir_array_fetch_long(&_20, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_fetch_long(&_21, _20, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_update_multi(&psr, &_21 TSRMLS_CC, SL("ll"), 2, j, 0);
				j++;
			}
		}
	}
	RETURN_MM_BOOL(1);

}

