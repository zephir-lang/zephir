
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/operators.h"


/**
 * Regression coverage for https://github.com/zephir-lang/zephir/issues/1097
 *
 * elseif conditions that contain sub-expressions with side effects (such as
 * array element access myvar[0]) were compiled by hoisting the side-effect code
 * before the entire if/elseif chain. This caused a PHP Notice when a scalar was
 * passed, because zephir_array_fetch_long() ran unconditionally even when the
 * first if-branch had already returned.
 *
 * Fix: preamble code emitted during elseif-condition compilation must be placed
 * inside the else branch, not before the outer if, so it only executes when
 * needed.
 */
ZEPHIR_INIT_CLASS(Stub_Issue1097)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1097, stub, issue1097, stub_issue1097_method_entry, 0);

	return SUCCESS;
}

/**
 * The canonical case from the bug report: if the first branch is taken
 * (is_string(myvar) is true), the elseif must NOT attempt myvar[0].
 */
PHP_METHOD(Stub_Issue1097, testStringOrArrayOfString)
{
	zval *myvar, myvar_sub, _0;

	ZVAL_UNDEF(&myvar_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(myvar)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &myvar);
	if (Z_TYPE_P(myvar) == IS_STRING) {
		RETURN_STRING("string");
	} else {
		zephir_array_fetch_long(&_0, myvar, 0, PH_NOISY | PH_READONLY, "stub/issue1097.zep", 26);
		if (Z_TYPE_P(&_0) == IS_STRING) {
			RETURN_STRING("array-of-string");
		}
	}
	RETURN_STRING("other");
}

/**
 * Variant with an else branch to ensure the else path is unaffected.
 */
PHP_METHOD(Stub_Issue1097, testWithElse)
{
	zval *myvar, myvar_sub;

	ZVAL_UNDEF(&myvar_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(myvar)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &myvar);
	if (Z_TYPE_P(myvar) == IS_STRING) {
		RETURN_STRING("string");
	} else if (Z_TYPE_P(myvar) == IS_ARRAY) {
		RETURN_STRING("array");
	} else {
		RETURN_STRING("other");
	}
}

/**
 * Two elseif branches, each with a sub-expression that generates preamble
 * code. Only the first matching branch must evaluate.
 */
PHP_METHOD(Stub_Issue1097, testMultipleElseif)
{
	zval *myvar, myvar_sub, _0, _1;

	ZVAL_UNDEF(&myvar_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(myvar)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &myvar);
	if (Z_TYPE_P(myvar) == IS_STRING) {
		RETURN_STRING("string");
	} else {
		zephir_array_fetch_long(&_0, myvar, 0, PH_NOISY | PH_READONLY, "stub/issue1097.zep", 55);
		if (Z_TYPE_P(&_0) == IS_STRING) {
			RETURN_STRING("array-of-string");
		} else {
			zephir_array_fetch_long(&_1, myvar, 0, PH_NOISY | PH_READONLY, "stub/issue1097.zep", 57);
			if (Z_TYPE_P(&_1) == IS_LONG) {
				RETURN_STRING("array-of-int");
			}
		}
	}
	RETURN_STRING("other");
}

/**
 * elseif with a simple condition (no preamble) must continue to work.
 */
PHP_METHOD(Stub_Issue1097, testSimpleElseif)
{
	zval *num_param = NULL;
	zend_long num;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(num)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &num_param);
	if (num > 0) {
		RETURN_STRING("positive");
	} else if (num == 0) {
		RETURN_STRING("zero");
	} else {
		RETURN_STRING("negative");
	}
}

