
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
#include "math.h"


ZEPHIR_INIT_CLASS(Test_Quantum) {

	ZEPHIR_REGISTER_CLASS(Test, Quantum, test, quantum, test_quantum_method_entry, 0);

	zend_declare_class_constant_double(test_quantum_ce, SL("PI"), 3.14159265358979323846 TSRMLS_CC);

	zend_declare_class_constant_long(test_quantum_ce, SL("MAX"), 751 TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Quantum, harmos) {

	zend_bool _11, _14, _31, _34;
	zval *_7 = NULL, *_9 = NULL, *_10 = NULL;
	zephir_fcall_cache_entry *_4 = NULL, *_35 = NULL;
	int i, j, n, ZEPHIR_LAST_CALL_STATUS, _12, _13, _15, _16, _32, _33;
	zval *x_param = NULL, *psr, *psi, *p2, *v, *paramater, *fp = NULL, *tmp, *_0 = NULL, _1 = zval_used_for_init, _2 = zval_used_for_init, *_3 = NULL, *_5 = NULL, *_6 = NULL, *_8 = NULL, *_17, *_18, *_19, *_20, *_21, *_22, *_23, *_24, *_25 = NULL, _26 = zval_used_for_init, *_27, *_28, *_29, *_30;
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
	ZEPHIR_CALL_FUNCTION(&fp, "fopen", NULL, 24, &_1, &_2);
	zephir_check_call_status();
	if (!(zephir_is_true(fp))) {
		RETURN_MM_LONG(1);
	}
	while (1) {
		if (!(i <= 751)) {
			break;
		}
		ZEPHIR_INIT_NVAR(_0);
		ZVAL_DOUBLE(_0, (k0 * x));
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_3, "exp", &_4, 1, &_1);
		zephir_check_call_status();
		item_psi = zephir_safe_div_double_zval(sin((k0 * x)), _3 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_5);
		ZVAL_DOUBLE(_5, (k0 * x));
		ZEPHIR_SINIT_NVAR(_1);
		ZVAL_DOUBLE(&_1, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_6, "exp", &_4, 1, &_1);
		zephir_check_call_status();
		item_psr = zephir_safe_div_double_zval(cos((k0 * x)), _6 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_7);
		zephir_create_array(_7, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_8);
		ZVAL_DOUBLE(_8, item_psr);
		zephir_array_fast_append(_7, _8);
		zephir_array_update_long(&psr, i, &_7, PH_COPY | PH_SEPARATE, "test/quantum.zep", 39);
		ZEPHIR_INIT_NVAR(_9);
		zephir_create_array(_9, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_8);
		ZVAL_DOUBLE(_8, item_psi);
		zephir_array_fast_append(_9, _8);
		zephir_array_update_long(&psi, i, &_9, PH_COPY | PH_SEPARATE, "test/quantum.zep", 40);
		ZEPHIR_INIT_NVAR(_10);
		zephir_create_array(_10, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_8);
		ZVAL_DOUBLE(_8, (double) ((double) (5.0 * x) * x));
		zephir_array_fast_append(_10, _8);
		zephir_array_update_long(&v, i, &_10, PH_COPY | PH_SEPARATE, "test/quantum.zep", 41);
		x = (x + dx);
		i++;
	}
	i = 1;
	j = 1;
	ZEPHIR_INIT_VAR(tmp);
	zephir_create_array(tmp, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(_5);
	ZVAL_DOUBLE(_5, 2.0);
	zephir_array_fast_append(tmp, _5);
	_13 = 20000;
	_12 = 0;
	_11 = 0;
	if (_12 <= _13) {
		while (1) {
			if (_11) {
				_12++;
				if (!(_12 <= _13)) {
					break;
				}
			} else {
				_11 = 1;
			}
			n = _12;
			_16 = (751 - 1);
			_15 = 1;
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
					i = _15;
					zephir_array_fetch_long(&_17, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_18, _17, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_19, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_20, psi, (i + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _20, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_22, psi, (i - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _22, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _21, _23 TSRMLS_CC);
					zephir_array_fetch_long(&_21, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_23, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _23, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _21, _24 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_25);
					sub_function(_25, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _19, _25 TSRMLS_CC);
					zephir_array_fetch_long(&_19, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_25);
					mul_function(_25, _19, _21 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _25 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					sub_function(&_1, _18, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_18, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_19, v, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _19, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _18, _21 TSRMLS_CC);
					zephir_array_fetch_long(&_18, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _18, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_26);
					mul_function(&_26, &_2, _21 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_25);
					zephir_add_function_ex(_25, &_1, &_26 TSRMLS_CC);
					zephir_array_update_multi(&psr, &_25 TSRMLS_CC, SL("ll"), 2, i, 1);
					zephir_array_fetch_long(&_21, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _21, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_27, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_28, _27, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _24, _28 TSRMLS_CC);
					zephir_array_fetch_long(&_24, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_28, _24, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_29, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_30, _29, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _28, _30 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_25);
					zephir_add_function_ex(_25, &_1, &_2 TSRMLS_CC);
					zephir_array_update_long(&p2, i, &_25, PH_COPY | PH_SEPARATE, "test/quantum.zep", 53);
				}
			}
			_33 = (751 - 1);
			_32 = 1;
			_31 = 0;
			if (_32 <= _33) {
				while (1) {
					if (_31) {
						_32++;
						if (!(_32 <= _33)) {
							break;
						}
					} else {
						_31 = 1;
					}
					j = _32;
					ZEPHIR_INIT_NVAR(_5);
					ZVAL_LONG(_5, 0);
					zephir_array_update_multi(&psr, &_5 TSRMLS_CC, SL("ll"), 2, 0, 1);
					ZEPHIR_INIT_NVAR(_5);
					ZVAL_LONG(_5, 0);
					zephir_array_update_multi(&psr, &_5 TSRMLS_CC, SL("ll"), 2, 751, 1);
					zephir_array_fetch_long(&_17, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_18, _17, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_19, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_20, psr, (j + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _20, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_22, psr, (j - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_23, _22, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _21, _23 TSRMLS_CC);
					zephir_array_fetch_long(&_21, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_23, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_24, _23, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _21, _24 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_25);
					sub_function(_25, &_1, &_2 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					mul_function(&_1, _19, _25 TSRMLS_CC);
					zephir_array_fetch_long(&_19, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_21, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_25);
					mul_function(_25, _19, _21 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					div_function(&_2, &_1, _25 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					zephir_add_function_ex(&_1, _18, &_2 TSRMLS_CC);
					zephir_array_fetch_long(&_18, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_19, v, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _19, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_2);
					mul_function(&_2, _18, _21 TSRMLS_CC);
					zephir_array_fetch_long(&_18, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_21, _18, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_26);
					mul_function(&_26, &_2, _21 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_25);
					sub_function(_25, &_1, &_26 TSRMLS_CC);
					zephir_array_update_multi(&psi, &_25 TSRMLS_CC, SL("ll"), 2, j, 1);
				}
			}
			_34 = n == 0;
			if (!(_34)) {
				_34 = zephir_safe_mod_long_long(n, 2000 TSRMLS_CC) == 0;
			}
			if (_34) {
				i = 1;
				while (1) {
					if (!(i < (751 - 1))) {
						break;
					}
					zephir_array_fetch_long(&_17, p2, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 67 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_1);
					ZVAL_STRING(&_1, "%16.8lf %16.8lf %16.8lf \n", 0);
					ZEPHIR_SINIT_NVAR(_2);
					ZVAL_DOUBLE(&_2, ((double) i * dx));
					ZEPHIR_SINIT_NVAR(_26);
					ZVAL_DOUBLE(&_26, ((double) n * dt));
					ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_35, 51, fp, &_1, &_2, &_26, _17);
					zephir_check_call_status();
					i = (i + 10);
				}
				ZEPHIR_SINIT_NVAR(_1);
				ZVAL_STRING(&_1, "\n", 0);
				ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_35, 51, fp, &_1);
				zephir_check_call_status();
			}
			j = 1;
			while (1) {
				if (!(j < (751 - 1))) {
					break;
				}
				zephir_array_fetch_long(&_17, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_fetch_long(&_18, _17, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_update_multi(&psi, &_18 TSRMLS_CC, SL("ll"), 2, j, 0);
				zephir_array_fetch_long(&_18, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_fetch_long(&_19, _18, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_update_multi(&psr, &_19 TSRMLS_CC, SL("ll"), 2, j, 0);
				j++;
			}
		}
	}
	RETURN_MM_BOOL(1);

}

