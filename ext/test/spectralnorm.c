
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

	zend_function *_9 = NULL, *_10 = NULL, *_12 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_11 = NULL;
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
					zephir_call_method_p2_cache(_6, this_ptr, "ax", &_9, _7, _8);
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1_cache(_7, u, "offsetge", &_10, _8);
					ZEPHIR_INIT_LNVAR(_11);
					mul_function(_11, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_11);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			ZEPHIR_INIT_NVAR(_8);
			ZVAL_LONG(_8, t);
			zephir_call_method_p2_cache(_6, v, "offsetset", &_12, _7, _8);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, Atu) {

	zend_function *_9 = NULL, *_10 = NULL, *_12 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_11 = NULL;
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
					zephir_call_method_p2_cache(_6, this_ptr, "ax", &_9, _7, _8);
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1_cache(_7, u, "offsetget", &_10, _8);
					ZEPHIR_INIT_LNVAR(_11);
					mul_function(_11, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_11);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			ZEPHIR_INIT_NVAR(_8);
			ZVAL_LONG(_8, t);
			zephir_call_method_p2_cache(_6, v, "offsetset", &_12, _7, _8);
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, AtAu) {

	zval *n, *u, *v, *w, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &n, &u, &v, &w);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p3(_0, this_ptr, "au", n, u, w);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_method_p3(_1, this_ptr, "atu", n, w, v);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, process) {

	zend_function *_12 = NULL, *_14 = NULL, *_16 = NULL, *_20 = NULL, *_21 = NULL, *_25 = NULL, *_26 = NULL, *_28 = NULL, *_29 = NULL;
	zend_bool _7, _17, _22;
	zend_class_entry *_0, *_3, *_5;
	zval *n_param = NULL, *u, *v, *w, *_1, *_2, *_4, *_6 = NULL, *_10 = NULL, *_11 = NULL, *_13 = NULL, *_15 = NULL, *_27 = NULL, *_30 = NULL, _31;
	int n, i, vv = 0, vBv = 0, _8, _9, _18, _19, _23, _24;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

		n = zephir_get_intval(n_param);


	ZEPHIR_INIT_VAR(u);
	_0 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(u, _0);
	ZEPHIR_INIT_VAR(_1);
	ZEPHIR_INIT_VAR(_2);
	ZVAL_LONG(_2, n);
	zephir_call_method_p1(_1, u, "__construct", _2);
	ZEPHIR_INIT_VAR(v);
	_3 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(v, _3);
	ZEPHIR_INIT_BNVAR(_2);
	ZEPHIR_INIT_VAR(_4);
	ZVAL_LONG(_4, n);
	zephir_call_method_p1(_2, v, "__construct", _4);
	ZEPHIR_INIT_VAR(w);
	_5 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(w, _5);
	ZEPHIR_INIT_BNVAR(_4);
	ZEPHIR_INIT_VAR(_6);
	ZVAL_LONG(_6, n);
	zephir_call_method_p1(_4, w, "__construct", _6);
	_9 = (n - 1);
	_8 = 0;
	_7 = 0;
	if ((_8 <= _9)) {
		while (1) {
			if (_7) {
				_8++;
				if (!((_8 <= _9))) {
					break;
				}
			} else {
				_7 = 1;
			}
			i = _8;
			ZEPHIR_INIT_NVAR(_6);
			ZEPHIR_INIT_NVAR(_10);
			ZVAL_LONG(_10, i);
			ZEPHIR_INIT_NVAR(_11);
			ZVAL_LONG(_11, 1);
			zephir_call_method_p2_cache(_6, u, "offsetset", &_12, _10, _11);
			ZEPHIR_INIT_NVAR(_10);
			ZEPHIR_INIT_NVAR(_11);
			ZVAL_LONG(_11, i);
			ZEPHIR_INIT_NVAR(_13);
			ZVAL_LONG(_13, 1);
			zephir_call_method_p2_cache(_10, v, "offsetset", &_14, _11, _13);
			ZEPHIR_INIT_NVAR(_11);
			ZEPHIR_INIT_NVAR(_13);
			ZVAL_LONG(_13, i);
			ZEPHIR_INIT_NVAR(_15);
			ZVAL_LONG(_15, 1);
			zephir_call_method_p2_cache(_11, w, "offsetset", &_16, _13, _15);
		}
	}
	_19 = 9;
	_18 = 0;
	_17 = 0;
	if ((_18 <= _19)) {
		while (1) {
			if (_17) {
				_18++;
				if (!((_18 <= _19))) {
					break;
				}
			} else {
				_17 = 1;
			}
			i = _18;
			ZEPHIR_INIT_NVAR(_10);
			ZEPHIR_INIT_NVAR(_11);
			ZVAL_LONG(_11, n);
			zephir_call_method_p4_cache(_10, this_ptr, "atau", &_20, _11, u, v, w);
			ZEPHIR_INIT_NVAR(_11);
			ZEPHIR_INIT_NVAR(_13);
			ZVAL_LONG(_13, n);
			zephir_call_method_p4_cache(_11, this_ptr, "atau", &_21, _13, v, u, w);
		}
	}
	_24 = (n - 1);
	_23 = 0;
	_22 = 0;
	if ((_23 <= _24)) {
		while (1) {
			if (_22) {
				_23++;
				if (!((_23 <= _24))) {
					break;
				}
			} else {
				_22 = 1;
			}
			i = _23;
			ZEPHIR_INIT_NVAR(_10);
			ZEPHIR_INIT_NVAR(_11);
			ZVAL_LONG(_11, i);
			zephir_call_method_p1_cache(_10, u, "offsetget", &_25, _11);
			ZEPHIR_INIT_NVAR(_11);
			ZEPHIR_INIT_NVAR(_13);
			ZVAL_LONG(_13, i);
			zephir_call_method_p1_cache(_11, v, "offsetget", &_26, _13);
			ZEPHIR_INIT_LNVAR(_27);
			mul_function(_27, _10, _11 TSRMLS_CC);
			vBv += zephir_get_numberval(_27);
			ZEPHIR_INIT_NVAR(_10);
			ZEPHIR_INIT_NVAR(_11);
			ZVAL_LONG(_11, i);
			zephir_call_method_p1_cache(_10, v, "offsetget", &_28, _11);
			ZEPHIR_INIT_NVAR(_11);
			ZEPHIR_INIT_NVAR(_13);
			ZVAL_LONG(_13, i);
			zephir_call_method_p1_cache(_11, v, "offsetget", &_29, _13);
			ZEPHIR_INIT_LNVAR(_30);
			mul_function(_30, _10, _11 TSRMLS_CC);
			vv += zephir_get_numberval(_30);
		}
	}
	ZEPHIR_SINIT_VAR(_31);
	ZVAL_LONG(&_31, (vBv / vv));
	zephir_call_func_p1(return_value, "sqrt", &_31);
	RETURN_MM();

}

