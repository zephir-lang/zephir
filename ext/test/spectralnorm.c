
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
#include "ext/spl/spl_fixedarray.h"


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

	zend_function *_9 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_10 = NULL;
	int n, t, i, j, _1, _2, _4, _5, ZEPHIR_LAST_CALL_STATUS;

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
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1(_7, u, "offsetge", _8);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_10);
					mul_function(_10, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_10);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZVAL_LONG(_6, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, t);
			zephir_call_method_p2_noret(v, "offsetset", _6, _7);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, Atu) {

	zend_function *_9 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_10 = NULL;
	int n, t, i, j, _1, _2, _4, _5, ZEPHIR_LAST_CALL_STATUS;

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
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(_7);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					zephir_call_method_p1(_7, u, "offsetget", _8);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_10);
					mul_function(_10, _6, _7 TSRMLS_CC);
					t += zephir_get_numberval(_10);
				}
			}
			ZEPHIR_INIT_NVAR(_6);
			ZVAL_LONG(_6, i);
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, t);
			zephir_call_method_p2_noret(v, "offsetset", _6, _7);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, AtAu) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *n, *u, *v, *w;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 4, 0, &n, &u, &v, &w);



	zephir_call_method_p3_noret(this_ptr, "au", n, u, w);
	zephir_check_call_status();
	zephir_call_method_p3_noret(this_ptr, "atu", n, w, v);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, process) {

	zend_function *_5 = NULL, *_6 = NULL, *_7 = NULL, *_11 = NULL, *_12 = NULL, *_16 = NULL, *_18 = NULL, *_20 = NULL, *_21 = NULL;
	zend_bool _1, _8, _13;
	zval *n_param = NULL, *u, *v, *w, *_0 = NULL, *_4 = NULL, *_17 = NULL, *_19 = NULL, *_22 = NULL, _23;
	int n, i, vv = 0, vBv = 0, ZEPHIR_LAST_CALL_STATUS, _2, _3, _9, _10, _14, _15;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	ZEPHIR_INIT_VAR(u);
	object_init_ex(u, spl_ce_SplFixedArray);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, n);
	zephir_call_method_p1_noret(u, "__construct", _0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(v);
	object_init_ex(v, spl_ce_SplFixedArray);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, n);
	zephir_call_method_p1_noret(v, "__construct", _0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(w);
	object_init_ex(w, spl_ce_SplFixedArray);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, n);
	zephir_call_method_p1_noret(w, "__construct", _0);
	zephir_check_call_status();
	_3 = (n - 1);
	_2 = 0;
	_1 = 0;
	if ((_2 <= _3)) {
		while (1) {
			if (_1) {
				_2++;
				if (!((_2 <= _3))) {
					break;
				}
			} else {
				_1 = 1;
			}
			i = _2;
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, 1);
			zephir_call_method_p2_cache_noret(u, "offsetset", &_5, _0, _4);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, 1);
			zephir_call_method_p2_cache_noret(v, "offsetset", &_6, _0, _4);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, 1);
			zephir_call_method_p2_cache_noret(w, "offsetset", &_7, _0, _4);
			zephir_check_call_status();
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
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, n);
			zephir_call_method_p4_cache_noret(this_ptr, "atau", &_11, _0, u, v, w);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, n);
			zephir_call_method_p4_cache_noret(this_ptr, "atau", &_12, _0, v, u, w);
			zephir_check_call_status();
		}
	}
	_15 = (n - 1);
	_14 = 0;
	_13 = 0;
	if ((_14 <= _15)) {
		while (1) {
			if (_13) {
				_14++;
				if (!((_14 <= _15))) {
					break;
				}
			} else {
				_13 = 1;
			}
			i = _14;
			ZEPHIR_INIT_NVAR(_0);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, i);
			zephir_call_method_p1_cache(_0, u, "offsetget", &_16, _4);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_4);
			ZEPHIR_INIT_NVAR(_17);
			ZVAL_LONG(_17, i);
			zephir_call_method_p1_cache(_4, v, "offsetget", &_18, _17);
			zephir_check_call_status();
			ZEPHIR_INIT_LNVAR(_19);
			mul_function(_19, _0, _4 TSRMLS_CC);
			vBv += zephir_get_numberval(_19);
			ZEPHIR_INIT_NVAR(_0);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, i);
			zephir_call_method_p1_cache(_0, v, "offsetget", &_20, _4);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_4);
			ZEPHIR_INIT_NVAR(_17);
			ZVAL_LONG(_17, i);
			zephir_call_method_p1_cache(_4, v, "offsetget", &_21, _17);
			zephir_check_call_status();
			ZEPHIR_INIT_LNVAR(_22);
			mul_function(_22, _0, _4 TSRMLS_CC);
			vv += zephir_get_numberval(_22);
		}
	}
	ZEPHIR_SINIT_VAR(_23);
	ZVAL_LONG(&_23, (vBv / vv));
	zephir_call_func_p1(return_value, "sqrt", &_23);
	zephir_check_call_status();
	RETURN_MM();

}

