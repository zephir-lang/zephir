
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
#include "kernel/fcall.h"


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

	zval *i, *j, *_0, _1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &i, &j);



	ZEPHIR_INIT_VAR(_0);
	zephir_add_function(_0, i, j TSRMLS_CC);
	ZEPHIR_SINIT_VAR(_1);
	zephir_add_function(&_1, i, j TSRMLS_CC);
	RETURN_MM_LONG((1 / ((((zephir_get_numberval(_0) * (zephir_get_numberval(&_1) + 1)) / 2) + zephir_get_numberval(i)) + 1)));

}

PHP_METHOD(Test_SpectralNorm, Au) {

	zend_function *_9 = NULL, *_11 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_10 = NULL;
	int n, t, i, j, _1, _2, _4, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);

		n = zephir_get_intval(n_param);


	_2 = (n - 1);
	_1 = 0;
	_0 = 0;
	if ((_1 <= _2)) {
		while (1) {
			if (_0) {
				_1++;
				if (!((_1 <= _2))) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			t = 0;
			_5 = (n - 1);
			_4 = 0;
			_3 = 0;
			if ((_4 <= _5)) {
				while (1) {
					if (_3) {
						_4++;
						if (!((_4 <= _5))) {
							break;
						}
					} else {
						_3 = 1;
					}
					j = _4;
					ZEPHIR_INIT_NVAR(_6);
					ZEPHIR_INIT_NVAR(_7);
					ZVAL_LONG(_7, i);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_internal_method_p2(_6, this_ptr, "ax", _7, _8);
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1_cache(_7, u, "offsetge", &_9, _8);
					ZEPHIR_INIT_LNVAR(_10);
					mul_function(_10, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_10);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZVAL_LONG(_6, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, t);
			zephir_call_method_p2_cache_noret(v, "offsetset", &_11, _6, _7);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, Atu) {

	zend_function *_9 = NULL, *_11 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_10 = NULL;
	int n, t, i, j, _1, _2, _4, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);

		n = zephir_get_intval(n_param);


	_2 = (n - 1);
	_1 = 0;
	_0 = 0;
	if ((_1 <= _2)) {
		while (1) {
			if (_0) {
				_1++;
				if (!((_1 <= _2))) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			t = 0;
			_5 = (n - 1);
			_4 = 0;
			_3 = 0;
			if ((_4 <= _5)) {
				while (1) {
					if (_3) {
						_4++;
						if (!((_4 <= _5))) {
							break;
						}
					} else {
						_3 = 1;
					}
					j = _4;
					ZEPHIR_INIT_NVAR(_6);
					ZEPHIR_INIT_NVAR(_7);
					ZVAL_LONG(_7, j);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, i);
					zephir_call_internal_method_p2(_6, this_ptr, "ax", _7, _8);
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1_cache(_7, u, "offsetget", &_9, _8);
					ZEPHIR_INIT_LNVAR(_10);
					mul_function(_10, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_10);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZVAL_LONG(_6, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, t);
			zephir_call_method_p2_cache_noret(v, "offsetset", &_11, _6, _7);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, AtAu) {

	zval *n, *u, *v, *w;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &n, &u, &v, &w);



	zephir_call_internal_method_p3_noret(this_ptr, "au", n, u, w);
	zephir_call_internal_method_p3_noret(this_ptr, "atu", n, w, v);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, process) {

	zend_function *_8 = NULL, *_9 = NULL, *_10 = NULL, *_17 = NULL, *_19 = NULL, *_21 = NULL, *_22 = NULL;
	zend_bool _4, _11, _14;
	zend_class_entry *_0, *_2, *_3;
	zval *n_param = NULL, *u, *v, *w, *_1 = NULL, *_7 = NULL, *_18 = NULL, *_20 = NULL, *_23 = NULL, _24;
	int n, i, vv = 0, vBv = 0, _5, _6, _12, _13, _15, _16;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

		n = zephir_get_intval(n_param);


	ZEPHIR_INIT_VAR(u);
	_0 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(u, _0);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, n);
	zephir_call_method_p1_noret(u, "__construct", _1);
	ZEPHIR_INIT_VAR(v);
	_2 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(v, _2);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, n);
	zephir_call_method_p1_noret(v, "__construct", _1);
	ZEPHIR_INIT_VAR(w);
	_3 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(w, _3);
	ZEPHIR_INIT_BNVAR(_1);
	ZVAL_LONG(_1, n);
	zephir_call_method_p1_noret(w, "__construct", _1);
	_6 = (n - 1);
	_5 = 0;
	_4 = 0;
	if ((_5 <= _6)) {
		while (1) {
			if (_4) {
				_5++;
				if (!((_5 <= _6))) {
					break;
				}
			} else {
				_4 = 1;
			}
			i = _5;
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, 1);
			zephir_call_method_p2_cache_noret(u, "offsetset", &_8, _1, _7);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, 1);
			zephir_call_method_p2_cache_noret(v, "offsetset", &_9, _1, _7);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, 1);
			zephir_call_method_p2_cache_noret(w, "offsetset", &_10, _1, _7);
		}
	}
	_13 = 9;
	_12 = 0;
	_11 = 0;
	if ((_12 <= _13)) {
		while (1) {
			if (_11) {
				_12++;
				if (!((_12 <= _13))) {
					break;
				}
			} else {
				_11 = 1;
			}
			i = _12;
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, n);
			zephir_call_internal_method_p4_noret(this_ptr, "atau", _1, u, v, w);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, n);
			zephir_call_internal_method_p4_noret(this_ptr, "atau", _1, v, u, w);
		}
	}
	_16 = (n - 1);
	_15 = 0;
	_14 = 0;
	if ((_15 <= _16)) {
		while (1) {
			if (_14) {
				_15++;
				if (!((_15 <= _16))) {
					break;
				}
			} else {
				_14 = 1;
			}
			i = _15;
			ZEPHIR_INIT_NVAR(_1);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			zephir_call_method_p1_cache(_1, u, "offsetget", &_17, _7);
			ZEPHIR_INIT_NVAR(_7);
			ZEPHIR_INIT_NVAR(_18);
			ZVAL_LONG(_18, i);
			zephir_call_method_p1_cache(_7, v, "offsetget", &_19, _18);
			ZEPHIR_INIT_LNVAR(_20);
			mul_function(_20, _1, _7 TSRMLS_CC);
			vBv += zephir_get_numberval(_20);
			ZEPHIR_INIT_NVAR(_1);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			zephir_call_method_p1_cache(_1, v, "offsetget", &_21, _7);
			ZEPHIR_INIT_NVAR(_7);
			ZEPHIR_INIT_NVAR(_18);
			ZVAL_LONG(_18, i);
			zephir_call_method_p1_cache(_7, v, "offsetget", &_22, _18);
			ZEPHIR_INIT_LNVAR(_23);
			mul_function(_23, _1, _7 TSRMLS_CC);
			vv += zephir_get_numberval(_23);
		}
	}
	ZEPHIR_SINIT_VAR(_24);
	ZVAL_LONG(&_24, (vBv / vv));
	zephir_call_func_p1(return_value, "sqrt", &_24);
	RETURN_MM();

}

