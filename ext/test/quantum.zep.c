
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

	zephir_declare_class_constant_double(test_quantum_ce, SL("PI"), 3.14159265358979323846);

	zephir_declare_class_constant_long(test_quantum_ce, SL("MAX"), 751);

	return SUCCESS;

}

PHP_METHOD(Test_Quantum, harmos) {

	zval _8$$4, _10$$4, _11$$4;
	zend_bool _12, _15$$5, _34$$5, _50$$5;
	zephir_fcall_cache_entry *_5 = NULL, *_55 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i = 0, j = 0, n = 0, _13, _14, _16$$5, _17$$5, _35$$5, _36$$5;
	zval *x_param = NULL, psr, psi, p2, v, paramater, fp, tmp, _0, _1, _2$$4, _3$$4, _4$$4, _6$$4, _7$$4, _9$$4, _18$$6, _19$$6, _20$$6, _21$$6, _22$$6, _23$$6, _24$$6, _25$$6, _26$$6, _27$$6, _28$$6, _29$$6, _30$$6, _31$$6, _32$$6, _33$$6, _37$$7, _38$$7, _39$$7, _40$$7, _41$$7, _42$$7, _43$$7, _44$$7, _45$$7, _46$$7, _47$$7, _48$$7, _49$$7, _51$$9, _52$$9, _53$$9, _54$$9, _56$$8, _57$$10, _58$$10, _59$$10;
	double x, dt = 0, dx = 0, k0 = 0, item_psr = 0, item_psi = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&psr);
	ZVAL_UNDEF(&psi);
	ZVAL_UNDEF(&p2);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&paramater);
	ZVAL_UNDEF(&fp);
	ZVAL_UNDEF(&tmp);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2$$4);
	ZVAL_UNDEF(&_3$$4);
	ZVAL_UNDEF(&_4$$4);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_18$$6);
	ZVAL_UNDEF(&_19$$6);
	ZVAL_UNDEF(&_20$$6);
	ZVAL_UNDEF(&_21$$6);
	ZVAL_UNDEF(&_22$$6);
	ZVAL_UNDEF(&_23$$6);
	ZVAL_UNDEF(&_24$$6);
	ZVAL_UNDEF(&_25$$6);
	ZVAL_UNDEF(&_26$$6);
	ZVAL_UNDEF(&_27$$6);
	ZVAL_UNDEF(&_28$$6);
	ZVAL_UNDEF(&_29$$6);
	ZVAL_UNDEF(&_30$$6);
	ZVAL_UNDEF(&_31$$6);
	ZVAL_UNDEF(&_32$$6);
	ZVAL_UNDEF(&_33$$6);
	ZVAL_UNDEF(&_37$$7);
	ZVAL_UNDEF(&_38$$7);
	ZVAL_UNDEF(&_39$$7);
	ZVAL_UNDEF(&_40$$7);
	ZVAL_UNDEF(&_41$$7);
	ZVAL_UNDEF(&_42$$7);
	ZVAL_UNDEF(&_43$$7);
	ZVAL_UNDEF(&_44$$7);
	ZVAL_UNDEF(&_45$$7);
	ZVAL_UNDEF(&_46$$7);
	ZVAL_UNDEF(&_47$$7);
	ZVAL_UNDEF(&_48$$7);
	ZVAL_UNDEF(&_49$$7);
	ZVAL_UNDEF(&_51$$9);
	ZVAL_UNDEF(&_52$$9);
	ZVAL_UNDEF(&_53$$9);
	ZVAL_UNDEF(&_54$$9);
	ZVAL_UNDEF(&_56$$8);
	ZVAL_UNDEF(&_57$$10);
	ZVAL_UNDEF(&_58$$10);
	ZVAL_UNDEF(&_59$$10);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_10$$4);
	ZVAL_UNDEF(&_11$$4);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &x_param);

	x = zephir_get_doubleval(x_param);


	dx = 0.02;
	k0 = (3.0 * 3.14159265358979323846);
	dt = zephir_safe_div_double_long((dx * dx), 4.0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&paramater);
	zephir_create_array(&paramater, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_DOUBLE(&_0, dx);
	zephir_array_fast_append(&paramater, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, k0);
	zephir_array_fast_append(&paramater, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, dt);
	zephir_array_fast_append(&paramater, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, x);
	zephir_array_fast_append(&paramater, &_0);
	i = 0;
	ZEPHIR_INIT_VAR(&psr);
	array_init(&psr);
	ZEPHIR_INIT_VAR(&psi);
	array_init(&psi);
	ZEPHIR_INIT_VAR(&p2);
	array_init(&p2);
	ZEPHIR_INIT_VAR(&v);
	array_init(&v);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "harmos.txt");
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "w");
	ZEPHIR_CALL_FUNCTION(&fp, "fopen", NULL, 30, &_0, &_1);
	zephir_check_call_status();
	if (!(zephir_is_true(&fp))) {
		RETURN_MM_LONG(1);
	}
	while (1) {
		if (!(i <= 751)) {
			break;
		}
		ZVAL_DOUBLE(&_2$$4, (k0 * x));
		ZVAL_DOUBLE(&_3$$4, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_4$$4, "exp", &_5, 2, &_3$$4);
		zephir_check_call_status();
		item_psi = zephir_safe_div_double_zval(sin((k0 * x)), &_4$$4 TSRMLS_CC);
		ZVAL_DOUBLE(&_3$$4, (k0 * x));
		ZVAL_DOUBLE(&_6$$4, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_7$$4, "exp", &_5, 2, &_6$$4);
		zephir_check_call_status();
		item_psr = zephir_safe_div_double_zval(cos((k0 * x)), &_7$$4 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(&_8$$4);
		zephir_create_array(&_8$$4, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(&_9$$4);
		ZVAL_DOUBLE(&_9$$4, item_psr);
		zephir_array_fast_append(&_8$$4, &_9$$4);
		zephir_array_update_long(&psr, i, &_8$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		ZEPHIR_INIT_NVAR(&_10$$4);
		zephir_create_array(&_10$$4, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(&_9$$4);
		ZVAL_DOUBLE(&_9$$4, item_psi);
		zephir_array_fast_append(&_10$$4, &_9$$4);
		zephir_array_update_long(&psi, i, &_10$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		ZEPHIR_INIT_NVAR(&_11$$4);
		zephir_create_array(&_11$$4, 1, 0 TSRMLS_CC);
		ZEPHIR_INIT_NVAR(&_9$$4);
		ZVAL_DOUBLE(&_9$$4, (double) ((double) (5.0 * x) * x));
		zephir_array_fast_append(&_11$$4, &_9$$4);
		zephir_array_update_long(&v, i, &_11$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		x = (x + dx);
		i++;
	}
	i = 1;
	j = 1;
	ZEPHIR_INIT_VAR(&tmp);
	zephir_create_array(&tmp, 1, 0 TSRMLS_CC);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 2.0);
	zephir_array_fast_append(&tmp, &_0);
	_14 = 20000;
	_13 = 0;
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
			n = _13;
			_17$$5 = (751 - 1);
			_16$$5 = 1;
			_15$$5 = 0;
			if (_16$$5 <= _17$$5) {
				while (1) {
					if (_15$$5) {
						_16$$5++;
						if (!(_16$$5 <= _17$$5)) {
							break;
						}
					} else {
						_15$$5 = 1;
					}
					i = _16$$5;
					zephir_array_fetch_long(&_18$$6, &psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_19$$6, &_18$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_20$$6, &paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_21$$6, &psi, (i + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_22$$6, &_21$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_23$$6, &psi, (i - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 50 TSRMLS_CC);
					zephir_array_fetch_long(&_24$$6, &_23$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_25$$6);
					zephir_add_function(&_25$$6, &_22$$6, &_24$$6);
					zephir_array_fetch_long(&_22$$6, &tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_24$$6, &psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_26$$6, &_24$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_27$$6);
					mul_function(&_27$$6, &_22$$6, &_26$$6 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_28$$6);
					zephir_sub_function(&_28$$6, &_25$$6, &_27$$6);
					ZEPHIR_SINIT_NVAR(_25$$6);
					mul_function(&_25$$6, &_20$$6, &_28$$6 TSRMLS_CC);
					zephir_array_fetch_long(&_20$$6, &paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_22$$6, &paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_28$$6);
					mul_function(&_28$$6, &_20$$6, &_22$$6 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_27$$6);
					div_function(&_27$$6, &_25$$6, &_28$$6 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_25$$6);
					zephir_sub_function(&_25$$6, &_19$$6, &_27$$6);
					zephir_array_fetch_long(&_19$$6, &paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_20$$6, &v, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_22$$6, &_20$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_27$$6);
					mul_function(&_27$$6, &_19$$6, &_22$$6 TSRMLS_CC);
					zephir_array_fetch_long(&_19$$6, &psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					zephir_array_fetch_long(&_22$$6, &_19$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 51 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_29$$6);
					mul_function(&_29$$6, &_27$$6, &_22$$6 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_28$$6);
					zephir_add_function(&_28$$6, &_25$$6, &_29$$6);
					zephir_array_update_multi(&psr, &_28$$6 TSRMLS_CC, SL("ll"), 2, i, 1);
					zephir_array_fetch_long(&_22$$6, &psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_26$$6, &_22$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_30$$6, &psr, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_31$$6, &_30$$6, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_25$$6);
					mul_function(&_25$$6, &_26$$6, &_31$$6 TSRMLS_CC);
					zephir_array_fetch_long(&_26$$6, &psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_31$$6, &_26$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_32$$6, &psi, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					zephir_array_fetch_long(&_33$$6, &_32$$6, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 53 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_27$$6);
					mul_function(&_27$$6, &_31$$6, &_33$$6 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_28$$6);
					zephir_add_function(&_28$$6, &_25$$6, &_27$$6);
					zephir_array_update_long(&p2, i, &_28$$6, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
				}
			}
			_36$$5 = (751 - 1);
			_35$$5 = 1;
			_34$$5 = 0;
			if (_35$$5 <= _36$$5) {
				while (1) {
					if (_34$$5) {
						_35$$5++;
						if (!(_35$$5 <= _36$$5)) {
							break;
						}
					} else {
						_34$$5 = 1;
					}
					j = _35$$5;
					ZEPHIR_INIT_NVAR(&_37$$7);
					ZVAL_LONG(&_37$$7, 0);
					zephir_array_update_multi(&psr, &_37$$7 TSRMLS_CC, SL("ll"), 2, 0, 1);
					ZEPHIR_INIT_NVAR(&_37$$7);
					ZVAL_LONG(&_37$$7, 0);
					zephir_array_update_multi(&psr, &_37$$7 TSRMLS_CC, SL("ll"), 2, 751, 1);
					zephir_array_fetch_long(&_38$$7, &psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_39$$7, &_38$$7, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_40$$7, &paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_41$$7, &psr, (j + 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_42$$7, &_41$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_43$$7, &psr, (j - 1), PH_NOISY | PH_READONLY, "test/quantum.zep", 58 TSRMLS_CC);
					zephir_array_fetch_long(&_44$$7, &_43$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_45$$7);
					zephir_add_function(&_45$$7, &_42$$7, &_44$$7);
					zephir_array_fetch_long(&_42$$7, &tmp, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_44$$7, &psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_46$$7, &_44$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_47$$7);
					mul_function(&_47$$7, &_42$$7, &_46$$7 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_48$$7);
					zephir_sub_function(&_48$$7, &_45$$7, &_47$$7);
					ZEPHIR_SINIT_NVAR(_45$$7);
					mul_function(&_45$$7, &_40$$7, &_48$$7 TSRMLS_CC);
					zephir_array_fetch_long(&_40$$7, &paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_42$$7, &paramater, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_48$$7);
					mul_function(&_48$$7, &_40$$7, &_42$$7 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_47$$7);
					div_function(&_47$$7, &_45$$7, &_48$$7 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_45$$7);
					zephir_add_function(&_45$$7, &_39$$7, &_47$$7);
					zephir_array_fetch_long(&_39$$7, &paramater, 2, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_40$$7, &v, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_42$$7, &_40$$7, 0, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_47$$7);
					mul_function(&_47$$7, &_39$$7, &_42$$7 TSRMLS_CC);
					zephir_array_fetch_long(&_39$$7, &psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					zephir_array_fetch_long(&_42$$7, &_39$$7, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 59 TSRMLS_CC);
					ZEPHIR_SINIT_NVAR(_49$$7);
					mul_function(&_49$$7, &_47$$7, &_42$$7 TSRMLS_CC);
					ZEPHIR_INIT_LNVAR(_48$$7);
					zephir_sub_function(&_48$$7, &_45$$7, &_49$$7);
					zephir_array_update_multi(&psi, &_48$$7 TSRMLS_CC, SL("ll"), 2, j, 1);
				}
			}
			_50$$5 = n == 0;
			if (!(_50$$5)) {
				_50$$5 = zephir_safe_mod_long_long(n, 2000 TSRMLS_CC) == 0;
			}
			if (_50$$5) {
				i = 1;
				while (1) {
					if (!(i < (751 - 1))) {
						break;
					}
					zephir_array_fetch_long(&_51$$9, &p2, i, PH_NOISY | PH_READONLY, "test/quantum.zep", 67 TSRMLS_CC);
					ZEPHIR_INIT_NVAR(&_52$$9);
					ZVAL_STRING(&_52$$9, "%16.8lf %16.8lf %16.8lf \n");
					ZVAL_DOUBLE(&_53$$9, ((double) i * dx));
					ZVAL_DOUBLE(&_54$$9, ((double) n * dt));
					ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_55, 56, &fp, &_52$$9, &_53$$9, &_54$$9, &_51$$9);
					zephir_check_call_status();
					i = (i + 10);
				}
				ZEPHIR_INIT_NVAR(&_56$$8);
				ZVAL_STRING(&_56$$8, "\n");
				ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_55, 56, &fp, &_56$$8);
				zephir_check_call_status();
			}
			j = 1;
			while (1) {
				if (!(j < (751 - 1))) {
					break;
				}
				zephir_array_fetch_long(&_57$$10, &psi, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_fetch_long(&_58$$10, &_57$$10, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 76 TSRMLS_CC);
				zephir_array_update_multi(&psi, &_58$$10 TSRMLS_CC, SL("ll"), 2, j, 0);
				zephir_array_fetch_long(&_58$$10, &psr, j, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_fetch_long(&_59$$10, &_58$$10, 1, PH_NOISY | PH_READONLY, "test/quantum.zep", 77 TSRMLS_CC);
				zephir_array_update_multi(&psr, &_59$$10 TSRMLS_CC, SL("ll"), 2, j, 0);
				j++;
			}
		}
	}
	RETURN_MM_BOOL(1);

}

