
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

	zval *_9$$4 = NULL, *_11$$4 = NULL, *_12$$4 = NULL;
	zend_bool _13, _16$$5, _35$$5, _51$$5;
	zephir_fcall_cache_entry *_6 = NULL, *_56 = NULL;
	int ZEPHIR_LAST_CALL_STATUS, i = 0, j = 0, n = 0, _14, _15, _17$$5, _18$$5, _36$$5, _37$$5;
	zval *x_param = NULL, *psr = NULL, *psi = NULL, *p2 = NULL, *v = NULL, *paramater = NULL, *fp = NULL, *tmp = NULL, *_0 = NULL, _1, _2, *_3$$4 = NULL, _4$$4 = zval_used_for_init, *_5$$4 = NULL, *_7$$4 = NULL, *_8$$4 = NULL, *_10$$4 = NULL, *_19$$6, *_20$$6, *_21$$6, *_22$$6, *_23$$6, *_24$$6, *_25$$6, _26$$6 = zval_used_for_init, *_27$$6, _28$$6 = zval_used_for_init, *_29$$6 = NULL, _30$$6 = zval_used_for_init, *_31$$6, *_32$$6, *_33$$6, *_34$$6, *_38$$7 = NULL, *_39$$7, *_40$$7, *_41$$7, *_42$$7, *_43$$7, *_44$$7, *_45$$7, _46$$7 = zval_used_for_init, *_47$$7, _48$$7 = zval_used_for_init, *_49$$7 = NULL, _50$$7 = zval_used_for_init, *_52$$9, _53$$9 = zval_used_for_init, _54$$9 = zval_used_for_init, _55$$9 = zval_used_for_init, _57$$8 = zval_used_for_init, *_58$$10, *_59$$10, *_60$$10;
	double x, dt = 0, dx = 0, k0 = 0, item_psr = 0, item_psi = 0;

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
	ZEPHIR_CALL_FUNCTION(&fp, "fopen", NULL, 26, &_1, &_2);
	zephir_check_call_status();
	if (!(zephir_is_true(fp))) {
		RETURN_MM_LONG(1);
	}
	while (1) {
		if (!(i <= 751)) {
			break;
		}
		ZEPHIR_INIT_NVAR(_3$$4);
		ZVAL_DOUBLE(_3$$4, (k0 * x));
		ZEPHIR_SINIT_NVAR(_4$$4);
		ZVAL_DOUBLE(&_4$$4, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_5$$4, "exp", &_6, 1, &_4$$4);
		zephir_check_call_status();
		item_psi = zephir_safe_div_double_zval(sin((k0 * x)), _5$$4 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_7$$4);
		ZVAL_DOUBLE(_7$$4, (k0 * x));
		ZEPHIR_SINIT_NVAR(_4$$4);
		ZVAL_DOUBLE(&_4$$4, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_8$$4, "exp", &_6, 1, &_4$$4);
		zephir_check_call_status();
		item_psr = zephir_safe_div_double_zval(cos((k0 * x)), _8$$4 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_9$$4);
		zephir_create_array(_9$$4, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_10$$4);
		ZVAL_DOUBLE(_10$$4, item_psr);
		zephir_array_fast_append(_9$$4, _10$$4);
		zephir_array_update_long(&psr, i, &_9$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		ZEPHIR_INIT_NVAR(_11$$4);
		zephir_create_array(_11$$4, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_10$$4);
		ZVAL_DOUBLE(_10$$4, item_psi);
		zephir_array_fast_append(_11$$4, _10$$4);
		zephir_array_update_long(&psi, i, &_11$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		ZEPHIR_INIT_NVAR(_12$$4);
		zephir_create_array(_12$$4, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(_10$$4);
		ZVAL_DOUBLE(_10$$4, (double) ((double) (5.0 * x) * x));
		zephir_array_fast_append(_12$$4, _10$$4);
		zephir_array_update_long(&v, i, &_12$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
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
			_18$$5 = (751 - 1);
			_17$$5 = 1;
			_16$$5 = 0;
			if (_17$$5 <= _18$$5) {
				while (1) {
					if (_16$$5) {
						_17$$5++;
						if (!(_17$$5 <= _18$$5)) {
							break;
						}
					} else {
						_16$$5 = 1;
					}
					i = _17$$5;
					zephir_array_fetch_long(&_19$$6, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_20$$6, _19$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_21$$6, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_22$$6, psi, (i + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_23$$6, _22$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_24$$6, psi, (i - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_25$$6, _24$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_26$$6);
					zephir_add_function(&_26$$6, _23$$6, _25$$6);
					zephir_array_fetch_long(&_23$$6, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_25$$6, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_27$$6, _25$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_28$$6);
					mul_function(&_28$$6, _23$$6, _27$$6 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_29$$6);
					zephir_sub_function(_29$$6, &_26$$6, &_28$$6);
					ZEPHIR_SINIT_NVAR(_26$$6);
					mul_function(&_26$$6, _21$$6, _29$$6 TSRMLS_CC);
					zephir_array_fetch_long(&_21$$6, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_23$$6, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_29$$6);
					mul_function(_29$$6, _21$$6, _23$$6 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_28$$6);
					div_function(&_28$$6, &_26$$6, _29$$6 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_26$$6);
					zephir_sub_function(&_26$$6, _20$$6, &_28$$6);
					zephir_array_fetch_long(&_20$$6, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_21$$6, v, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_23$$6, _21$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_28$$6);
					mul_function(&_28$$6, _20$$6, _23$$6 TSRMLS_CC);
					zephir_array_fetch_long(&_20$$6, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_23$$6, _20$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_30$$6);
					mul_function(&_30$$6, &_28$$6, _23$$6 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_29$$6);
					zephir_add_function(_29$$6, &_26$$6, &_30$$6);
					zephir_array_update_multi(&psr, &_29$$6 TSRMLS_CC, SL("ll"), 2, i, 1);
					zephir_array_fetch_long(&_23$$6, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_27$$6, _23$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_31$$6, psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_32$$6, _31$$6, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_26$$6);
					mul_function(&_26$$6, _27$$6, _32$$6 TSRMLS_CC);
					zephir_array_fetch_long(&_27$$6, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_32$$6, _27$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_33$$6, psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_34$$6, _33$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_28$$6);
					mul_function(&_28$$6, _32$$6, _34$$6 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_29$$6);
					zephir_add_function(_29$$6, &_26$$6, &_28$$6);
					zephir_array_update_long(&p2, i, &_29$$6, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
				}
			}
			_37$$5 = (751 - 1);
			_36$$5 = 1;
			_35$$5 = 0;
			if (_36$$5 <= _37$$5) {
				while (1) {
					if (_35$$5) {
						_36$$5++;
						if (!(_36$$5 <= _37$$5)) {
							break;
						}
					} else {
						_35$$5 = 1;
					}
					j = _36$$5;
					ZEPHIR_INIT_NVAR(_38$$7);
					ZVAL_LONG(_38$$7, 0);
					zephir_array_update_multi(&psr, &_38$$7 TSRMLS_CC, SL("ll"), 2, 0, 1);
					ZEPHIR_INIT_NVAR(_38$$7);
					ZVAL_LONG(_38$$7, 0);
					zephir_array_update_multi(&psr, &_38$$7 TSRMLS_CC, SL("ll"), 2, 751, 1);
					zephir_array_fetch_long(&_39$$7, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_40$$7, _39$$7, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_41$$7, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_42$$7, psr, (j + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_43$$7, _42$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_44$$7, psr, (j - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_45$$7, _44$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_46$$7);
					zephir_add_function(&_46$$7, _43$$7, _45$$7);
					zephir_array_fetch_long(&_43$$7, tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_45$$7, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_47$$7, _45$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_48$$7);
					mul_function(&_48$$7, _43$$7, _47$$7 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_49$$7);
					zephir_sub_function(_49$$7, &_46$$7, &_48$$7);
					ZEPHIR_SINIT_NVAR(_46$$7);
					mul_function(&_46$$7, _41$$7, _49$$7 TSRMLS_CC);
					zephir_array_fetch_long(&_41$$7, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_43$$7, paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_49$$7);
					mul_function(_49$$7, _41$$7, _43$$7 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_48$$7);
					div_function(&_48$$7, &_46$$7, _49$$7 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_46$$7);
					zephir_add_function(&_46$$7, _40$$7, &_48$$7);
					zephir_array_fetch_long(&_40$$7, paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_41$$7, v, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_43$$7, _41$$7, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_48$$7);
					mul_function(&_48$$7, _40$$7, _43$$7 TSRMLS_CC);
					zephir_array_fetch_long(&_40$$7, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_43$$7, _40$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_50$$7);
					mul_function(&_50$$7, &_48$$7, _43$$7 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_49$$7);
					zephir_sub_function(_49$$7, &_46$$7, &_50$$7);
					zephir_array_update_multi(&psi, &_49$$7 TSRMLS_CC, SL("ll"), 2, j, 1);
				}
			}
			_51$$5 = n == 0;
			if (!(_51$$5)) {
				_51$$5 = zephir_safe_mod_long_long(n, 2000 TSRMLS_CC) == 0;
			}
			if (_51$$5) {
				i = 1;
				while (1) {
					if (!(i < (751 - 1))) {
						break;
					}
					zephir_array_fetch_long(&_52$$9, p2, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 67 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_53$$9);
					ZVAL_STRING(&_53$$9, "%16.8lf %16.8lf %16.8lf \n", 0);
					ZEPHIR_SINIT_NVAR(_54$$9);
					ZVAL_DOUBLE(&_54$$9, ((double) i * dx));
					ZEPHIR_SINIT_NVAR(_55$$9);
					ZVAL_DOUBLE(&_55$$9, ((double) n * dt));
					ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_56, 54, fp, &_53$$9, &_54$$9, &_55$$9, _52$$9);
					zephir_check_call_status();
					i = (i + 10);
				}
				ZEPHIR_SINIT_NVAR(_57$$8);
				ZVAL_STRING(&_57$$8, "\n", 0);
				ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_56, 54, fp, &_57$$8);
				zephir_check_call_status();
			}
			j = 1;
			while (1) {
				if (!(j < (751 - 1))) {
					break;
				}
				zephir_array_fetch_long(&_58$$10, psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_fetch_long(&_59$$10, _58$$10, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_update_multi(&psi, &_59$$10 TSRMLS_CC, SL("ll"), 2, j, 0);
				zephir_array_fetch_long(&_59$$10, psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_fetch_long(&_60$$10, _59$$10, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_update_multi(&psr, &_60$$10 TSRMLS_CC, SL("ll"), 2, j, 0);
				j++;
			}
		}
	}
	RETURN_MM_BOOL(1);

}

