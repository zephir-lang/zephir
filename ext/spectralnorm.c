
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

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

	ZEPHIR_REGISTER_CLASS(Test, SpectralNorm, spectralnorm, test_spectralnorm_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_SpectralNorm, Ax) {

	zval *i, *j, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &i, &j);



	ZEPHIR_INIT_VAR(_0);
	zephir_add_function(_0, i, j TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	zephir_add_function(_1, i, j TSRMLS_CC);
	RETURN_MM_LONG((1 / ((((zephir_get_numberval(_0) * (zephir_get_numberval(_1) + 1)) / 2) + zephir_get_intval(i)) + 1)));

}

PHP_METHOD(Test_SpectralNorm, Au) {

	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_9 = NULL;
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
					zephir_call_method_p2(_6, this_ptr, "ax", _7, _8);
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1(_7, u, "offsetge", _8);
					ZEPHIR_INIT_LNVAR(_9);
					mul_function(_9, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_9);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZVAL_LONG(_6, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, t);
			zephir_call_method_p2_noret(v, "offsetset", _6, _7);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, Atu) {

	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_9 = NULL;
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
					zephir_call_method_p2(_6, this_ptr, "ax", _7, _8);
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1(_7, u, "offsetget", _8);
					ZEPHIR_INIT_LNVAR(_9);
					mul_function(_9, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_9);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZVAL_LONG(_6, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, t);
			zephir_call_method_p2_noret(v, "offsetset", _6, _7);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, AtAu) {

	zval *n, *u, *v, *w;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &n, &u, &v, &w);



	zephir_call_method_p3_noret(this_ptr, "au", n, u, w);
	zephir_call_method_p3_noret(this_ptr, "atu", n, w, v);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, process) {

	zend_bool _4, _8, _11;
	zend_class_entry *_0, *_2, *_3;
	zval *n_param = NULL, *u = NULL, *v = NULL, *w = NULL, *_1 = NULL, *_7 = NULL, *_14 = NULL, *_15 = NULL, *_16 = NULL, _17 = zval_used_for_init;
	int n, i, vv = 0, vBv = 0, _5, _6, _9, _10, _12, _13;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

		n = zephir_get_intval(n_param);


	ZEPHIR_INIT_NVAR(u);
	_0 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(u, _0);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, n);
	zephir_call_method_p1_noret(u, "__construct", _1);
	ZEPHIR_INIT_NVAR(v);
	_2 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(v, _2);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, n);
	zephir_call_method_p1_noret(v, "__construct", _1);
	ZEPHIR_INIT_NVAR(w);
	_3 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(w, _3);
	ZEPHIR_INIT_NVAR(_1);
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
			zephir_call_method_p2_noret(u, "offsetset", _1, _7);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, 1);
			zephir_call_method_p2_noret(v, "offsetset", _1, _7);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, 1);
			zephir_call_method_p2_noret(w, "offsetset", _1, _7);
		}
	}
	_10 = 9;
	_9 = 0;
	_8 = 0;
	if ((_9 <= _10)) {
		while (1) {
			if (_8) {
				_9++;
				if (!((_9 <= _10))) {
					break;
				}
			} else {
				_8 = 1;
			}
			i = _9;
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, n);
			zephir_call_method_p4_noret(this_ptr, "atau", _1, u, v, w);
			ZEPHIR_INIT_NVAR(_1);
			ZVAL_LONG(_1, n);
			zephir_call_method_p4_noret(this_ptr, "atau", _1, v, u, w);
		}
	}
	_13 = (n - 1);
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
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			zephir_call_method_p1(_1, u, "offsetget", _7);
			ZEPHIR_INIT_NVAR(_7);
			ZEPHIR_INIT_NVAR(_14);
			ZVAL_LONG(_14, i);
			zephir_call_method_p1(_7, v, "offsetget", _14);
			ZEPHIR_INIT_LNVAR(_15);
			mul_function(_15, _1, _7 TSRMLS_CC);
			vBv += zephir_get_numberval(_15);
			ZEPHIR_INIT_NVAR(_1);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			zephir_call_method_p1(_1, v, "offsetget", _7);
			ZEPHIR_INIT_NVAR(_7);
			ZEPHIR_INIT_NVAR(_14);
			ZVAL_LONG(_14, i);
			zephir_call_method_p1(_7, v, "offsetget", _14);
			ZEPHIR_INIT_LNVAR(_16);
			mul_function(_16, _1, _7 TSRMLS_CC);
			vv += zephir_get_numberval(_16);
		}
	}
	ZEPHIR_SINIT_NVAR(_17);
	ZVAL_LONG(&_17, (vBv / vv));
	zephir_call_func_p1(return_value, "sqrt", &_17);
	RETURN_MM();

}

