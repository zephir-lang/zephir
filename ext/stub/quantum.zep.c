
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"
#include "math.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Quantum)
{
	ZEPHIR_REGISTER_CLASS(Stub, Quantum, stub, quantum, stub_quantum_method_entry, 0);

	zephir_declare_class_constant_double(stub_quantum_ce, SL("PI"), 3.14159265358979323846);

	zephir_declare_class_constant_long(stub_quantum_ce, SL("MAX"), 751);

	return SUCCESS;
}

PHP_METHOD(Stub_Quantum, harmos)
{
	zval _9$$4, _11$$4, _12$$4;
	zend_bool _13, _16$$5, _36$$5, _53$$5;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_5 = NULL, *_58 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, i = 0, j = 0, n = 0, _14, _15, _17$$5, _18$$5, _37$$5, _38$$5;
	zval *x_param = NULL, psr, psi, p2, v, paramater, fp, tmp, _0, _1, _2$$4, _3$$4, _4$$4, _6$$4, _7$$4, _8$$4, _10$$4, _19$$6, _20$$6, _21$$6, _22$$6, _23$$6, _24$$6, _25$$6, _26$$6, _27$$6, _28$$6, _29$$6, _30$$6, _31$$6, _32$$6, _33$$6, _34$$6, _35$$6, _39$$7, _40$$7, _41$$7, _42$$7, _43$$7, _44$$7, _45$$7, _46$$7, _47$$7, _48$$7, _49$$7, _50$$7, _51$$7, _52$$7, _54$$9, _55$$9, _56$$9, _57$$9, _59$$8, _60$$10, _61$$10, _62$$10;
	double x, dt = 0, dx = 0, k0 = 0, item_psr = 0, item_psi = 0;

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
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_10$$4);
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
	ZVAL_UNDEF(&_34$$6);
	ZVAL_UNDEF(&_35$$6);
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
	ZVAL_UNDEF(&_50$$7);
	ZVAL_UNDEF(&_51$$7);
	ZVAL_UNDEF(&_52$$7);
	ZVAL_UNDEF(&_54$$9);
	ZVAL_UNDEF(&_55$$9);
	ZVAL_UNDEF(&_56$$9);
	ZVAL_UNDEF(&_57$$9);
	ZVAL_UNDEF(&_59$$8);
	ZVAL_UNDEF(&_60$$10);
	ZVAL_UNDEF(&_61$$10);
	ZVAL_UNDEF(&_62$$10);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_11$$4);
	ZVAL_UNDEF(&_12$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_DOUBLE(x)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &x_param);
	dx =  (0.02);
	k0 =  ((3.0 * 3.14159265358979323846));
	dt =  (zephir_safe_div_double_double((dx * dx), 4.0));
	ZEPHIR_INIT_VAR(&paramater);
	zephir_create_array(&paramater, 4, 0);
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
	ZEPHIR_CALL_FUNCTION(&fp, "fopen", NULL, 46, &_0, &_1);
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
		ZEPHIR_CALL_FUNCTION(&_4$$4, "exp", &_5, 3, &_3$$4);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(&_6$$4);
		zephir_div_double_zval(&_6$$4, sin((k0 * x)), &_4$$4);
		item_psi = zephir_get_numberval(&_6$$4);
		ZVAL_DOUBLE(&_3$$4, (k0 * x));
		ZVAL_DOUBLE(&_7$$4, ((x * x) * 2.0));
		ZEPHIR_CALL_FUNCTION(&_4$$4, "exp", &_5, 3, &_7$$4);
		zephir_check_call_status();
		ZEPHIR_INIT_NVAR(&_8$$4);
		zephir_div_double_zval(&_8$$4, cos((k0 * x)), &_4$$4);
		item_psr = zephir_get_numberval(&_8$$4);
		ZEPHIR_INIT_NVAR(&_9$$4);
		zephir_create_array(&_9$$4, 1, 0);
		ZEPHIR_INIT_NVAR(&_10$$4);
		ZVAL_DOUBLE(&_10$$4, item_psr);
		zephir_array_fast_append(&_9$$4, &_10$$4);
		zephir_array_update_long(&psr, i, &_9$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		ZEPHIR_INIT_NVAR(&_11$$4);
		zephir_create_array(&_11$$4, 1, 0);
		ZEPHIR_INIT_NVAR(&_10$$4);
		ZVAL_DOUBLE(&_10$$4, item_psi);
		zephir_array_fast_append(&_11$$4, &_10$$4);
		zephir_array_update_long(&psi, i, &_11$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		ZEPHIR_INIT_NVAR(&_12$$4);
		zephir_create_array(&_12$$4, 1, 0);
		ZEPHIR_INIT_NVAR(&_10$$4);
		ZVAL_DOUBLE(&_10$$4, (double) ((double) (5.0 * x) * x));
		zephir_array_fast_append(&_12$$4, &_10$$4);
		zephir_array_update_long(&v, i, &_12$$4, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
		x =  ((x + dx));
		i++;
	}
	i = 1;
	j = 1;
	ZEPHIR_INIT_VAR(&tmp);
	zephir_create_array(&tmp, 1, 0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_DOUBLE(&_0, 2.0);
	zephir_array_fast_append(&tmp, &_0);
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
					zephir_array_fetch_long(&_19$$6, &psr, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 50);
					ZEPHIR_OBS_NVAR(&_20$$6);
					zephir_array_fetch_long(&_20$$6, &_19$$6, 0, PH_NOISY, "stub/quantum.zep", 50);
					zephir_array_fetch_long(&_21$$6, &paramater, 2, PH_NOISY | PH_READONLY, "stub/quantum.zep", 50);
					zephir_array_fetch_long(&_22$$6, &psi, (i + 1), PH_NOISY | PH_READONLY, "stub/quantum.zep", 50);
					ZEPHIR_OBS_NVAR(&_23$$6);
					zephir_array_fetch_long(&_23$$6, &_22$$6, 0, PH_NOISY, "stub/quantum.zep", 50);
					zephir_array_fetch_long(&_24$$6, &psi, (i - 1), PH_NOISY | PH_READONLY, "stub/quantum.zep", 50);
					ZEPHIR_OBS_NVAR(&_25$$6);
					zephir_array_fetch_long(&_25$$6, &_24$$6, 0, PH_NOISY, "stub/quantum.zep", 51);
					ZEPHIR_INIT_NVAR(&_26$$6);
					zephir_add_function(&_26$$6, &_23$$6, &_25$$6);
					zephir_array_fetch_long(&_27$$6, &tmp, 0, PH_NOISY | PH_READONLY, "stub/quantum.zep", 51);
					zephir_array_fetch_long(&_28$$6, &psi, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 51);
					ZEPHIR_OBS_NVAR(&_23$$6);
					zephir_array_fetch_long(&_23$$6, &_28$$6, 0, PH_NOISY, "stub/quantum.zep", 51);
					ZEPHIR_INIT_NVAR(&_29$$6);
					mul_function(&_29$$6, &_27$$6, &_23$$6);
					ZEPHIR_INIT_NVAR(&_30$$6);
					zephir_sub_function(&_30$$6, &_26$$6, &_29$$6);
					ZEPHIR_INIT_NVAR(&_26$$6);
					mul_function(&_26$$6, &_21$$6, &_30$$6);
					zephir_array_fetch_long(&_21$$6, &paramater, 0, PH_NOISY | PH_READONLY, "stub/quantum.zep", 51);
					zephir_array_fetch_long(&_27$$6, &paramater, 0, PH_NOISY | PH_READONLY, "stub/quantum.zep", 51);
					ZEPHIR_INIT_NVAR(&_30$$6);
					mul_function(&_30$$6, &_21$$6, &_27$$6);
					ZEPHIR_INIT_NVAR(&_29$$6);
					div_function(&_29$$6, &_26$$6, &_30$$6);
					ZEPHIR_INIT_NVAR(&_26$$6);
					zephir_sub_function(&_26$$6, &_20$$6, &_29$$6);
					zephir_array_fetch_long(&_21$$6, &paramater, 2, PH_NOISY | PH_READONLY, "stub/quantum.zep", 51);
					zephir_array_fetch_long(&_27$$6, &v, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 51);
					ZEPHIR_OBS_NVAR(&_20$$6);
					zephir_array_fetch_long(&_20$$6, &_27$$6, 0, PH_NOISY, "stub/quantum.zep", 51);
					ZEPHIR_INIT_NVAR(&_29$$6);
					mul_function(&_29$$6, &_21$$6, &_20$$6);
					zephir_array_fetch_long(&_21$$6, &psi, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 51);
					ZEPHIR_OBS_NVAR(&_20$$6);
					zephir_array_fetch_long(&_20$$6, &_21$$6, 0, PH_NOISY, "stub/quantum.zep", 51);
					ZEPHIR_INIT_NVAR(&_31$$6);
					mul_function(&_31$$6, &_29$$6, &_20$$6);
					ZEPHIR_INIT_NVAR(&_30$$6);
					zephir_add_function(&_30$$6, &_26$$6, &_31$$6);
					zephir_array_update_multi(&psr, &_30$$6, SL("ll"), 2, (zend_long) i, (zend_long) 1);
					zephir_array_fetch_long(&_32$$6, &psr, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 53);
					ZEPHIR_OBS_NVAR(&_20$$6);
					zephir_array_fetch_long(&_20$$6, &_32$$6, 0, PH_NOISY, "stub/quantum.zep", 53);
					zephir_array_fetch_long(&_33$$6, &psr, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 53);
					ZEPHIR_OBS_NVAR(&_23$$6);
					zephir_array_fetch_long(&_23$$6, &_33$$6, 1, PH_NOISY, "stub/quantum.zep", 53);
					ZEPHIR_INIT_NVAR(&_26$$6);
					mul_function(&_26$$6, &_20$$6, &_23$$6);
					zephir_array_fetch_long(&_34$$6, &psi, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 53);
					ZEPHIR_OBS_NVAR(&_20$$6);
					zephir_array_fetch_long(&_20$$6, &_34$$6, 0, PH_NOISY, "stub/quantum.zep", 53);
					zephir_array_fetch_long(&_35$$6, &psi, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 53);
					ZEPHIR_OBS_NVAR(&_23$$6);
					zephir_array_fetch_long(&_23$$6, &_35$$6, 0, PH_NOISY, "stub/quantum.zep", 53);
					ZEPHIR_INIT_NVAR(&_29$$6);
					mul_function(&_29$$6, &_20$$6, &_23$$6);
					ZEPHIR_INIT_NVAR(&_30$$6);
					zephir_add_function(&_30$$6, &_26$$6, &_29$$6);
					zephir_array_update_long(&p2, i, &_30$$6, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
				}
			}
			_38$$5 = (751 - 1);
			_37$$5 = 1;
			_36$$5 = 0;
			if (_37$$5 <= _38$$5) {
				while (1) {
					if (_36$$5) {
						_37$$5++;
						if (!(_37$$5 <= _38$$5)) {
							break;
						}
					} else {
						_36$$5 = 1;
					}
					j = _37$$5;
					ZEPHIR_INIT_NVAR(&_39$$7);
					ZVAL_LONG(&_39$$7, 0);
					zephir_array_update_multi(&psr, &_39$$7, SL("ll"), 2, (zend_long) 0, (zend_long) 1);
					ZEPHIR_INIT_NVAR(&_39$$7);
					ZVAL_LONG(&_39$$7, 0);
					zephir_array_update_multi(&psr, &_39$$7, SL("ll"), 2, (zend_long) 751, (zend_long) 1);
					zephir_array_fetch_long(&_40$$7, &psi, j, PH_NOISY | PH_READONLY, "stub/quantum.zep", 58);
					ZEPHIR_OBS_NVAR(&_41$$7);
					zephir_array_fetch_long(&_41$$7, &_40$$7, 0, PH_NOISY, "stub/quantum.zep", 58);
					zephir_array_fetch_long(&_42$$7, &paramater, 2, PH_NOISY | PH_READONLY, "stub/quantum.zep", 58);
					zephir_array_fetch_long(&_43$$7, &psr, (j + 1), PH_NOISY | PH_READONLY, "stub/quantum.zep", 58);
					ZEPHIR_OBS_NVAR(&_44$$7);
					zephir_array_fetch_long(&_44$$7, &_43$$7, 1, PH_NOISY, "stub/quantum.zep", 58);
					zephir_array_fetch_long(&_45$$7, &psr, (j - 1), PH_NOISY | PH_READONLY, "stub/quantum.zep", 58);
					ZEPHIR_OBS_NVAR(&_46$$7);
					zephir_array_fetch_long(&_46$$7, &_45$$7, 1, PH_NOISY, "stub/quantum.zep", 59);
					ZEPHIR_INIT_NVAR(&_47$$7);
					zephir_add_function(&_47$$7, &_44$$7, &_46$$7);
					zephir_array_fetch_long(&_48$$7, &tmp, 0, PH_NOISY | PH_READONLY, "stub/quantum.zep", 59);
					zephir_array_fetch_long(&_49$$7, &psr, j, PH_NOISY | PH_READONLY, "stub/quantum.zep", 59);
					ZEPHIR_OBS_NVAR(&_44$$7);
					zephir_array_fetch_long(&_44$$7, &_49$$7, 1, PH_NOISY, "stub/quantum.zep", 59);
					ZEPHIR_INIT_NVAR(&_50$$7);
					mul_function(&_50$$7, &_48$$7, &_44$$7);
					ZEPHIR_INIT_NVAR(&_51$$7);
					zephir_sub_function(&_51$$7, &_47$$7, &_50$$7);
					ZEPHIR_INIT_NVAR(&_47$$7);
					mul_function(&_47$$7, &_42$$7, &_51$$7);
					zephir_array_fetch_long(&_42$$7, &paramater, 0, PH_NOISY | PH_READONLY, "stub/quantum.zep", 59);
					zephir_array_fetch_long(&_48$$7, &paramater, 0, PH_NOISY | PH_READONLY, "stub/quantum.zep", 59);
					ZEPHIR_INIT_NVAR(&_51$$7);
					mul_function(&_51$$7, &_42$$7, &_48$$7);
					ZEPHIR_INIT_NVAR(&_50$$7);
					div_function(&_50$$7, &_47$$7, &_51$$7);
					ZEPHIR_INIT_NVAR(&_47$$7);
					zephir_add_function(&_47$$7, &_41$$7, &_50$$7);
					zephir_array_fetch_long(&_42$$7, &paramater, 2, PH_NOISY | PH_READONLY, "stub/quantum.zep", 59);
					zephir_array_fetch_long(&_48$$7, &v, j, PH_NOISY | PH_READONLY, "stub/quantum.zep", 59);
					ZEPHIR_OBS_NVAR(&_41$$7);
					zephir_array_fetch_long(&_41$$7, &_48$$7, 0, PH_NOISY, "stub/quantum.zep", 59);
					ZEPHIR_INIT_NVAR(&_50$$7);
					mul_function(&_50$$7, &_42$$7, &_41$$7);
					zephir_array_fetch_long(&_42$$7, &psr, j, PH_NOISY | PH_READONLY, "stub/quantum.zep", 59);
					ZEPHIR_OBS_NVAR(&_41$$7);
					zephir_array_fetch_long(&_41$$7, &_42$$7, 1, PH_NOISY, "stub/quantum.zep", 59);
					ZEPHIR_INIT_NVAR(&_52$$7);
					mul_function(&_52$$7, &_50$$7, &_41$$7);
					ZEPHIR_INIT_NVAR(&_51$$7);
					zephir_sub_function(&_51$$7, &_47$$7, &_52$$7);
					zephir_array_update_multi(&psi, &_51$$7, SL("ll"), 2, (zend_long) j, (zend_long) 1);
				}
			}
			_53$$5 = n == 0;
			if (!(_53$$5)) {
				_53$$5 = zephir_safe_mod_long_long(n, 2000) == 0;
			}
			if (_53$$5) {
				i = 1;
				while (1) {
					if (!(i < (751 - 1))) {
						break;
					}
					zephir_array_fetch_long(&_54$$9, &p2, i, PH_NOISY | PH_READONLY, "stub/quantum.zep", 67);
					ZEPHIR_INIT_NVAR(&_55$$9);
					ZVAL_STRING(&_55$$9, "%16.8lf %16.8lf %16.8lf \n");
					ZVAL_DOUBLE(&_56$$9, ((double) i * dx));
					ZVAL_DOUBLE(&_57$$9, ((double) n * dt));
					ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_58, 102, &fp, &_55$$9, &_56$$9, &_57$$9, &_54$$9);
					zephir_check_call_status();
					i = (i + 10);
				}
				ZEPHIR_INIT_NVAR(&_59$$8);
				ZVAL_STRING(&_59$$8, "\n");
				ZEPHIR_CALL_FUNCTION(NULL, "fprintf", &_58, 102, &fp, &_59$$8);
				zephir_check_call_status();
			}
			j = 1;
			while (1) {
				if (!(j < (751 - 1))) {
					break;
				}
				zephir_array_fetch_long(&_60$$10, &psi, j, PH_NOISY | PH_READONLY, "stub/quantum.zep", 76);
				ZEPHIR_OBS_NVAR(&_61$$10);
				zephir_array_fetch_long(&_61$$10, &_60$$10, 1, PH_NOISY, "stub/quantum.zep", 76);
				zephir_array_update_multi(&psi, &_61$$10, SL("ll"), 2, (zend_long) j, (zend_long) 0);
				zephir_array_fetch_long(&_62$$10, &psr, j, PH_NOISY | PH_READONLY, "stub/quantum.zep", 77);
				ZEPHIR_OBS_NVAR(&_61$$10);
				zephir_array_fetch_long(&_61$$10, &_62$$10, 1, PH_NOISY, "stub/quantum.zep", 77);
				zephir_array_update_multi(&psr, &_61$$10, SL("ll"), 2, (zend_long) j, (zend_long) 0);
				j++;
			}
		}
	}
	RETURN_MM_BOOL(1);
}

