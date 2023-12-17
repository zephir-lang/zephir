
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/array.h"


/**
 * Method calls
 */
ZEPHIR_INIT_CLASS(Stub_Mcall)
{
	ZEPHIR_REGISTER_CLASS(Stub, Mcall, stub, mcall, stub_mcall_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Mcall, testMethod1)
{

	RETURN_STRING("hello public");
}

PHP_METHOD(Stub_Mcall, testMethod2)
{

	RETURN_STRING("hello protected");
}

PHP_METHOD(Stub_Mcall, testMethod3)
{

	RETURN_STRING("hello private");
}

PHP_METHOD(Stub_Mcall, testMethod4)
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

PHP_METHOD(Stub_Mcall, testMethod5)
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

PHP_METHOD(Stub_Mcall, testMethod6)
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

PHP_METHOD(Stub_Mcall, testMethod7)
{

	object_init(return_value);
	return;
}

PHP_METHOD(Stub_Mcall, testCall1)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall2)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall3)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod3", NULL, 56);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall4)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall5)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall6)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod6", NULL, 57, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall7)
{
	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, _0, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod4", &_1, 0, a, b);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "testmethod4", &_1, 0, a, b);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_2);
	return;
}

PHP_METHOD(Stub_Mcall, testCall8)
{
	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, _0, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod5", &_1, 0, a, b);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "testmethod5", &_1, 0, a, b);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_2);
	return;
}

PHP_METHOD(Stub_Mcall, testCall9)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, _0, _1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod6", NULL, 57, a, b);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_1);
	return;
}

PHP_METHOD(Stub_Mcall, testCall10)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall11)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall12)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod3", NULL, 56);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall13)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall14)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall15)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod6", NULL, 57, a, b);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall16)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *c, c_sub, *d, d_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&c_sub);
	ZVAL_UNDEF(&d_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(c)
		Z_PARAM_ZVAL(d)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &c, &d);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, c, d);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall17)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *c, c_sub, *d, d_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&c_sub);
	ZVAL_UNDEF(&d_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(c)
		Z_PARAM_ZVAL(d)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 2, 0, &c, &d);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, c, d);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCall18)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod7", NULL, 58);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testMethod19)
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

PHP_METHOD(Stub_Mcall, testCall20)
{
	zend_bool _0;
	zephir_fcall_cache_entry *_4 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;
	zval *this_ptr = getThis();

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
			ZEPHIR_CALL_METHOD(&_3$$3, this_ptr, "testmethod19", &_4, 0, p, p);
			zephir_check_call_status();
			j += zephir_get_numberval(&_3$$3);
		}
	}
	RETURN_MM_LONG(j);
}

PHP_METHOD(Stub_Mcall, testMethod21)
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

PHP_METHOD(Stub_Mcall, testCall22)
{
	zend_bool _0;
	zephir_fcall_cache_entry *_4 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;
	zval *this_ptr = getThis();

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
			ZEPHIR_CALL_METHOD(&_3$$3, this_ptr, "testmethod21", &_4, 59, p, p);
			zephir_check_call_status();
			j += zephir_get_numberval(&_3$$3);
		}
	}
	RETURN_MM_LONG(j);
}

PHP_METHOD(Stub_Mcall, optionalRequereString)
{
	zval *param_param = NULL;
	zval param;

	ZVAL_UNDEF(&param);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &param_param);
	if (UNEXPECTED(Z_TYPE_P(param_param) != IS_STRING && Z_TYPE_P(param_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'param' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(param_param) == IS_STRING)) {
		zephir_get_strval(&param, param_param);
	} else {
		ZVAL_NULL(&param);
	}
	RETURN_CTOR(&param);
}

PHP_METHOD(Stub_Mcall, optionalParameterString)
{
	zval *param_param = NULL;
	zval param;

	ZVAL_UNDEF(&param);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &param_param);
	if (!param_param) {
		ZVAL_NULL(&param);
		ZVAL_STRING(&param, "test string");
	} else {
		zephir_get_strval(&param, param_param);
	}
	RETURN_CTOR(&param);
}

PHP_METHOD(Stub_Mcall, optionalParameterStringNull)
{
	zval *param_param = NULL;
	zval param;

	ZVAL_UNDEF(&param);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &param_param);
	if (!param_param) {
		ZVAL_NULL(&param);
	} else {
		zephir_get_strval(&param, param_param);
	}
	RETURN_CTOR(&param);
}

PHP_METHOD(Stub_Mcall, optionalParameterInt)
{
	zval *param_param = NULL;
	zend_long param;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_LONG(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 2;
	} else {
		}
	RETURN_LONG(param);
}

PHP_METHOD(Stub_Mcall, optionalParameterVar)
{
	zval *param = NULL, param_sub, __$null;

	ZVAL_UNDEF(&param_sub);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param);
	if (!param) {
		param = &param_sub;
		param = &__$null;
	}
	RETVAL_ZVAL(param, 1, 0);
	return;
}

PHP_METHOD(Stub_Mcall, optionalParameterBoolTrue)
{
	zval *param_param = NULL;
	zend_bool param;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 1;
	} else {
		}
	RETURN_BOOL(param);
}

PHP_METHOD(Stub_Mcall, optionalParameterBoolFalse)
{
	zval *param_param = NULL;
	zend_bool param;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param_param);
	if (!param_param) {
		param = 0;
	} else {
		}
	RETURN_BOOL(param);
}

