
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"


ZEPHIR_INIT_CLASS(Test_Statements) {

	ZEPHIR_REGISTER_CLASS(Test, Statements, test, statements, test_statements_method_entry, 0);

	zend_declare_property_string(test_statements_ce, SL("tmp1"), "test", ZEND_ACC_PUBLIC TSRMLS_CC);

	zend_declare_property_string(test_statements_ce, SL("tmp2"), "test string", ZEND_ACC_PUBLIC TSRMLS_CC);

	/**
	 * Vars for 544 Issue test
	 */
	zend_declare_property_long(test_statements_ce, SL("totalSteps"), 100, ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_long(test_statements_ce, SL("width"), 100, ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_string(test_statements_ce, SL("filledChar"), "=", ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_string(test_statements_ce, SL("unfilledChar"), ".", ZEND_ACC_PRIVATE TSRMLS_CC);

	zend_declare_property_string(test_statements_ce, SL("arrow"), ">", ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Statements, testPropertyAcccessAvoidTmpReuse) {

	zval *result1, *result2, *result3, *result4, *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7;

	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result1);
	zephir_fast_strpos(result1, _0, _1, 0 );
	_2 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_3 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result2);
	zephir_fast_strpos(result2, _2, _3, 0 );
	_4 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_5 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result3);
	zephir_fast_strpos(result3, _4, _5, 0 );
	_6 = zephir_fetch_nproperty_this(this_ptr, SL("tmp2"), PH_NOISY_CC);
	_7 = zephir_fetch_nproperty_this(this_ptr, SL("tmp1"), PH_NOISY_CC);
	ZEPHIR_INIT_VAR(result4);
	zephir_fast_strpos(result4, _6, _7, 0 );
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Statements, testElseIf) {

	zval *num_param = NULL;
	int num;

	zephir_fetch_params(0, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	if (num > 0) {
		RETURN_STRING("more", 1);
	} else if (num == 0) {
		RETURN_STRING("equal", 1);
	} else if (num == -1) {
		RETURN_STRING("-1", 1);
	} else {
		RETURN_STRING("less", 1);
	}

}

PHP_METHOD(Test_Statements, testElseIf1) {

	zval *num_param = NULL;
	int num, total = 10;

	zephir_fetch_params(0, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	if (num < total) {
		RETURN_STRING("less", 1);
	} else if (num == total) {
		RETURN_STRING("equal", 1);
	} else {
		RETURN_STRING("else", 1);
	}

}

PHP_METHOD(Test_Statements, testElseIf2) {

	zval *num_param = NULL, *total;
	int num;

	zephir_fetch_params(0, 2, 0, &num_param, &total);

	num = zephir_get_intval(num_param);


	if (ZEPHIR_GT_LONG(total, num)) {
		RETURN_STRING("less", 1);
	} else if (ZEPHIR_IS_LONG(total, num)) {
		RETURN_STRING("equal", 1);
	} else {
		RETURN_STRING("else", 1);
	}

}

PHP_METHOD(Test_Statements, test544Issue) {

	zephir_fcall_cache_entry *_8 = NULL;
	zval *step_param = NULL, *_0, *_1, *_2, *_3, *_4, *_5, _6 = zval_used_for_init, *_7 = NULL, *_9, *_10, *_11 = NULL;
	int step, filledWidth, unfilledWidth, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &step_param);

	if (unlikely(Z_TYPE_P(step_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'step' must be a long/integer") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	step = Z_LVAL_P(step_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	if (ZEPHIR_GT_LONG(_0, step)) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
		filledWidth = (long) ((zephir_safe_div_long_zval(((zephir_get_numberval(_2) - 1)), _3 TSRMLS_CC) * step));
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		unfilledWidth = (((zephir_get_numberval(_4) - 1)) - filledWidth);
		_5 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_6);
		ZVAL_LONG(&_6, filledWidth);
		ZEPHIR_CALL_FUNCTION(&_7, "str_repeat", &_8, 21, _5, &_6);
		zephir_check_call_status();
		_9 = zephir_fetch_nproperty_this(this_ptr, SL("arrow"), PH_NOISY_CC);
		_10 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_6);
		ZVAL_LONG(&_6, unfilledWidth);
		ZEPHIR_CALL_FUNCTION(&_11, "str_repeat", &_8, 21, _10, &_6);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VVV(return_value, _7, _9, _11);
		RETURN_MM();
	} else if (ZEPHIR_IS_LONG_IDENTICAL(_1, step)) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_8, 21, _2, _3);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_8, 21, _2, _3);
		zephir_check_call_status();
		RETURN_MM();
	}

}

PHP_METHOD(Test_Statements, test544IssueWithVariable) {

	zephir_fcall_cache_entry *_6 = NULL;
	zval *step_param = NULL, *_0, *_1, *_2, *_3, _4 = zval_used_for_init, *_5 = NULL, *_7, *_8, *_9 = NULL;
	int step, filledWidth, unfilledWidth, totalSteps, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &step_param);

	if (unlikely(Z_TYPE_P(step_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'step' must be a long/integer") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	step = Z_LVAL_P(step_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	totalSteps = zephir_get_numberval(_0);
	if (step < totalSteps) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		filledWidth = (long) ((zephir_safe_div_long_long(((zephir_get_numberval(_1) - 1)), totalSteps TSRMLS_CC) * step));
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		unfilledWidth = (((zephir_get_numberval(_2) - 1)) - filledWidth);
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_4);
		ZVAL_LONG(&_4, filledWidth);
		ZEPHIR_CALL_FUNCTION(&_5, "str_repeat", &_6, 21, _3, &_4);
		zephir_check_call_status();
		_7 = zephir_fetch_nproperty_this(this_ptr, SL("arrow"), PH_NOISY_CC);
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_4);
		ZVAL_LONG(&_4, unfilledWidth);
		ZEPHIR_CALL_FUNCTION(&_9, "str_repeat", &_6, 21, _8, &_4);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VVV(return_value, _5, _7, _9);
		RETURN_MM();
	} else if (step == totalSteps) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_6, 21, _1, _2);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_6, 21, _1, _2);
		zephir_check_call_status();
		RETURN_MM();
	}

}

