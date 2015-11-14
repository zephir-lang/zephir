
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Scope) {

	ZEPHIR_REGISTER_CLASS(Test, Scope, test, scope, test_scope_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Scope, getStr) {

	

	RETURN_STRING("internal_string", 1);

}

PHP_METHOD(Test_Scope, getDyStr) {

	zval *g_param = NULL, _0;
	int g;

	zephir_fetch_params(0, 1, 0, &g_param);

	g = zephir_get_intval(g_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, g);
	ZEPHIR_CONCAT_SV(return_value, "internal_", &_0);
	return;

}

PHP_METHOD(Test_Scope, test1) {

	zval *ret = NULL, *k = NULL, *k$$3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS, r;
	zephir_fcall_cache_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(ret);
	ZVAL_STRING(ret, "", 1);
	ZEPHIR_CALL_SELF(&k, "getstr", &_0, 64);
	zephir_check_call_status();
	r = 1;
	if (r == 1) {
		ZEPHIR_INIT_VAR(k$$3);
		ZVAL_STRING(k$$3, "test", 1);
		zephir_concat_self(&ret, k$$3 TSRMLS_CC);
	}
	ZEPHIR_CONCAT_VV(return_value, ret, k);
	RETURN_MM();

}

PHP_METHOD(Test_Scope, test2) {

	zend_bool _0;
	zval *k = NULL, *c = NULL, *_5, _3$$3 = zval_used_for_init, *_4$$3 = NULL;
	int p, _1, _2, p$$3;

	ZEPHIR_MM_GROW();

	p = 15;
	ZEPHIR_INIT_VAR(k);
	ZVAL_STRING(k, "", 1);
	_2 = 3;
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
			ZEPHIR_INIT_NVAR(c);
			ZVAL_LONG(c, _1);
			p$$3 = 66;
			ZEPHIR_SINIT_NVAR(_3$$3);
			ZVAL_LONG(&_3$$3, ((zephir_get_numberval(c) * p$$3)));
			ZEPHIR_INIT_LNVAR(_4$$3);
			ZEPHIR_CONCAT_VVS(_4$$3, k, &_3$$3, "_");
			ZEPHIR_CPY_WRT(k, _4$$3);
		}
	}
	zephir_create_array(return_value, 2, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_5);
	ZVAL_LONG(_5, p);
	zephir_array_fast_append(return_value, _5);
	zephir_array_fast_append(return_value, k);
	RETURN_MM();

}

PHP_METHOD(Test_Scope, test3) {

	zend_bool _0;
	zval *k = NULL, *c = NULL, *str$$3 = NULL;
	int ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zephir_fcall_cache_entry *_3 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(k);
	ZVAL_STRING(k, "", 1);
	_2 = 3;
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
			ZEPHIR_INIT_NVAR(c);
			ZVAL_LONG(c, _1);
			ZEPHIR_CALL_SELF(&str$$3, "getdystr", &_3, 65, c);
			zephir_check_call_status();
			zephir_concat_self(&k, str$$3 TSRMLS_CC);
		}
	}
	RETURN_CCTOR(k);

}

