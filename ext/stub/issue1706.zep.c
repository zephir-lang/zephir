
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
#include "kernel/exception.h"


/**
 * Issue #1706: a switch that has a `default` clause and returns on every
 * path is exhaustive and must satisfy the return-type hint.
 */
ZEPHIR_INIT_CLASS(Stub_Issue1706)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1706, stub, issue1706, stub_issue1706_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue1706, testSwitch)
{
	zend_long a = 0;
	a = 1;
	if (a == 1) { goto zephir_switch_0_clause_0; }
	if (a == 2) { goto zephir_switch_0_clause_2; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
	zephir_switch_0_clause_1: ;
		RETURN_LONG(1);
	zephir_switch_0_clause_2: ;
		RETURN_LONG(2);

}

PHP_METHOD(Stub_Issue1706, switchDefaultLast)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	if (a == 1) { goto zephir_switch_0_clause_0; }
	if (a == 2) { goto zephir_switch_0_clause_1; }
	goto zephir_switch_0_clause_2;
	zephir_switch_0_clause_0: ;
		RETURN_LONG(10);
	zephir_switch_0_clause_1: ;
		RETURN_LONG(20);
	zephir_switch_0_clause_2: ;
		RETURN_LONG(0);

}

PHP_METHOD(Stub_Issue1706, switchWithThrow)
{
	zval *a_param = NULL;
	zend_long a;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	if (a == 1) { goto zephir_switch_0_clause_0; }
	goto zephir_switch_0_clause_1;
	zephir_switch_0_clause_0: ;
		RETURN_STRING("one");
	zephir_switch_0_clause_1: ;
		ZEPHIR_THROW_EXCEPTION_DEBUG_STRW(zend_ce_exception, "bad", "stub/issue1706.zep", 42);
		return;

}

