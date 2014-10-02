
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
#include "kernel/object.h"
#include "kernel/string.h"
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

PHP_METHOD(Test_Statements, test544Issue) {

	zephir_nts_static zephir_fcall_cache_entry *_7 = NULL;
	zval *step_param = NULL, *_0, *_1, *_2, *_3, *_4, _5 = zval_used_for_init, *_6 = NULL, *_8, *_9, *_10 = NULL;
	int step, filledWidth, unfilledWidth, ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &step_param);

	if (unlikely(Z_TYPE_P(step_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'step' must be a long/integer") TSRMLS_CC);
		RETURN_MM_NULL();
	}

	step = Z_LVAL_P(step_param);


	_0 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	if (ZEPHIR_GT_LONG(_0, step)) {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
		filledWidth = ((((zephir_get_numberval(_1) - 1)) / zephir_get_numberval(_2)) * step);
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		unfilledWidth = (((zephir_get_numberval(_3) - 1)) - filledWidth);
		_4 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_VAR(_5);
		ZVAL_LONG(&_5, filledWidth);
		ZEPHIR_CALL_FUNCTION(&_6, "str_repeat", &_7, _4, &_5);
		zephir_check_call_status();
		_8 = zephir_fetch_nproperty_this(this_ptr, SL("arrow"), PH_NOISY_CC);
		_9 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		ZEPHIR_SINIT_NVAR(_5);
		ZVAL_LONG(&_5, unfilledWidth);
		ZEPHIR_CALL_FUNCTION(&_10, "str_repeat", &_7, _9, &_5);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VVV(return_value, _6, _8, _10);
		RETURN_MM();
	_1 = zephir_fetch_nproperty_this(this_ptr, SL("totalSteps"), PH_NOISY_CC);
	} else if (ZEPHIR_IS_LONG_IDENTICAL(_1, step)) {
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("filledChar"), PH_NOISY_CC);
		_3 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_CALL_FUNCTION(&_6, "str_repeat", &_7, _2, _3);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VS(return_value, _6, "\n");
		RETURN_MM();
	} else {
		_1 = zephir_fetch_nproperty_this(this_ptr, SL("unfilledChar"), PH_NOISY_CC);
		_2 = zephir_fetch_nproperty_this(this_ptr, SL("width"), PH_NOISY_CC);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_7, _1, _2);
		zephir_check_call_status();
		RETURN_MM();
	}

}

