
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
#include "kernel/operators.h"
#include "kernel/fcall.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/array.h"


/**
 * Method calls
 */
ZEPHIR_INIT_CLASS(Test_Mcall) {

	ZEPHIR_REGISTER_CLASS(Test, Mcall, test, mcall, test_mcall_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Mcall, testMethod1) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("hello public");

}

PHP_METHOD(Test_Mcall, testMethod2) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("hello protected");

}

PHP_METHOD(Test_Mcall, testMethod3) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("hello private");

}

PHP_METHOD(Test_Mcall, testMethod4) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testMethod5) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testMethod6) {

	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testMethod7) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	object_init(return_value);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall1) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall2) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall3) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod3", NULL, 45);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall4) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall5) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall6) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod6", NULL, 46, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall7) {

	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, _0, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod4", &_1, 0, a, b);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "testmethod4", &_1, 0, a, b);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_2);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall8) {

	zephir_fcall_cache_entry *_1 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, _0, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod5", &_1, 0, a, b);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_2, this_ptr, "testmethod5", &_1, 0, a, b);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_2);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall9) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub, _0, _1;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod6", NULL, 46, a, b);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, this_ptr, "testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	zephir_add_function(return_value, &_0, &_1);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall10) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall11) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall12) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod3", NULL, 45);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall13) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall14) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod5", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall15) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);

	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod6", NULL, 46, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall16) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *c, c_sub, *d, d_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&c_sub);
	ZVAL_UNDEF(&d_sub);

	zephir_fetch_params(1, 2, 0, &c, &d);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, c, d);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall17) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *c, c_sub, *d, d_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&c_sub);
	ZVAL_UNDEF(&d_sub);

	zephir_fetch_params(1, 2, 0, &c, &d);



	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod4", NULL, 0, c, d);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall18) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "testmethod7", NULL, 47);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testMethod19) {

	zval *a_param = NULL, *b_param = NULL;
	long a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_MM_LONG((a + b));

}

PHP_METHOD(Test_Mcall, testCall20) {

	zend_bool _0;
	zephir_fcall_cache_entry *_4 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_3$$3);

	zephir_fetch_params(1, 2, 0, &k_param, &p);

	k = zephir_get_intval(k_param);


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

PHP_METHOD(Test_Mcall, testMethod21) {

	zval *a_param = NULL, *b_param = NULL;
	long a, b;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_MM_LONG((a + b));

}

PHP_METHOD(Test_Mcall, testCall22) {

	zend_bool _0;
	zephir_fcall_cache_entry *_4 = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS, _1;
	zval *k_param = NULL, *p, p_sub, _3$$3;
	long k, i = 0, j, _2;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&p_sub);
	ZVAL_UNDEF(&_3$$3);

	zephir_fetch_params(1, 2, 0, &k_param, &p);

	k = zephir_get_intval(k_param);


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
			ZEPHIR_CALL_METHOD(&_3$$3, this_ptr, "testmethod21", &_4, 48, p, p);
			zephir_check_call_status();
			j += zephir_get_numberval(&_3$$3);
		}
	}
	RETURN_MM_LONG(j);

}

PHP_METHOD(Test_Mcall, optionalRequereString) {

	zval *param_param = NULL;
	zval param;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param);

	zephir_fetch_params(1, 1, 0, &param_param);

	if (UNEXPECTED(Z_TYPE_P(param_param) != IS_STRING && Z_TYPE_P(param_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'param' must be of the type string") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(param_param) == IS_STRING)) {
		zephir_get_strval(&param, param_param);
	} else {
		ZEPHIR_MM_ZVAL_EMPTY_STRING(&param);
	}


	RETURN_MM_CTOR(&param);

}

PHP_METHOD(Test_Mcall, optionalParameterString) {

	zval *param_param = NULL;
	zval param;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param);

	zephir_fetch_params(1, 0, 1, &param_param);

	if (!param_param) {
		ZEPHIR_MM_ZVAL_STRING(&param, "test string");
	} else {
		zephir_get_strval(&param, param_param);
	}


	RETURN_MM_CTOR(&param);

}

PHP_METHOD(Test_Mcall, optionalParameterStringNull) {

	zval *param_param = NULL;
	zval param;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param);

	zephir_fetch_params(1, 0, 1, &param_param);

	if (!param_param) {
		ZEPHIR_MM_ZVAL_STRING(&param, "");
	} else {
		zephir_get_strval(&param, param_param);
	}


	RETURN_MM_CTOR(&param);

}

PHP_METHOD(Test_Mcall, optionalParameterInt) {

	zval *param_param = NULL;
	zend_long param;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 0, 1, &param_param);

	if (!param_param) {
		param = 2;
	} else {
		param = zephir_get_intval(param_param);
	}


	RETURN_MM_LONG(param);

}

PHP_METHOD(Test_Mcall, optionalParameterVar) {

	zval *param = NULL, param_sub, __$null;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param_sub);
	ZVAL_NULL(&__$null);

	zephir_fetch_params(1, 0, 1, &param);

	if (!param) {
		param = &param_sub;
		param = &__$null;
	}


	RETURN_MM_CTOR(param);

}

