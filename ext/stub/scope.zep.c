
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
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "kernel/array.h"


ZEPHIR_INIT_CLASS(Stub_Scope)
{
	ZEPHIR_REGISTER_CLASS(Stub, Scope, stub, scope, stub_scope_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Scope, getStr)
{
	zval *this_ptr = getThis();



	RETURN_STRING("internal_string");
}

PHP_METHOD(Stub_Scope, getDyStr)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *g_param = NULL, _0;
	zend_long g;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(g)
	ZEND_PARSE_PARAMETERS_END();
#endif


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &g_param);
	g = zephir_get_intval(g_param);


	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, g);
	ZEPHIR_CONCAT_SV(return_value, "internal_", &_0);
	RETURN_MM();
}

PHP_METHOD(Stub_Scope, test1)
{
	zval ret, k, k$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, r;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&k$$3);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&ret);
	ZVAL_STRING(&ret, "");
	ZEPHIR_CALL_SELF(&k, "getstr", &_0, 88);
	zephir_check_call_status();
	r = 1;
	if (r == 1) {
		ZEPHIR_INIT_VAR(&k$$3);
		ZVAL_STRING(&k$$3, "test");
		zephir_concat_self(&ret, &k$$3);
	}
	ZEPHIR_CONCAT_VV(return_value, &ret, &k);
	RETURN_MM();
}

PHP_METHOD(Stub_Scope, test2)
{
	zend_bool _0;
	zval k, c, _5, _3$$3, _4$$3;
	zend_long p, _1, _2, p$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);


	ZEPHIR_MM_GROW();

	p = 15;
	ZEPHIR_INIT_VAR(&k);
	ZVAL_STRING(&k, "");
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
			ZEPHIR_INIT_NVAR(&c);
			ZVAL_LONG(&c, _1);
			p$$3 = 66;
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZVAL_LONG(&_3$$3, ((zephir_get_numberval(&c) * p$$3)));
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZEPHIR_CONCAT_VVS(&_4$$3, &k, &_3$$3, "_");
			ZEPHIR_CPY_WRT(&k, &_4$$3);
		}
	}
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_5);
	ZVAL_LONG(&_5, p);
	zephir_array_fast_append(return_value, &_5);
	zephir_array_fast_append(return_value, &k);
	RETURN_MM();
}

PHP_METHOD(Stub_Scope, test3)
{
	zend_bool _0;
	zval k, c, str$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1, _2;
	zephir_fcall_cache_entry *_3 = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&str$$3);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&k);
	ZVAL_STRING(&k, "");
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
			ZEPHIR_INIT_NVAR(&c);
			ZVAL_LONG(&c, _1);
			ZEPHIR_CALL_SELF(&str$$3, "getdystr", &_3, 89, &c);
			zephir_check_call_status();
			zephir_concat_self(&k, &str$$3);
		}
	}
	RETURN_CCTOR(&k);
}

