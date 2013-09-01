
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
#include "kernel/object.h"


/**
 * SpectralNorm
 *
 * @see http://mathworld.wolfram.com/SpectralNorm.html
 */
ZEPHIR_INIT_CLASS(Test_SpectralNorm) {

	ZEPHIR_REGISTER_CLASS(Test, SpectralNorm, spectralnorm, test_spectralnorm_method_entry, 0);

	zend_declare_property_null(test_spectralnorm_ce, SL("n"), ZEND_ACC_PUBLIC TSRMLS_CC);

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
	RETURN_MM_LONG(1 / zephir_get_numberval(_0) * zephir_get_numberval(_1) + 1 / 2 + zephir_get_intval(i) + 1);

}

PHP_METHOD(Test_SpectralNorm, Au) {

	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_9 = NULL, *_10 = NULL, *_11 = NULL;
	int n, t, i, j, _1, _2, _4, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);

		n = zephir_get_intval(n_param);


	_2 = n - 1;
	_1 = 0;
	_0 = 0;
	while (1 && (_1 <= _2)) {
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
		_5 = n - 1;
		_4 = 0;
		_3 = 0;
		while (1 && (_4 <= _5)) {
			if (_3) {
				_4++;
				if (!(_4 <= _5)) {
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
			ZEPHIR_INIT_NVAR(_9);
			ZEPHIR_INIT_NVAR(_10);
			ZVAL_LONG(_10, j);
			zephir_call_method_p1(_9, u, "offsetge", _10);
			ZEPHIR_INIT_LNVAR(_11);
			mul_function(_11, _6, _9 TSRMLS_CC);
			t += zephir_get_numberval(_11);
		}
		ZEPHIR_INIT_NVAR(_6);
		ZVAL_LONG(_6, i);
		ZEPHIR_INIT_NVAR(_7);
		ZVAL_LONG(_7, t);
		zephir_call_method_p2_noret(v, "offsetset", _6, _7);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, Atu) {

	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_9 = NULL, *_10 = NULL, *_11 = NULL;
	int n, t, i, j, _1, _2, _4, _5;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 3, 0, &n_param, &u, &v);

		n = zephir_get_intval(n_param);


	_2 = n - 1;
	_1 = 0;
	_0 = 0;
	while (1 && (_1 <= _2)) {
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
		_5 = n - 1;
		_4 = 0;
		_3 = 0;
		while (1 && (_4 <= _5)) {
			if (_3) {
				_4++;
				if (!(_4 <= _5)) {
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
			ZEPHIR_INIT_NVAR(_9);
			ZEPHIR_INIT_NVAR(_10);
			ZVAL_LONG(_10, j);
			zephir_call_method_p1(_9, u, "offsetget", _10);
			ZEPHIR_INIT_LNVAR(_11);
			mul_function(_11, _6, _9 TSRMLS_CC);
			t += zephir_get_numberval(_11);
		}
		ZEPHIR_INIT_NVAR(_6);
		ZVAL_LONG(_6, i);
		ZEPHIR_INIT_NVAR(_7);
		ZVAL_LONG(_7, t);
		zephir_call_method_p2_noret(v, "offsetset", _6, _7);
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, AtAu) {

	zval *n, *u, *v, *w;

	zephir_fetch_params(0, 4, 0, &n, &u, &v, &w);



	zephir_call_method_p3_noret(this_ptr, "au", n, u, w);
	zephir_call_method_p3_noret(this_ptr, "atu", n, w, v);

}

PHP_METHOD(Test_SpectralNorm, process) {

	zend_bool _7, _16, _19;
	zend_class_entry *_0, *_2, *_4;
	zval *n_param = NULL, *u = NULL, *v = NULL, *w = NULL, *_1 = NULL, *_3 = NULL, *_5 = NULL, *_6 = NULL, *_10 = NULL, *_11 = NULL, *_12 = NULL, *_13 = NULL, *_14 = NULL, *_15 = NULL, *_22 = NULL, *_23 = NULL, *_24 = NULL, *_25 = NULL, _26 = zval_used_for_init;
	int n, i, vv = 0, vBv = 0, _8, _9, _17, _18, _20, _21;

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
	ZEPHIR_INIT_NVAR(_3);
	ZVAL_LONG(_3, n);
	zephir_call_method_p1_noret(v, "__construct", _3);
	ZEPHIR_INIT_NVAR(w);
	_4 = zend_fetch_class(SL("SplFixedArray"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	object_init_ex(w, _4);
	ZEPHIR_INIT_NVAR(_5);
	ZVAL_LONG(_5, n);
	zephir_call_method_p1_noret(w, "__construct", _5);
	ZEPHIR_INIT_NVAR(_6);
	ZVAL_LONG(_6, n);
	zephir_update_property_zval(this_ptr, SL("n"), _6 TSRMLS_CC);
	_9 = n - 1;
	_8 = 0;
	_7 = 0;
	while (1 && (_8 <= _9)) {
		if (_7) {
			_8++;
			if (!(_8 <= _9)) {
				break;
			}
		} else {
			_7 = 1;
		}
		i = _8;
		ZEPHIR_INIT_NVAR(_10);
		ZVAL_LONG(_10, i);
		ZEPHIR_INIT_NVAR(_11);
		ZVAL_LONG(_11, 1);
		zephir_call_method_p2_noret(u, "offsetset", _10, _11);
		ZEPHIR_INIT_NVAR(_12);
		ZVAL_LONG(_12, i);
		ZEPHIR_INIT_NVAR(_13);
		ZVAL_LONG(_13, 1);
		zephir_call_method_p2_noret(v, "offsetset", _12, _13);
		ZEPHIR_INIT_NVAR(_14);
		ZVAL_LONG(_14, i);
		ZEPHIR_INIT_NVAR(_15);
		ZVAL_LONG(_15, 1);
		zephir_call_method_p2_noret(w, "offsetset", _14, _15);
	}
	_18 = 9;
	_17 = 0;
	_16 = 0;
	while (1 && (_17 <= _18)) {
		if (_16) {
			_17++;
			if (!(_17 <= _18)) {
				break;
			}
		} else {
			_16 = 1;
		}
		i = _17;
		ZEPHIR_INIT_NVAR(_10);
		ZVAL_LONG(_10, n);
		zephir_call_method_p4_noret(this_ptr, "atau", _10, u, v, w);
		ZEPHIR_INIT_NVAR(_11);
		ZVAL_LONG(_11, n);
		zephir_call_method_p4_noret(this_ptr, "atau", _11, v, u, w);
	}
	_21 = n - 1;
	_20 = 0;
	_19 = 0;
	while (1 && (_20 <= _21)) {
		if (_19) {
			_20++;
			if (!(_20 <= _21)) {
				break;
			}
		} else {
			_19 = 1;
		}
		i = _20;
		ZEPHIR_INIT_NVAR(_10);
		ZEPHIR_INIT_NVAR(_11);
		ZVAL_LONG(_11, i);
		zephir_call_method_p1(_10, u, "offsetget", _11);
		ZEPHIR_INIT_NVAR(_12);
		ZEPHIR_INIT_NVAR(_13);
		ZVAL_LONG(_13, i);
		zephir_call_method_p1(_12, v, "offsetget", _13);
		ZEPHIR_INIT_LNVAR(_22);
		mul_function(_22, _10, _12 TSRMLS_CC);
		vBv += zephir_get_numberval(_22);
		ZEPHIR_INIT_NVAR(_14);
		ZEPHIR_INIT_NVAR(_15);
		ZVAL_LONG(_15, i);
		zephir_call_method_p1(_14, v, "offsetget", _15);
		ZEPHIR_INIT_NVAR(_23);
		ZEPHIR_INIT_NVAR(_24);
		ZVAL_LONG(_24, i);
		zephir_call_method_p1(_23, v, "offsetget", _24);
		ZEPHIR_INIT_LNVAR(_25);
		mul_function(_25, _14, _23 TSRMLS_CC);
		vv += zephir_get_numberval(_25);
	}
	ZEPHIR_SINIT_NVAR(_26);
	ZVAL_LONG(&_26, vBv / vv);
	zephir_call_func_p1(return_value, "sqrt", &_26);
	RETURN_MM();

}

