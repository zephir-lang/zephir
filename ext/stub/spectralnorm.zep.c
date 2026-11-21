
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
	zval *n_param = NULL, *u, u_sub, *v, v_sub, _11$$3, _12$$3, _6$$4, _7$$4, _8$$4, _9$$4, _10$$4;
	zend_long n, ZEPHIR_LAST_CALL_STATUS, t = 0, i = 0, j = 0, _1, _2, _4$$3, _5$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_11$$3);
	ZVAL_UNDEF(&_12$$3);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_10$$4);
	ZEND_TLS zephir_method_cache _zephir_call_cache[3];
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
					ZEPHIR_CALL_METHOD_CACHED(&_6$$4, this_ptr, "Ax", &_zephir_call_cache[0], &_7$$4, &_8$$4);
					zephir_check_call_status();
					ZVAL_LONG(&_7$$4, j);
					ZEPHIR_CALL_METHOD_CACHED(&_9$$4, u, "offsetGet", &_zephir_call_cache[1], &_7$$4);
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(&_10$$4);
					mul_function(&_10$$4, &_6$$4, &_9$$4);
					if (UNEXPECTED(EG(exception))) {
						ZEPHIR_MM_RESTORE();
						return;
					}
					t += zephir_get_numberval(&_10$$4);
				}
			}
			ZVAL_LONG(&_11$$3, i);
			ZVAL_LONG(&_12$$3, t);
			ZEPHIR_CALL_METHOD_CACHED(NULL, v, "offsetSet", &_zephir_call_cache[2], &_11$$3, &_12$$3);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_SpectralNorm, Atu)
{
	zend_bool _0, _3$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, *u, u_sub, *v, v_sub, _11$$3, _12$$3, _6$$4, _7$$4, _8$$4, _9$$4, _10$$4;
	zend_long n, ZEPHIR_LAST_CALL_STATUS, t = 0, i = 0, j = 0, _1, _2, _4$$3, _5$$3;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&_11$$3);
	ZVAL_UNDEF(&_12$$3);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_7$$4);
	ZVAL_UNDEF(&_8$$4);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_10$$4);
	ZEND_TLS zephir_method_cache _zephir_call_cache[3];
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
					ZEPHIR_CALL_METHOD_CACHED(&_6$$4, this_ptr, "Ax", &_zephir_call_cache[0], &_7$$4, &_8$$4);
					zephir_check_call_status();
					ZVAL_LONG(&_7$$4, j);
					ZEPHIR_CALL_METHOD_CACHED(&_9$$4, u, "offsetGet", &_zephir_call_cache[1], &_7$$4);
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(&_10$$4);
					mul_function(&_10$$4, &_6$$4, &_9$$4);
					if (UNEXPECTED(EG(exception))) {
						ZEPHIR_MM_RESTORE();
						return;
					}
					t += zephir_get_numberval(&_10$$4);
				}
			}
			ZVAL_LONG(&_11$$3, i);
			ZVAL_LONG(&_12$$3, t);
			ZEPHIR_CALL_METHOD_CACHED(NULL, v, "offsetSet", &_zephir_call_cache[2], &_11$$3, &_12$$3);
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
	ZEND_TLS zephir_method_cache _zephir_call_cache[2];
	ZEND_PARSE_PARAMETERS_START(4, 4)
		Z_PARAM_ZVAL(n)
		Z_PARAM_ZVAL(u)
		Z_PARAM_ZVAL(v)
		Z_PARAM_ZVAL(w)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 4, 0, &n, &u, &v, &w);
	ZEPHIR_CALL_METHOD_CACHED(NULL, this_ptr, "Au", &_zephir_call_cache[0], n, u, w);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD_CACHED(NULL, this_ptr, "Atu", &_zephir_call_cache[1], n, w, v);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_SpectralNorm, process)
{
	zend_bool _1, _6, _10;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *n_param = NULL, u, v, w, _0, _18, _4$$3, _5$$3, _9$$4, _13$$5, _14$$5, _15$$5, _16$$5, _17$$5;
	zend_long n, ZEPHIR_LAST_CALL_STATUS, i = 0, vv, vBv, _2, _3, _7, _8, _11, _12;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&w);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_18);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_13$$5);
	ZVAL_UNDEF(&_14$$5);
	ZVAL_UNDEF(&_15$$5);
	ZVAL_UNDEF(&_16$$5);
	ZVAL_UNDEF(&_17$$5);
	ZEND_TLS zephir_method_cache _zephir_call_cache[12];
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
	ZEPHIR_CALL_METHOD_CACHED(NULL, &u, "__construct", &_zephir_call_cache[0], &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&v);
	object_init_ex(&v, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &v, "__construct", &_zephir_call_cache[1], &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&w);
	object_init_ex(&w, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD_CACHED(NULL, &w, "__construct", &_zephir_call_cache[2], &_0);
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
			ZEPHIR_CALL_METHOD_CACHED(NULL, &u, "offsetSet", &_zephir_call_cache[3], &_4$$3, &_5$$3);
			zephir_check_call_status();
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, 1);
			ZEPHIR_CALL_METHOD_CACHED(NULL, &v, "offsetSet", &_zephir_call_cache[4], &_4$$3, &_5$$3);
			zephir_check_call_status();
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, 1);
			ZEPHIR_CALL_METHOD_CACHED(NULL, &w, "offsetSet", &_zephir_call_cache[5], &_4$$3, &_5$$3);
			zephir_check_call_status();
		}
	}
	_8 = 9;
	_7 = 0;
	_6 = 0;
	if (_7 <= _8) {
		while (1) {
			if (_6) {
				_7++;
				if (!(_7 <= _8)) {
					break;
				}
			} else {
				_6 = 1;
			}
			i = _7;
			ZVAL_LONG(&_9$$4, n);
			ZEPHIR_CALL_METHOD_CACHED(NULL, this_ptr, "AtAu", &_zephir_call_cache[6], &_9$$4, &u, &v, &w);
			zephir_check_call_status();
			ZVAL_LONG(&_9$$4, n);
			ZEPHIR_CALL_METHOD_CACHED(NULL, this_ptr, "AtAu", &_zephir_call_cache[7], &_9$$4, &v, &u, &w);
			zephir_check_call_status();
		}
	}
	_12 = (n - 1);
	_11 = 0;
	_10 = 0;
	if (_11 <= _12) {
		while (1) {
			if (_10) {
				_11++;
				if (!(_11 <= _12)) {
					break;
				}
			} else {
				_10 = 1;
			}
			i = _11;
			ZVAL_LONG(&_14$$5, i);
			ZEPHIR_CALL_METHOD_CACHED(&_13$$5, &u, "offsetGet", &_zephir_call_cache[8], &_14$$5);
			zephir_check_call_status();
			ZVAL_LONG(&_14$$5, i);
			ZEPHIR_CALL_METHOD_CACHED(&_15$$5, &v, "offsetGet", &_zephir_call_cache[9], &_14$$5);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(&_16$$5);
			mul_function(&_16$$5, &_13$$5, &_15$$5);
			if (UNEXPECTED(EG(exception))) {
				ZEPHIR_MM_RESTORE();
				return;
			}
			vBv += zephir_get_numberval(&_16$$5);
			ZVAL_LONG(&_14$$5, i);
			ZEPHIR_CALL_METHOD_CACHED(&_13$$5, &v, "offsetGet", &_zephir_call_cache[10], &_14$$5);
			zephir_check_call_status();
			ZVAL_LONG(&_14$$5, i);
			ZEPHIR_CALL_METHOD_CACHED(&_15$$5, &v, "offsetGet", &_zephir_call_cache[11], &_14$$5);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(&_17$$5);
			mul_function(&_17$$5, &_13$$5, &_15$$5);
			if (UNEXPECTED(EG(exception))) {
				ZEPHIR_MM_RESTORE();
				return;
			}
			vv += zephir_get_numberval(&_17$$5);
		}
	}
	ZEPHIR_INIT_VAR(&_18);
	zephir_div_long_long(&_18, vBv, vv);
	if (UNEXPECTED(EG(exception))) {
		ZEPHIR_MM_RESTORE();
		return;
	}
	RETURN_MM_DOUBLE(zephir_sqrt(&_18));
}

