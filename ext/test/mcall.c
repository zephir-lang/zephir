
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


/**
 * Method calls
 */
ZEPHIR_INIT_CLASS(Test_Mcall) {

	ZEPHIR_REGISTER_CLASS(Test, Mcall, test, mcall, test_mcall_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Mcall, testMethod1) {


	RETURN_STRING("hello public", 1);

}

PHP_METHOD(Test_Mcall, testMethod2) {


	RETURN_STRING("hello protected", 1);

}

PHP_METHOD(Test_Mcall, testMethod3) {


	RETURN_STRING("hello private", 1);

}

PHP_METHOD(Test_Mcall, testMethod4) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Mcall, testMethod5) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Mcall, testMethod6) {

	zval *a, *b;

	zephir_fetch_params(0, 2, 0, &a, &b);



	zephir_add_function(return_value, a, b TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Mcall, testCall1) {

	ZEPHIR_MM_GROW();

	zephir_call_method(return_value, this_ptr, "testmethod1");
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall2) {

	ZEPHIR_MM_GROW();

	zephir_call_method(return_value, this_ptr, "testmethod2");
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall3) {

	ZEPHIR_MM_GROW();

	zephir_call_method(return_value, this_ptr, "testmethod3");
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall4) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_method_p2(return_value, this_ptr, "testmethod4", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall5) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_method_p2(return_value, this_ptr, "testmethod5", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall6) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_method_p2(return_value, this_ptr, "testmethod6", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall7) {

	zval *a, *b, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p2(_0, this_ptr, "testmethod4", a, b);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_method_p2(_1, this_ptr, "testmethod4", a, b);
	zephir_add_function(return_value, _0, _1 TSRMLS_CC);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall8) {

	zval *a, *b, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p2(_0, this_ptr, "testmethod5", a, b);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_method_p2(_1, this_ptr, "testmethod5", a, b);
	zephir_add_function(return_value, _0, _1 TSRMLS_CC);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall9) {

	zval *a, *b, *_0, *_1;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_INIT_VAR(_0);
	zephir_call_method_p2(_0, this_ptr, "testmethod6", a, b);
	ZEPHIR_INIT_VAR(_1);
	zephir_call_method_p2(_1, this_ptr, "testmethod5", a, b);
	zephir_add_function(return_value, _0, _1 TSRMLS_CC);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall10) {

	ZEPHIR_MM_GROW();

	zephir_call_method(return_value, this_ptr, "testmethod1");
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall11) {

	ZEPHIR_MM_GROW();

	zephir_call_method(return_value, this_ptr, "testmethod2");
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall12) {

	ZEPHIR_MM_GROW();

	zephir_call_method(return_value, this_ptr, "testmethod3");
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall13) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_method_p2(return_value, this_ptr, "testmethod4", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall14) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_method_p2(return_value, this_ptr, "testmethod5", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall15) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_method_p2(return_value, this_ptr, "testmethod6", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall16) {

	zval *c, *d;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &c, &d);



	zephir_call_method_p2(return_value, this_ptr, "testmethod4", c, d);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, testCall17) {

	zval *c, *d;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &c, &d);



	zephir_call_method_p2(return_value, this_ptr, "testmethod4", c, d);
	RETURN_MM();

}

PHP_METHOD(Test_Mcall, optionalRequereString) {

	zval *param_param = NULL;
	zval *param = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &param_param);

	if (Z_TYPE_P(param_param) != IS_STRING && Z_TYPE_P(param_param) != IS_NULL) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'param' must be a string") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	if (Z_TYPE_P(param_param) == IS_STRING) {
		param = param_param;
	} else {
		ZEPHIR_INIT_VAR(param);
		ZVAL_EMPTY_STRING(param);
	}


	RETURN_CTOR(param);

}

PHP_METHOD(Test_Mcall, optionalParameterString) {

	zval *param_param = NULL;
	zval *param = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &param_param);

	if (!param_param) {
		ZEPHIR_INIT_VAR(param);
		ZVAL_STRING(param, "test string", 1);
	} else {
		zephir_get_strval(param, param_param);
	}


	RETURN_CTOR(param);

}

PHP_METHOD(Test_Mcall, optionalParameterStringNull) {

	zval *param_param = NULL;
	zval *param = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &param_param);

	if (!param_param) {
		ZEPHIR_INIT_VAR(param);
		ZVAL_EMPTY_STRING(param);
	} else {
		zephir_get_strval(param, param_param);
	}


	RETURN_CTOR(param);

}

PHP_METHOD(Test_Mcall, optionalParameterInt) {

	zval *param_param = NULL;
	int param;

	zephir_fetch_params(0, 0, 1, &param_param);

	if (!param_param) {
		param = 2;
	} else {
		param = zephir_get_intval(param_param);
	}


	RETURN_LONG(param);

}

PHP_METHOD(Test_Mcall, optionalParameterVar) {

	zval *param = NULL;

	zephir_fetch_params(0, 0, 1, &param);

	if (!param) {
		param = ZEPHIR_GLOBAL(global_null);
	}


	RETURN_CCTORW(param);

}

PHP_METHOD(Test_Mcall, optionalParameterBoolTrue) {

	zval *param_param = NULL;
	zend_bool param;

	zephir_fetch_params(0, 0, 1, &param_param);

	if (!param_param) {
		param = 1;
	} else {
		param = zephir_get_boolval(param_param);
	}


	RETURN_BOOL(param);

}

PHP_METHOD(Test_Mcall, optionalParameterBoolFalse) {

	zval *param_param = NULL;
	zend_bool param;

	zephir_fetch_params(0, 0, 1, &param_param);

	if (!param_param) {
		param = 0;
	} else {
		param = zephir_get_boolval(param_param);
	}


	RETURN_BOOL(param);

}

PHP_METHOD(Test_Mcall, optionalParameterBoolean) {

	zval *start_param = NULL;
	zend_bool start;

	zephir_fetch_params(0, 0, 1, &start_param);

	if (!start_param) {
		start = 1;
	} else {
		if (Z_TYPE_P(start_param) != IS_BOOL) {
			zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'start' must be a bool") TSRMLS_CC);
			RETURN_NULL();
		}

		start = Z_BVAL_P(start_param);
	}


	RETURN_BOOL(start);

}

PHP_METHOD(Test_Mcall, optionalParameterBooleanNull) {

	zval *value_param = NULL;
	zend_bool value;

	zephir_fetch_params(0, 0, 1, &value_param);

	if (!value_param) {
		value = 0;
	} else {
		if (Z_TYPE_P(value_param) != IS_BOOL) {
			zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'value' must be a bool") TSRMLS_CC);
			RETURN_NULL();
		}

		value = Z_BVAL_P(value_param);
	}


	RETURN_BOOL(value);

}

/**
 * @link https://github.com/phalcon/zephir/issues/142
 */
PHP_METHOD(Test_Mcall, arrayParamWithDefaultEmptyArray) {

	zval *driverOptions_param = NULL;
	zval *driverOptions = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 0, 1, &driverOptions_param);

	if (!driverOptions_param) {
		ZEPHIR_INIT_VAR(driverOptions);
		array_init(driverOptions);
	} else {
		zephir_get_arrval(driverOptions, driverOptions_param);
	}


	RETURN_CTOR(driverOptions);

}

