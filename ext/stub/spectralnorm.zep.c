
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
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "ext/spl/spl_fixedarray.h"
#include "kernel/math.h"


/**
 * SpectralNorm
 *
 * @see https://mathworld.wolfram.com/SpectralNorm.html
 */
ZEPHIR_INIT_CLASS(Stub_SpectralNorm)
{
	ZEPHIR_REGISTER_CLASS(Stub, SpectralNorm, stub, spectralnorm, stub_spectralnorm_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_SpectralNorm, Ax)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *i, i_sub, *j, j_sub, _0, _1, _2, _3;

	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(j)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &i, &j);
	ZEPHIR_INIT_VAR(&_0);
	zephir_add_function(&_0, i, j);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_1);
	zephir_add_function(&_1, i, j);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_INIT_VAR(&_3);
	zephir_add_function(&_3, &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_1);
	mul_function(&_1, &_0, &_3);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_0);
	zephir_div_zval_long(&_0, &_1, 2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_1);
	zephir_add_function(&_1, &_0, i);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	ZEPHIR_INIT_NVAR(&_2);
	ZVAL_LONG(&_2, 1);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_add_function(&_0, &_1, &_2);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	zephir_div_long_zval(return_value, 1, &_0);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM();
}

PHP_METHOD(Stub_SpectralNorm, Au)
{
	zend_bool _0, _3$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_9 = NULL, *_11 = NULL, *_15 = NULL;
	zval *n_param = NULL, *u, u_sub, *v, v_sub, _13$$3, _14$$3, _6$$4, _7$$4, _8$$4, _10$$4, _12$$4;
	zend_long n, ZEPHIR_LAST_CALL_STATUS, t = 0, i = 0, j = 0, _1, _2, _4$$3, _5$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_13$$3);
	ZVAL_UNDEF(&_14$$3);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_10$$4);
	ZVAL_UNDEF(&_12$$4);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(n)
		Z_PARAM_OBJECT_OF_CLASS(u, spl_ce_SplFixedArray)
		Z_PARAM_OBJECT_OF_CLASS(v, spl_ce_SplFixedArray)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);
	_2 = (n - 1);
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			t = 0;
			_5$$3 = (n - 1);
			_4$$3 = 0;
			_3$$3 = 0;
			if (_4$$3 <= _5$$3) {
				while (1) {
					if (_3$$3) {
						_4$$3++;
						if (!(_4$$3 <= _5$$3)) {
							break;
						}
					} else {
						_3$$3 = 1;
					}
					j = _4$$3;
					ZVAL_LONG(&_7$$4, i);
					ZVAL_LONG(&_8$$4, j);
					ZEPHIR_CALL_METHOD(&_6$$4, this_ptr, "Ax", &_9, 116, &_7$$4, &_8$$4);
					zephir_check_call_status();
					ZVAL_LONG(&_7$$4, j);
					ZEPHIR_CALL_METHOD(&_10$$4, u, "offsetGet", &_11, 0, &_7$$4);
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(&_12$$4);
					mul_function(&_12$$4, &_6$$4, &_10$$4);
					if (UNEXPECTED(EG(exception))) {
						ZEPHIR_MM_RESTORE();
						return;
					}
					t += zephir_get_numberval(&_12$$4);
				}
			}
			ZVAL_LONG(&_13$$3, i);
			ZVAL_LONG(&_14$$3, t);
			ZEPHIR_CALL_METHOD(NULL, v, "offsetSet", &_15, 0, &_13$$3, &_14$$3);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_SpectralNorm, Atu)
{
	zend_bool _0, _3$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_9 = NULL, *_11 = NULL, *_15 = NULL;
	zval *n_param = NULL, *u, u_sub, *v, v_sub, _13$$3, _14$$3, _6$$4, _7$$4, _8$$4, _10$$4, _12$$4;
	zend_long n, ZEPHIR_LAST_CALL_STATUS, t = 0, i = 0, j = 0, _1, _2, _4$$3, _5$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_13$$3);
	ZVAL_UNDEF(&_14$$3);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_10$$4);
	ZVAL_UNDEF(&_12$$4);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_LONG(n)
		Z_PARAM_OBJECT_OF_CLASS(u, spl_ce_SplFixedArray)
		Z_PARAM_OBJECT_OF_CLASS(v, spl_ce_SplFixedArray)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);
	_2 = (n - 1);
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			t = 0;
			_5$$3 = (n - 1);
			_4$$3 = 0;
			_3$$3 = 0;
			if (_4$$3 <= _5$$3) {
				while (1) {
					if (_3$$3) {
						_4$$3++;
						if (!(_4$$3 <= _5$$3)) {
							break;
						}
					} else {
						_3$$3 = 1;
					}
					j = _4$$3;
					ZVAL_LONG(&_7$$4, j);
					ZVAL_LONG(&_8$$4, i);
					ZEPHIR_CALL_METHOD(&_6$$4, this_ptr, "Ax", &_9, 116, &_7$$4, &_8$$4);
					zephir_check_call_status();
					ZVAL_LONG(&_7$$4, j);
					ZEPHIR_CALL_METHOD(&_10$$4, u, "offsetGet", &_11, 0, &_7$$4);
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(&_12$$4);
					mul_function(&_12$$4, &_6$$4, &_10$$4);
					if (UNEXPECTED(EG(exception))) {
						ZEPHIR_MM_RESTORE();
						return;
					}
					t += zephir_get_numberval(&_12$$4);
				}
			}
			ZVAL_LONG(&_13$$3, i);
			ZVAL_LONG(&_14$$3, t);
			ZEPHIR_CALL_METHOD(NULL, v, "offsetSet", &_15, 0, &_13$$3, &_14$$3);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_SpectralNorm, AtAu)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n, n_sub, *u, u_sub, *v, v_sub, *w, w_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&u_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&w_sub);
	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_ZVAL(n)
		Z_PARAM_ZVAL(u)
		Z_PARAM_ZVAL(v)
		Z_PARAM_ZVAL(w)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 4, 0, &n, &u, &v, &w);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "Au", NULL, 117, n, u, w);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "Atu", NULL, 118, n, w, v);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_SpectralNorm, process)
{
	zend_bool _1, _9, _14;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zephir_fcall_cache_entry *_6 = NULL, *_7 = NULL, *_8 = NULL, *_13 = NULL, *_19 = NULL, *_21 = NULL;
	zval *n_param = NULL, u, v, w, _0, _24, _4$$3, _5$$3, _12$$4, _17$$5, _18$$5, _20$$5, _22$$5, _23$$5;
	zend_long n, ZEPHIR_LAST_CALL_STATUS, i = 0, vv, vBv, _2, _3, _10, _11, _15, _16;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&w);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_24);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_12$$4);
	ZVAL_UNDEF(&_17$$5);
	ZVAL_UNDEF(&_18$$5);
	ZVAL_UNDEF(&_20$$5);
	ZVAL_UNDEF(&_22$$5);
	ZVAL_UNDEF(&_23$$5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(n)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &n_param);
	vv = 0;
	vBv = 0;
	ZEPHIR_INIT_VAR(&u);
	object_init_ex(&u, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD(NULL, &u, "__construct", NULL, 119, &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&v);
	object_init_ex(&v, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD(NULL, &v, "__construct", NULL, 119, &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&w);
	object_init_ex(&w, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD(NULL, &w, "__construct", NULL, 119, &_0);
	zephir_check_call_status();
	_3 = (n - 1);
	_2 = 0;
	_1 = 0;
	if (_2 <= _3) {
		while (1) {
			if (_1) {
				_2++;
				if (!(_2 <= _3)) {
					break;
				}
			} else {
				_1 = 1;
			}
			i = _2;
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, 1);
			ZEPHIR_CALL_METHOD(NULL, &u, "offsetSet", &_6, 120, &_4$$3, &_5$$3);
			zephir_check_call_status();
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, 1);
			ZEPHIR_CALL_METHOD(NULL, &v, "offsetSet", &_7, 120, &_4$$3, &_5$$3);
			zephir_check_call_status();
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, 1);
			ZEPHIR_CALL_METHOD(NULL, &w, "offsetSet", &_8, 120, &_4$$3, &_5$$3);
			zephir_check_call_status();
		}
	}
	_11 = 9;
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
			i = _10;
			ZVAL_LONG(&_12$$4, n);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "AtAu", &_13, 121, &_12$$4, &u, &v, &w);
			zephir_check_call_status();
			ZVAL_LONG(&_12$$4, n);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "AtAu", &_13, 121, &_12$$4, &v, &u, &w);
			zephir_check_call_status();
		}
	}
	_16 = (n - 1);
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
			i = _15;
			ZVAL_LONG(&_18$$5, i);
			ZEPHIR_CALL_METHOD(&_17$$5, &u, "offsetGet", &_19, 122, &_18$$5);
			zephir_check_call_status();
			ZVAL_LONG(&_18$$5, i);
			ZEPHIR_CALL_METHOD(&_20$$5, &v, "offsetGet", &_21, 122, &_18$$5);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(&_22$$5);
			mul_function(&_22$$5, &_17$$5, &_20$$5);
			if (UNEXPECTED(EG(exception))) {
				ZEPHIR_MM_RESTORE();
				return;
			}
			vBv += zephir_get_numberval(&_22$$5);
			ZVAL_LONG(&_18$$5, i);
			ZEPHIR_CALL_METHOD(&_17$$5, &v, "offsetGet", &_21, 122, &_18$$5);
			zephir_check_call_status();
			ZVAL_LONG(&_18$$5, i);
			ZEPHIR_CALL_METHOD(&_20$$5, &v, "offsetGet", &_21, 122, &_18$$5);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(&_23$$5);
			mul_function(&_23$$5, &_17$$5, &_20$$5);
			if (UNEXPECTED(EG(exception))) {
				ZEPHIR_MM_RESTORE();
				return;
			}
			vv += zephir_get_numberval(&_23$$5);
		}
	}
	ZEPHIR_INIT_VAR(&_24);
	zephir_div_long_long(&_24, vBv, vv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_DOUBLE(zephir_sqrt(&_24));
}