PHP_METHOD(Test_Mcall, optionalParameterBoolTrue) {

	zval *param_param = NULL;
	zend_bool param;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 0, 1, &param_param);

	if (!param_param) {
		param = 1;
	} else {
		param = zephir_get_boolval(param_param);
	}


	RETURN_MM_BOOL(param);

}

PHP_METHOD(Test_Mcall, optionalParameterBoolFalse) {

	zval *param_param = NULL;
	zend_bool param;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 0, 1, &param_param);

	if (!param_param) {
		param = 0;
	} else {
		param = zephir_get_boolval(param_param);
	}


	RETURN_MM_BOOL(param);

}

PHP_METHOD(Test_Mcall, optionalParameterBoolean) {

	zval *start_param = NULL;
	zend_bool start;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 0, 1, &start_param);

	if (!start_param) {
		start = 1;
	} else {
	if (UNEXPECTED(Z_TYPE_P(start_param) != IS_TRUE && Z_TYPE_P(start_param) != IS_FALSE)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'start' must be of the type bool") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	start = (Z_TYPE_P(start_param) == IS_TRUE);
	}


	RETURN_MM_BOOL(start);

}

PHP_METHOD(Test_Mcall, optionalParameterBooleanNull) {

	zval *value_param = NULL;
	zend_bool value;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	zephir_fetch_params(1, 0, 1, &value_param);

	if (!value_param) {
		value = 0;
	} else {
	if (UNEXPECTED(Z_TYPE_P(value_param) != IS_TRUE && Z_TYPE_P(value_param) != IS_FALSE)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'value' must be of the type bool") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	value = (Z_TYPE_P(value_param) == IS_TRUE);
	}


	RETURN_MM_BOOL(value);

}

/**
 * @link https://github.com/phalcon/zephir/issues/142
 */
PHP_METHOD(Test_Mcall, arrayParamWithDefaultEmptyArray) {

	zval *driverOptions_param = NULL;
	zval driverOptions;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&driverOptions);

	zephir_fetch_params(1, 0, 1, &driverOptions_param);

	if (!driverOptions_param) {
		array_init(&driverOptions);
		ZEPHIR_MM_ADD_ENTRY(&driverOptions);
	} else {
		zephir_get_arrval(&driverOptions, driverOptions_param);
	}


	RETURN_MM_CTOR(&driverOptions);

}

PHP_METHOD(Test_Mcall, arrayParamWithDefaultNullValue) {

	zval *driverOptions_param = NULL;
	zval driverOptions;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&driverOptions);

	zephir_fetch_params(1, 0, 1, &driverOptions_param);

	if (!driverOptions_param) {
		array_init(&driverOptions);
		ZEPHIR_MM_ADD_ENTRY(&driverOptions);
	} else {
		zephir_get_arrval(&driverOptions, driverOptions_param);
	}


	RETURN_MM_CTOR(&driverOptions);

}

PHP_METHOD(Test_Mcall, arrayParam) {

	zval *driverOptions_param = NULL;
	zval driverOptions;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&driverOptions);

	zephir_fetch_params(1, 1, 0, &driverOptions_param);

	zephir_get_arrval(&driverOptions, driverOptions_param);


	RETURN_MM_CTOR(&driverOptions);

}

PHP_METHOD(Test_Mcall, objectParamCastStdClass) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param_sub);

	zephir_fetch_params(1, 1, 0, &param);



	RETURN_MM_CTOR(param);

}

PHP_METHOD(Test_Mcall, objectParamCastOoParam) {

	zval *param, param_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&param_sub);

	zephir_fetch_params(1, 1, 0, &param);



	RETURN_MM_CTOR(param);

}

PHP_METHOD(Test_Mcall, bb) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("hello");

}

PHP_METHOD(Test_Mcall, testCallablePass) {

	zval a, _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);


	object_init_ex(&a, test_oo_ooparams_ce);ZEPHIR_MM_ADD_ENTRY(&a);
	if (zephir_has_constructor(&a TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_create_closure_ex(&_0, NULL, test_10__closure_ce, SL("__invoke"));
	ZEPHIR_MM_ADD_ENTRY(&_0);
	ZEPHIR_RETURN_CALL_METHOD(&a, "setcallable", NULL, 49, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCallableArrayThisMethodPass) {

	zval _0;
	zval a, _1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);


	object_init_ex(&a, test_oo_ooparams_ce);ZEPHIR_MM_ADD_ENTRY(&a);
	if (zephir_has_constructor(&a TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_MM_ADD_ENTRY(&_0);
	zephir_array_fast_append(&_0, this_ptr);
	ZEPHIR_MM_ZVAL_STRING(&_1, "bb");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_RETURN_CALL_METHOD(&a, "setcallable", NULL, 49, &_0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, aa) {

	zval a;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&a);


	object_init_ex(&a, test_mcall_ce);ZEPHIR_MM_ADD_ENTRY(&a);
	if (zephir_has_constructor(&a TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &a, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZEPHIR_RETURN_CALL_METHOD(&a, "bb", NULL, 50);
	zephir_check_call_status();
	RETURN_MM();

}

