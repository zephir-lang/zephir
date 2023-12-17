
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Stub_Scall)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Scall, stub, scall, stub_scallparent_ce, stub_scall_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Scall, testMethod1)
{

	RETURN_STRING("hello public");
}

PHP_METHOD(Stub_Scall, testMethod2)
{

	RETURN_STRING("hello protected");
}

PHP_METHOD(Stub_Scall, testMethod3)
{

	RETURN_STRING("hello private");
}

PHP_METHOD(Stub_Scall, testMethod4)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	zephir_add_function(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Scall, testMethod5)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	zephir_add_function(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Scall, testMethod6)
{
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a, &b);
	zephir_add_function(return_value, a, b);
	return;
}

PHP_METHOD(Stub_Scall, testMethod7)
{

	object_init(return_value);
	return;
}

PHP_METHOD(Stub_Scall, testCall1)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_SELF("testmethod1", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall2)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_SELF("testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall3)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	ZEPHIR_RETURN_CALL_SELF("testmethod3", &_0, 91);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall4)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_SELF("testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall5)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_SELF("testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall6)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_SELF("testmethod6", &_0, 92, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall7)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_SELF("testmethod1", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall8)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_SELF("testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall9)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	ZEPHIR_RETURN_CALL_SELF("testmethod3", &_0, 91);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall10)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_SELF("testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall11)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_SELF("testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall12)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_SELF("testmethod6", &_0, 92, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall13)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_PARENT(stub_scall_ce, getThis(), "testmethod1", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall14)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_PARENT(stub_scall_ce, getThis(), "testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testCall15)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_SELF("testmethod7", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, testMethod16)
{
	zval *a_param = NULL, *b_param = NULL;
	long a, b;

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(a)
		Z_PARAM_LONG(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &a_param, &b_param);
	RETURN_LONG((a + b));
}

PHP_METHOD(Stub_Scall, testCall17)
{
	zend_bool _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zephir_fcall_cache_entry *_4 = NULL;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &k_param, &p);
	j = 0;
	_2 = k;
	_1 = 1;
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
			ZEPHIR_CALL_CE_STATIC(&_3$$3, stub_scallexternal_ce, "testmethod3", &_4, 0, p, p);
			zephir_check_call_status();
			j += zephir_get_numberval(&_3$$3);
		}
	}
	RETURN_MM_LONG(j);
}

PHP_METHOD(Stub_Scall, testCall18)
{
	zend_bool _0;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zephir_fcall_cache_entry *_4 = NULL;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;

	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_3$$3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(k)
		Z_PARAM_ZVAL(p)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &k_param, &p);
	j = 0;
	_2 = k;
	_1 = 1;
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
			ZEPHIR_CALL_SELF(&_3$$3, "testmethod16", &_4, 0, p, p);
			zephir_check_call_status();
			j += zephir_get_numberval(&_3$$3);
		}
	}
	RETURN_MM_LONG(j);
}

PHP_METHOD(Stub_Scall, testMethodStatic)
{

	RETURN_STRING("hello Scall");
}

PHP_METHOD(Stub_Scall, interpolatedStaticReturn)
{
	zend_class_entry *_0;
	zval className, methodName;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&methodName);
	ZVAL_NULL(&className);
	ZVAL_STRING(&className, "Stub\\Scall");
	ZVAL_NULL(&methodName);
	ZVAL_STRING(&methodName, "testMethodStatic");
	_0 = zephir_fetch_class(&className);
	ZEPHIR_RETURN_CALL_CE_STATIC_ZVAL(_0, methodName, NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Scall, interpolatedStaticEcho)
{
	zend_class_entry *_1;
	zval _0;
	zval className, methodName;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&className);
	ZVAL_UNDEF(&methodName);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&className);
	ZVAL_STRING(&className, "Stub\\Scall");
	ZVAL_NULL(&methodName);
	ZVAL_STRING(&methodName, "testMethodStatic");
	_1 = zephir_fetch_class(&className);
	ZEPHIR_CALL_CE_STATIC_ZVAL(&_0, _1, methodName, NULL, 0);
	zephir_check_call_status();
	zend_print_zval(&_0, 0);
}

