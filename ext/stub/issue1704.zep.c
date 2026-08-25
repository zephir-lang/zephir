
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
#include "kernel/exception.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * Issue #1704: a `break` written after a `return` inside a `case` is dead code
 * that PHP accepts without emitting any diagnostic. It must not raise an
 * `Unreachable code` warning, and it must not stop the `switch` from
 * satisfying the method's return-type hint.
 */
ZEPHIR_INIT_CLASS(Stub_Issue1704)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1704, stub, issue1704, stub_issue1704_method_entry, 0);

	return SUCCESS;
}

/**
 * The snippet reported in the issue.
 */
PHP_METHOD(Stub_Issue1704, runmode)
{
	zval *value_param = NULL;
	zend_long value;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value_param);
	if (value == 123) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		RETURN_STRING("abc");
		goto zephir_switch_0_end;
	zephir_switch_0_clause_1: ;
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_ce_exception, "Invalid application runmode.", "stub/issue1704.zep", 22);
		return;
	zephir_switch_0_end: ;

}

/**
 * Every clause returns and carries the same dead `break` padding.
 */
PHP_METHOD(Stub_Issue1704, padded)
{
	zval *value_param = NULL;
	zend_long value;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value_param);
	if (value == 1) { goto zephir_switch_0_clause_0; }
	if (value == 2) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_clause_2;
	zephir_switch_0_clause_0: ;
		RETURN_STRING("one");
		goto zephir_switch_0_end;
	zephir_switch_0_clause_1: ;
		RETURN_STRING("two");
		goto zephir_switch_0_end;
	zephir_switch_0_clause_2: ;
		RETURN_STRING("many");
		goto zephir_switch_0_end;
	zephir_switch_0_end: ;

}

/**
 * A dead `break` inside a loop is equally silent in PHP.
 */
PHP_METHOD(Stub_Issue1704, breakAfterReturnInLoop)
{
	zval *limit_param = NULL;
	zend_long limit, i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(limit)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &limit_param);
	i = 0;
	while (1) {
		if (!(i < limit)) {
			break;
		}
		RETURN_LONG(i);
		break;
	}
	RETURN_LONG(-1);
}

/**
 * As is a dead `continue`.
 */
PHP_METHOD(Stub_Issue1704, continueAfterReturnInLoop)
{
	zval *limit_param = NULL;
	zend_long limit, i;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(limit)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &limit_param);
	i = 0;
	while (1) {
		if (!(i < limit)) {
			break;
		}
		RETURN_LONG(i);
		continue;
	}
	RETURN_LONG(-1);
}

