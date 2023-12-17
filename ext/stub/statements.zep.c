
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


ZEPHIR_INIT_CLASS(Stub_Statements)
{
	ZEPHIR_REGISTER_CLASS(Stub, Statements, stub, statements, stub_statements_method_entry, 0);

	zend_declare_property_string(stub_statements_ce, SL("tmp1"), "test", ZEND_ACC_PUBLIC);
	zend_declare_property_string(stub_statements_ce, SL("tmp2"), "test string", ZEND_ACC_PUBLIC);
	/**
	 * @issue https://github.com/zephir-lang/zephir/issues/544
	 */
	zend_declare_property_long(stub_statements_ce, SL("totalSteps"), 100, ZEND_ACC_PRIVATE);
	zend_declare_property_long(stub_statements_ce, SL("width"), 100, ZEND_ACC_PRIVATE);
	zend_declare_property_string(stub_statements_ce, SL("filledChar"), "=", ZEND_ACC_PRIVATE);
	zend_declare_property_string(stub_statements_ce, SL("unfilledChar"), ".", ZEND_ACC_PRIVATE);
	zend_declare_property_string(stub_statements_ce, SL("arrow"), ">", ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_Statements, testPropertyAcccessAvoidTmpReuse)
{
	zval result1, result2, result3, result4, _0, _1, _2, _3, _4, _5, _6, _7;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&result1);
	ZVAL_UNDEF(&result2);
	ZVAL_UNDEF(&result3);
	ZVAL_UNDEF(&result4);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&_7);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("tmp2"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("tmp1"), PH_NOISY_CC | PH_READONLY);
	ZVAL_NULL(&result1);
	zephir_fast_strpos(&result1, &_0, &_1, 0 );
	zephir_read_property(&_2, this_ptr, ZEND_STRL("tmp2"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_3, this_ptr, ZEND_STRL("tmp1"), PH_NOISY_CC | PH_READONLY);
	ZVAL_NULL(&result2);
	zephir_fast_strpos(&result2, &_2, &_3, 0 );
	zephir_read_property(&_4, this_ptr, ZEND_STRL("tmp2"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_5, this_ptr, ZEND_STRL("tmp1"), PH_NOISY_CC | PH_READONLY);
	ZVAL_NULL(&result3);
	zephir_fast_strpos(&result3, &_4, &_5, 0 );
	zephir_read_property(&_6, this_ptr, ZEND_STRL("tmp2"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_7, this_ptr, ZEND_STRL("tmp1"), PH_NOISY_CC | PH_READONLY);
	ZVAL_NULL(&result4);
	zephir_fast_strpos(&result4, &_6, &_7, 0 );
}

PHP_METHOD(Stub_Statements, testElseIf)
{
	zval *num_param = NULL;
	zend_long num;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	if (num > 0) {
		RETURN_STRING("more");
	} else if (num == 0) {
		RETURN_STRING("equal");
	} else if (num == -1) {
		RETURN_STRING("-1");
	} else {
		RETURN_STRING("less");
	}
}

PHP_METHOD(Stub_Statements, testElseIf1)
{
	zval *num_param = NULL;
	zend_long num, total;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	total = 10;
	if (num < total) {
		RETURN_STRING("less");
	} else if (num == total) {
		RETURN_STRING("equal");
	} else {
		RETURN_STRING("else");
	}
}

PHP_METHOD(Stub_Statements, testElseIf2)
{
	zval *num_param = NULL, *total, total_sub;
	zend_long num;

	ZVAL_UNDEF(&total_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_LONG(num)
		Z_PARAM_ZVAL(total)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &num_param, &total);
	if (ZEPHIR_GT_LONG(total, num)) {
		RETURN_STRING("less");
	} else if (ZEPHIR_IS_LONG(total, num)) {
		RETURN_STRING("equal");
	} else {
		RETURN_STRING("else");
	}
}

PHP_METHOD(Stub_Statements, test544Issue)
{
	zephir_fcall_cache_entry *_8 = NULL;
	zval *step_param = NULL, _0, _1, _2$$3, _3$$3, _4$$3, _5$$3, _6$$3, _7$$3, _9$$3, _10$$3, _11$$3, _12$$4, _13$$4, _14$$5, _15$$5;
	zend_long step, ZEPHIR_LAST_CALL_STATUS, filledWidth = 0, unfilledWidth = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_9$$3);
	ZVAL_UNDEF(&_10$$3);
	ZVAL_UNDEF(&_11$$3);
	ZVAL_UNDEF(&_12$$4);
	ZVAL_UNDEF(&_13$$4);
	ZVAL_UNDEF(&_14$$5);
	ZVAL_UNDEF(&_15$$5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(step)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &step_param);
	if (UNEXPECTED(Z_TYPE_P(step_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'step' must be of the type int"));
		RETURN_MM_NULL();
	}
	step = Z_LVAL_P(step_param);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("totalSteps"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("totalSteps"), PH_NOISY_CC | PH_READONLY);
	if (ZEPHIR_GT_LONG(&_0, step)) {
		zephir_read_property(&_2$$3, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		zephir_read_property(&_3$$3, this_ptr, ZEND_STRL("totalSteps"), PH_NOISY_CC | PH_READONLY);
		filledWidth = (long) ((zephir_safe_div_long_zval(((zephir_get_numberval(&_2$$3) - 1)), &_3$$3) * step));
		zephir_read_property(&_4$$3, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		unfilledWidth = (((zephir_get_numberval(&_4$$3) - 1)) - filledWidth);
		zephir_read_property(&_5$$3, this_ptr, ZEND_STRL("filledChar"), PH_NOISY_CC | PH_READONLY);
		ZVAL_LONG(&_6$$3, filledWidth);
		ZEPHIR_CALL_FUNCTION(&_7$$3, "str_repeat", &_8, 23, &_5$$3, &_6$$3);
		zephir_check_call_status();
		zephir_read_property(&_6$$3, this_ptr, ZEND_STRL("arrow"), PH_NOISY_CC | PH_READONLY);
		zephir_read_property(&_9$$3, this_ptr, ZEND_STRL("unfilledChar"), PH_NOISY_CC | PH_READONLY);
		ZVAL_LONG(&_10$$3, unfilledWidth);
		ZEPHIR_CALL_FUNCTION(&_11$$3, "str_repeat", &_8, 23, &_9$$3, &_10$$3);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VVV(return_value, &_7$$3, &_6$$3, &_11$$3);
		return;
	} else if (ZEPHIR_IS_LONG_IDENTICAL(&_1, step)) {
		zephir_read_property(&_12$$4, this_ptr, ZEND_STRL("filledChar"), PH_NOISY_CC | PH_READONLY);
		zephir_read_property(&_13$$4, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_8, 23, &_12$$4, &_13$$4);
		zephir_check_call_status();
		return;
	} else {
		zephir_read_property(&_14$$5, this_ptr, ZEND_STRL("unfilledChar"), PH_NOISY_CC | PH_READONLY);
		zephir_read_property(&_15$$5, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_8, 23, &_14$$5, &_15$$5);
		zephir_check_call_status();
		return;
	}
}

PHP_METHOD(Stub_Statements, test544IssueWithVariable)
{
	zephir_fcall_cache_entry *_6 = NULL;
	zval *step_param = NULL, _0, _1$$3, _2$$3, _3$$3, _4$$3, _5$$3, _7$$3, _8$$3, _9$$3, _10$$4, _11$$4, _12$$5, _13$$5;
	zend_long step, ZEPHIR_LAST_CALL_STATUS, filledWidth = 0, unfilledWidth = 0, totalSteps = 0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$3);
	ZVAL_UNDEF(&_9$$3);
	ZVAL_UNDEF(&_10$$4);
	ZVAL_UNDEF(&_11$$4);
	ZVAL_UNDEF(&_12$$5);
	ZVAL_UNDEF(&_13$$5);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(step)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &step_param);
	if (UNEXPECTED(Z_TYPE_P(step_param) != IS_LONG)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'step' must be of the type int"));
		RETURN_MM_NULL();
	}
	step = Z_LVAL_P(step_param);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("totalSteps"), PH_NOISY_CC | PH_READONLY);
	totalSteps = zephir_get_numberval(&_0);
	if (step < totalSteps) {
		zephir_read_property(&_1$$3, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		filledWidth = (long) ((zephir_safe_div_long_long(((zephir_get_numberval(&_1$$3) - 1)), totalSteps) * step));
		zephir_read_property(&_2$$3, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		unfilledWidth = (((zephir_get_numberval(&_2$$3) - 1)) - filledWidth);
		zephir_read_property(&_3$$3, this_ptr, ZEND_STRL("filledChar"), PH_NOISY_CC | PH_READONLY);
		ZVAL_LONG(&_4$$3, filledWidth);
		ZEPHIR_CALL_FUNCTION(&_5$$3, "str_repeat", &_6, 23, &_3$$3, &_4$$3);
		zephir_check_call_status();
		zephir_read_property(&_4$$3, this_ptr, ZEND_STRL("arrow"), PH_NOISY_CC | PH_READONLY);
		zephir_read_property(&_7$$3, this_ptr, ZEND_STRL("unfilledChar"), PH_NOISY_CC | PH_READONLY);
		ZVAL_LONG(&_8$$3, unfilledWidth);
		ZEPHIR_CALL_FUNCTION(&_9$$3, "str_repeat", &_6, 23, &_7$$3, &_8$$3);
		zephir_check_call_status();
		ZEPHIR_CONCAT_VVV(return_value, &_5$$3, &_4$$3, &_9$$3);
		return;
	} else if (step == totalSteps) {
		zephir_read_property(&_10$$4, this_ptr, ZEND_STRL("filledChar"), PH_NOISY_CC | PH_READONLY);
		zephir_read_property(&_11$$4, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_6, 23, &_10$$4, &_11$$4);
		zephir_check_call_status();
		return;
	} else {
		zephir_read_property(&_12$$5, this_ptr, ZEND_STRL("unfilledChar"), PH_NOISY_CC | PH_READONLY);
		zephir_read_property(&_13$$5, this_ptr, ZEND_STRL("width"), PH_NOISY_CC | PH_READONLY);
		ZEPHIR_RETURN_CALL_FUNCTION("str_repeat", &_6, 23, &_12$$5, &_13$$5);
		zephir_check_call_status();
		return;
	}
}

