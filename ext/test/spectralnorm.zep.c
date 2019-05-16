
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
#include "math.h"


/**
 * SpectralNorm
 *
 * @see http://mathworld.wolfram.com/SpectralNorm.html
 */
ZEPHIR_INIT_CLASS(Test_SpectralNorm) {

	ZEPHIR_REGISTER_CLASS(Test, SpectralNorm, test, spectralnorm, test_spectralnorm_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_SpectralNorm, Ax) {

	zval *i, i_sub, *j, j_sub, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&j_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &i, &j);



	ZEPHIR_INIT_VAR(&_0);
	zephir_add_function(&_0, i, j);
	ZEPHIR_SINIT_VAR(_1);
	zephir_add_function(&_1, i, j);
	RETURN_MM_DOUBLE(zephir_safe_div_long_double((double) 1, ((zephir_safe_div_long_long((zephir_get_numberval(&_0) * (zephir_get_numberval(&_1) + 1)), 2 TSRMLS_CC) + zephir_get_numberval(i)) + (double) (1)) TSRMLS_CC));

}

PHP_METHOD(Test_SpectralNorm, Au) {

	zend_bool _0, _3$$3;
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

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);

	n = zephir_get_intval(n_param);


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
					ZEPHIR_CALL_METHOD(&_6$$4, this_ptr, "ax", &_9, 74, &_7$$4, &_8$$4);
					zephir_check_call_status();
					ZVAL_LONG(&_7$$4, j);
					ZEPHIR_CALL_METHOD(&_10$$4, u, "offsetget", &_11, 0, &_7$$4);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_12$$4);
					mul_function(&_12$$4, &_6$$4, &_10$$4 TSRMLS_CC);
					t += zephir_get_numberval(&_12$$4);
				}
			}
			ZVAL_LONG(&_13$$3, i);
			ZVAL_LONG(&_14$$3, t);
			ZEPHIR_CALL_METHOD(NULL, v, "offsetset", &_15, 0, &_13$$3, &_14$$3);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, Atu) {

	zend_bool _0, _3$$3;
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

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);

	n = zephir_get_intval(n_param);


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
					ZEPHIR_CALL_METHOD(&_6$$4, this_ptr, "ax", &_9, 74, &_7$$4, &_8$$4);
					zephir_check_call_status();
					ZVAL_LONG(&_7$$4, j);
					ZEPHIR_CALL_METHOD(&_10$$4, u, "offsetget", &_11, 0, &_7$$4);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_12$$4);
					mul_function(&_12$$4, &_6$$4, &_10$$4 TSRMLS_CC);
					t += zephir_get_numberval(&_12$$4);
				}
			}
			ZVAL_LONG(&_13$$3, i);
			ZVAL_LONG(&_14$$3, t);
			ZEPHIR_CALL_METHOD(NULL, v, "offsetset", &_15, 0, &_13$$3, &_14$$3);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, AtAu) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *n, n_sub, *u, u_sub, *v, v_sub, *w, w_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n_sub);
	ZVAL_UNDEF(&u_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&w_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &n, &u, &v, &w);



	ZEPHIR_CALL_METHOD(NULL, this_ptr, "au", NULL, 75, n, u, w);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "atu", NULL, 76, n, w, v);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, process) {

	zend_bool _1, _7, _12;
	zephir_fcall_cache_entry *_6 = NULL, *_11 = NULL, *_17 = NULL;
	zval *n_param = NULL, u, v, w, _0, _4$$3, _5$$3, _10$$4, _15$$5, _16$$5, _18$$5, _19$$5, _20$$5;
	zend_long n, ZEPHIR_LAST_CALL_STATUS, i = 0, vv, vBv, _2, _3, _8, _9, _13, _14;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&w);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_10$$4);
	ZVAL_UNDEF(&_15$$5);
	ZVAL_UNDEF(&_16$$5);
	ZVAL_UNDEF(&_18$$5);
	ZVAL_UNDEF(&_19$$5);
	ZVAL_UNDEF(&_20$$5);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	vv = 0;
	vBv = 0;
	ZEPHIR_INIT_VAR(&u);
	object_init_ex(&u, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD(NULL, &u, "__construct", NULL, 77, &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&v);
	object_init_ex(&v, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD(NULL, &v, "__construct", NULL, 77, &_0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(&w);
	object_init_ex(&w, spl_ce_SplFixedArray);
	ZVAL_LONG(&_0, n);
	ZEPHIR_CALL_METHOD(NULL, &w, "__construct", NULL, 77, &_0);
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
			ZEPHIR_CALL_METHOD(NULL, &u, "offsetset", &_6, 78, &_4$$3, &_5$$3);
			zephir_check_call_status();
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, 1);
			ZEPHIR_CALL_METHOD(NULL, &v, "offsetset", &_6, 78, &_4$$3, &_5$$3);
			zephir_check_call_status();
			ZVAL_LONG(&_4$$3, i);
			ZVAL_LONG(&_5$$3, 1);
			ZEPHIR_CALL_METHOD(NULL, &w, "offsetset", &_6, 78, &_4$$3, &_5$$3);
			zephir_check_call_status();
		}
	}
	_9 = 9;
	_8 = 0;
	_7 = 0;
	if (_8 <= _9) {
		while (1) {
			if (_7) {
				_8++;
				if (!(_8 <= _9)) {
					break;
				}
			} else {
				_7 = 1;
			}
			i = _8;
			ZVAL_LONG(&_10$$4, n);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "atau", &_11, 79, &_10$$4, &u, &v, &w);
			zephir_check_call_status();
			ZVAL_LONG(&_10$$4, n);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "atau", &_11, 79, &_10$$4, &v, &u, &w);
			zephir_check_call_status();
		}
	}
	_14 = (n - 1);
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
			i = _13;
			ZVAL_LONG(&_16$$5, i);
			ZEPHIR_CALL_METHOD(&_15$$5, &u, "offsetget", &_17, 80, &_16$$5);
			zephir_check_call_status();
			ZVAL_LONG(&_16$$5, i);
			ZEPHIR_CALL_METHOD(&_18$$5, &v, "offsetget", &_17, 80, &_16$$5);
			zephir_check_call_status();
			ZEPHIR_INIT_LNVAR(_19$$5);
			mul_function(&_19$$5, &_15$$5, &_18$$5 TSRMLS_CC);
			vBv += zephir_get_numberval(&_19$$5);
			ZVAL_LONG(&_16$$5, i);
			ZEPHIR_CALL_METHOD(&_15$$5, &v, "offsetget", &_17, 80, &_16$$5);
			zephir_check_call_status();
			ZVAL_LONG(&_16$$5, i);
			ZEPHIR_CALL_METHOD(&_18$$5, &v, "offsetget", &_17, 80, &_16$$5);
			zephir_check_call_status();
			ZEPHIR_INIT_LNVAR(_20$$5);
			mul_function(&_20$$5, &_15$$5, &_18$$5 TSRMLS_CC);
			vv += zephir_get_numberval(&_20$$5);
		}
	}
	ZVAL_DOUBLE(&_0, zephir_safe_div_long_long(vBv, vv TSRMLS_CC));
	RETURN_MM_DOUBLE(sqrt(zephir_safe_div_long_long(vBv, vv TSRMLS_CC)));

}