PHP_METHOD(Stub_Mcall, optionalParameterBoolean)
{
	zval *start_param = NULL;
	zend_bool start;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(start)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &start_param);
	if (!start_param) {
		start = 1;
	} else {
	if (UNEXPECTED(Z_TYPE_P(start_param) != IS_TRUE && Z_TYPE_P(start_param) != IS_FALSE)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'start' must be of the type bool"));
		RETURN_NULL();
	}
	start = (Z_TYPE_P(start_param) == IS_TRUE);
	}
	RETURN_BOOL(start);
}

PHP_METHOD(Stub_Mcall, optionalParameterBooleanNull)
{
	zval *value_param = NULL;
	zend_bool value;

	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL_OR_NULL(value, is_null_true)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &value_param);
	if (!value_param) {
		value = 0;
	} else {
	if (UNEXPECTED(Z_TYPE_P(value_param) != IS_TRUE && Z_TYPE_P(value_param) != IS_FALSE)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'value' must be of the type bool"));
		RETURN_NULL();
	}
	value = (Z_TYPE_P(value_param) == IS_TRUE);
	}
	RETURN_BOOL(value);
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/142
 */
PHP_METHOD(Stub_Mcall, testArrayParamWithDefaultEmptyArray)
{
	zval *driverOptions_param = NULL;
	zval driverOptions;

	ZVAL_UNDEF(&driverOptions);
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY(driverOptions)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &driverOptions_param);
	if (!driverOptions_param) {
		ZVAL_NULL(&driverOptions);
		array_init(&driverOptions);
	} else {
		zephir_get_arrval(&driverOptions, driverOptions_param);
	}
	RETURN_CTOR(&driverOptions);
}

PHP_METHOD(Stub_Mcall, testArrayParamWithDefaultNullValue)
{
	zval *driverOptions_param = NULL;
	zval driverOptions;

	ZVAL_UNDEF(&driverOptions);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ARRAY_OR_NULL(driverOptions)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 0, 1, &driverOptions_param);
	if (!driverOptions_param) {
		ZVAL_NULL(&driverOptions);
	} else {
		zephir_get_arrval(&driverOptions, driverOptions_param);
	}
	RETURN_CTOR(&driverOptions);
}

PHP_METHOD(Stub_Mcall, testArrayParam)
{
	zval *driverOptions_param = NULL;
	zval driverOptions;

	ZVAL_UNDEF(&driverOptions);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(driverOptions)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &driverOptions_param);
	zephir_get_arrval(&driverOptions, driverOptions_param);
	RETURN_CTOR(&driverOptions);
}

PHP_METHOD(Stub_Mcall, testObjectParamCastStdClass)
{
	zval *param, param_sub;

	ZVAL_UNDEF(&param_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(param, zend_standard_class_def)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &param);
	RETVAL_ZVAL(param, 1, 0);
	return;
}

PHP_METHOD(Stub_Mcall, testObjectParamCastOoParam)
{
	zval *param, param_sub;

	ZVAL_UNDEF(&param_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(param, stub_oo_param_ce)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &param);
	RETVAL_ZVAL(param, 1, 0);
	return;
}

PHP_METHOD(Stub_Mcall, bb)
{

	RETURN_STRING("hello");
}

PHP_METHOD(Stub_Mcall, testCallablePass)
{
	zval a, _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&a);
	object_init_ex(&a, stub_oo_ooparams_ce);
	if (zephir_has_constructor(&a)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	ZVAL_NULL(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_closure_ex(&_0, NULL, stub_12__closure_ce, SL("__invoke"));
	ZEPHIR_RETURN_CALL_METHOD(&a, "setcallable", NULL, 60, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, testCallableArrayThisMethodPass)
{
	zval _0;
	zval a, _1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&a);
	object_init_ex(&a, stub_oo_ooparams_ce);
	if (zephir_has_constructor(&a)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	ZVAL_NULL(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, this_ptr);
	ZVAL_NULL(&_1);
	ZVAL_STRING(&_1, "bb");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_RETURN_CALL_METHOD(&a, "setcallable", NULL, 60, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Mcall, aa)
{
	zval a;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&a);
	ZVAL_NULL(&a);
	object_init_ex(&a, stub_mcall_ce);
	if (zephir_has_constructor(&a)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	ZEPHIR_RETURN_CALL_METHOD(&a, "bb", NULL, 61);
	zephir_check_call_status();
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1136
 */
PHP_METHOD(Stub_Mcall, issue1136)
{
	zval _finfo, _0, _1, _2, _3;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_finfo);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_NULL(&_0);
	ZEPHIR_GET_CONSTANT(&_0, "PHP_VERSION");
	ZVAL_NULL(&_1);
	ZVAL_STRING(&_1, "8.0.0");
	ZVAL_NULL(&_2);
	ZVAL_STRING(&_2, ">=");
	ZEPHIR_CALL_FUNCTION(&_3, "version_compare", NULL, 62, &_0, &_1, &_2);
	zephir_check_call_status();
	if (zephir_is_true(&_3)) {
		ZVAL_NULL(&_finfo);
		object_init_ex(&_finfo, zephir_get_internal_ce(SL("finfo")));
		ZEPHIR_CALL_METHOD(NULL, &_finfo, "__construct", NULL, 0);
		zephir_check_call_status();
	} else {
		ZEPHIR_CALL_FUNCTION(&_finfo, "finfo_open", NULL, 63);
		zephir_check_call_status();
	}
	RETURN_CCTOR(&_finfo);
}

PHP_METHOD(Stub_Mcall, issue2245VarArgumentNullable)
{
	zval *param = NULL, param_sub, __$null;

	ZVAL_UNDEF(&param_sub);
	ZVAL_NULL(&__$null);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(0, 1, &param);
	if (!param) {
		param = &param_sub;
		param = &__$null;
	}
	RETVAL_ZVAL(param, 1, 0);
	return;
}

