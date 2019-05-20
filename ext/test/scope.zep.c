
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
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Test_Scope) {

	ZEPHIR_REGISTER_CLASS(Test, Scope, test, scope, test_scope_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Scope, getStr) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("internal_string");

}

PHP_METHOD(Test_Scope, getDyStr) {

	zval *g_param = NULL, _0;
	zend_long g;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&_0);

	zephir_fetch_params(0, 1, 0, &g_param);

	g = zephir_get_intval(g_param);


	ZVAL_LONG(&_0, g);
	ZEPHIR_CONCAT_SV(return_value, "internal_", &_0);
	RETURN_MM();

}

PHP_METHOD(Test_Scope, test1) {

	zval ret, k, k$$3;
	zend_long ZEPHIR_LAST_CALL_STATUS, r;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&k$$3);


	ZEPHIR_MM_ZVAL_STRING(&ret, "");
	ZEPHIR_CALL_SELF(&k, "getstr", &_0, 70);
	zephir_check_call_status();
	r = 1;
	if (r == 1) {
		ZEPHIR_MM_ZVAL_STRING(&k$$3, "test");
		ZEPHIR_MM_CONCAT_SELF(&ret, &k$$3);
	}
	ZEPHIR_CONCAT_VV(return_value, &ret, &k);
	RETURN_MM();

}

PHP_METHOD(Test_Scope, test2) {

	zend_bool _0;
	zval k, c, _5, _3$$3, _4$$3;
	zend_long p, _1, _2, p$$3;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);


	p = 15;
	ZEPHIR_MM_ZVAL_STRING(&k, "");
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
			ZVAL_LONG(&c, _1);
			p$$3 = 66;
			ZVAL_LONG(&_3$$3, ((zephir_get_numberval(&c) * p$$3)));
			ZEPHIR_CONCAT_VVS(&_4$$3, &k, &_3$$3, "_");
			ZEPHIR_MM_ADD_ENTRY(&_4$$3);
			ZEPHIR_CPY_WRT(&k, &_4$$3);
		}
	}
	zephir_create_array(return_value, 2, 0);
	ZVAL_LONG(&_5, p);
	zephir_array_fast_append(return_value, &_5);
	zephir_array_fast_append(return_value, &k);
	RETURN_MM();

}

PHP_METHOD(Test_Scope, test3) {

	zend_bool _0;
	zval k, c, str$$3;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zephir_fcall_cache_entry *_3 = NULL;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&str$$3);


	ZEPHIR_MM_ZVAL_STRING(&k, "");
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
			ZVAL_LONG(&c, _1);
			ZEPHIR_CALL_SELF(&str$$3, "getdystr", &_3, 71, &c);
			zephir_check_call_status();
			ZEPHIR_MM_CONCAT_SELF(&k, &str$$3);
		}
	}
	RETURN_MM_CTOR(&k);

}

