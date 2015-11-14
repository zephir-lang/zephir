
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

	zval *result1 = NULL, *result2 = NULL, *result3 = NULL, *result4 = NULL, *_0, *_1, *_2, *_3, *_4, *_5, *_6, *_7;

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
	int num, total;

	zephir_fetch_params(0, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	total = 10;
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
	zval *step_param = NULL, *_0, *_1, *_2$$3, *_3$$3, *_4$$3, *_5$$3, _6$$3 = zval_used_for_init, *_7$$3 = NULL, *_9$$3, *_10$$3, *_11$$3 = NULL, *_12$$4, *_13$$4, *_14$$5, *_15$$5;
	int step, ZEPHIR_LAST_CALL_STATUS, filledWidth = 0, unfilledWidth = 0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &step_param);

	if (unlikely(Z_TYPE_P(step_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'step' must be a int") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	step = Z_LVAL_P(step_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	if (ZEPHIR_GT_LONG(_0, step)) {
		_2$$3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		_3$$3 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
		filledWidth = (long) ((zephir_safe_div_long_zval(((zephir_get_numberval(_2$$3) - 1)), _3$$3 TSRMLS_CC) * step));
		_4$$3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		unfilledWidth = (((zephir_get_numberval(_4$$3) - 1)) - filledWidth);
		_5$$3 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_6$$3);
		ZVAL_LONG(&_6$$3, filledWidth);
		ZEPHIR_CALL_FUNCTION(&_7$$3, "str_repeat", &_8, 17, _5$$3, &_6$$3);
		zephir_check_call_status();
		_9$$3 = zephir_fetch_nproperty_this(this_ptr, SL("arrow"), PH_NOISY_CC);
		_10$$3 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_6$$3);
		ZVAL_LONG(&_6$$3, unfilledWidth);
		ZEPHIR_CALL_FUNCTION(&_11$$3, "str_repeat", &_8, 17, _10$$3, &_6$$3);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VVV(return_value, _7$$3, _9$$3, _11$$3);
		RETURN_MM();
	} else if (ZEPHIR_IS_LONG_IDENTICAL(_1, step)) {
		_12$$4 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		_13$$4 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_8, 17, _12$$4, _13$$4);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		_14$$5 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		_15$$5 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_8, 17, _14$$5, _15$$5);
		zephir_check_call_status();
		RETURN_MM();
	}

}

PHP_METHOD(Test_Statements, test544IssueWithVariable) {

	zephir_fcall_cache_entry *_6 = NULL;
	zval *step_param = NULL, *_0, *_1$$3, *_2$$3, *_3$$3, _4$$3 = zval_used_for_init, *_5$$3 = NULL, *_7$$3, *_8$$3, *_9$$3 = NULL, *_10$$4, *_11$$4, *_12$$5, *_13$$5;
	int step, ZEPHIR_LAST_CALL_STATUS, filledWidth = 0, unfilledWidth = 0, totalSteps = 0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &step_param);

	if (unlikely(Z_TYPE_P(step_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'step' must be a int") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	step = Z_LVAL_P(step_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	totalSteps = zephir_get_numberval(_0);
	if (step < totalSteps) {
		_1$$3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		filledWidth = (long) ((zephir_safe_div_long_long(((zephir_get_numberval(_1$$3) - 1)), totalSteps TSRMLS_CC) * step));
		_2$$3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		unfilledWidth = (((zephir_get_numberval(_2$$3) - 1)) - filledWidth);
		_3$$3 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_4$$3);
		ZVAL_LONG(&_4$$3, filledWidth);
		ZEPHIR_CALL_FUNCTION(&_5$$3, "str_repeat", &_6, 17, _3$$3, &_4$$3);
		zephir_check_call_status();
		_7$$3 = zephir_fetch_nproperty_this(this_ptr, SL("arrow"), PH_NOISY_CC);
		_8$$3 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_4$$3);
		ZVAL_LONG(&_4$$3, unfilledWidth);
		ZEPHIR_CALL_FUNCTION(&_9$$3, "str_repeat", &_6, 17, _8$$3, &_4$$3);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VVV(return_value, _5$$3, _7$$3, _9$$3);
		RETURN_MM();
	} else if (step == totalSteps) {
		_10$$4 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		_11$$4 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_6, 17, _10$$4, _11$$4);
		zephir_check_call_status();
		RETURN_MM();
	} else {
		_12$$5 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		_13$$5 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_6, 17, _12$$5, _13$$5);
		zephir_check_call_status();
		RETURN_MM();
	}

}

