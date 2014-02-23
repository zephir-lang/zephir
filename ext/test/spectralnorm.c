
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

	zephir_fcall_cache_entry *_9 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_10 = NULL, *_11 = NULL;
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
					ZEPHIR_INIT_NVAR(_7);
					ZVAL_LONG(_7, i);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, j);
					ZEPHIR_CALL_METHOD(&_6, this_ptr, "ax", &_9, _7, _8);
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(_7);
					ZVAL_LONG(_7, j);
					ZEPHIR_CALL_METHOD(&_10, u, "offsetge", NULL, _7);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_11);
					mul_function(_11, _6, _10 TSRMLS_CC);
					t += zephir_get_numberval(_11);
				}
			}
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			ZEPHIR_INIT_NVAR(_8);
			ZVAL_LONG(_8, t);
			ZEPHIR_CALL_METHOD(NULL, v, "offsetset", NULL, _7, _8);
			zephir_check_call_status();
		}
	}
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, Atu) {

	zephir_fcall_cache_entry *_9 = NULL;
	zend_bool _0, _3;
	zval *n_param = NULL, *u, *v, *_6 = NULL, *_7 = NULL, *_8 = NULL, *_10 = NULL, *_11 = NULL;
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
					ZEPHIR_INIT_NVAR(_7);
					ZVAL_LONG(_7, j);
					ZEPHIR_INIT_NVAR(_8);
					ZVAL_LONG(_8, i);
					ZEPHIR_CALL_METHOD(&_6, this_ptr, "ax", &_9, _7, _8);
					zephir_check_call_status();
					ZEPHIR_INIT_NVAR(_7);
					ZVAL_LONG(_7, j);
					ZEPHIR_CALL_METHOD(&_10, u, "offsetget", NULL, _7);
					zephir_check_call_status();
					ZEPHIR_INIT_LNVAR(_11);
					mul_function(_11, _6, _10 TSRMLS_CC);
					t += zephir_get_numberval(_11);
				}
			}
			ZEPHIR_INIT_NVAR(_7);
			ZVAL_LONG(_7, i);
			ZEPHIR_INIT_NVAR(_8);
			ZVAL_LONG(_8, t);
			ZEPHIR_CALL_METHOD(NULL, v, "offsetset", NULL, _7, _8);
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



	ZEPHIR_CALL_METHOD(NULL, this_ptr, "au", NULL, n, u, w);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "atu", NULL, n, w, v);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_SpectralNorm, process) {

	zephir_fcall_cache_entry *_5 = NULL, *_6 = NULL, *_7 = NULL, *_11 = NULL, *_12 = NULL, *_17 = NULL, *_19 = NULL, *_21 = NULL, *_22 = NULL;
	zend_bool _1, _8, _13;
	zval *n_param = NULL, *u, *v, *w, *_0 = NULL, *_4 = NULL, *_16 = NULL, *_18 = NULL, *_20 = NULL, *_23 = NULL, _24;
	int n, i, vv = 0, vBv = 0, ZEPHIR_LAST_CALL_STATUS, _2, _3, _9, _10, _14, _15;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	ZEPHIR_INIT_VAR(u);
	object_init_ex(u, spl_ce_SplFixedArray);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, n);
	ZEPHIR_CALL_METHOD(NULL, u, "__construct", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(v);
	object_init_ex(v, spl_ce_SplFixedArray);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, n);
	ZEPHIR_CALL_METHOD(NULL, v, "__construct", NULL, _0);
	zephir_check_call_status();
	ZEPHIR_INIT_VAR(w);
	object_init_ex(w, spl_ce_SplFixedArray);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, n);
	ZEPHIR_CALL_METHOD(NULL, w, "__construct", NULL, _0);
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
			ZEPHIR_CALL_METHOD(NULL, u, "offsetset", &_5, _0, _4);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, 1);
			ZEPHIR_CALL_METHOD(NULL, v, "offsetset", &_6, _0, _4);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_INIT_NVAR(_4);
			ZVAL_LONG(_4, 1);
			ZEPHIR_CALL_METHOD(NULL, w, "offsetset", &_7, _0, _4);
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
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "atau", &_11, _0, u, v, w);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, n);
			ZEPHIR_CALL_METHOD(NULL, this_ptr, "atau", &_12, _0, v, u, w);
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
			ZVAL_LONG(_0, i);
			ZEPHIR_CALL_METHOD(&_16, u, "offsetget", &_17, _0);
			zephir_check_call_status();
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_CALL_METHOD(&_18, v, "offsetget", &_19, _0);
			zephir_check_call_status();
			ZEPHIR_INIT_LNVAR(_20);
			mul_function(_20, _16, _18 TSRMLS_CC);
			vBv += zephir_get_numberval(_20);
			ZEPHIR_OBS_NVAR(_16);
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_CALL_METHOD(&_16, v, "offsetget", &_21, _0);
			zephir_check_call_status();
			ZEPHIR_OBS_NVAR(_18);
			ZEPHIR_INIT_NVAR(_0);
			ZVAL_LONG(_0, i);
			ZEPHIR_CALL_METHOD(&_18, v, "offsetget", &_22, _0);
			zephir_check_call_status();
			ZEPHIR_INIT_LNVAR(_23);
			mul_function(_23, _16, _18 TSRMLS_CC);
			vv += zephir_get_numberval(_23);
		}
	}
	ZEPHIR_SINIT_VAR(_24);
	ZVAL_LONG(&_24, (vBv / vv));
	ZEPHIR_RETURN_CALL_FUNCTION("sqrt", NULL, &_24);
	zephir_check_call_status();
	RETURN_MM();

}

