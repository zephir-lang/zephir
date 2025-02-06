
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
#include "kernel/fcall.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Call external static functions
 */
ZEPHIR_INIT_CLASS(Stub_ScallExternal)
{
	ZEPHIR_REGISTER_CLASS(Stub, ScallExternal, stub, scallexternal, stub_scallexternal_method_entry, 0);

	zend_declare_property_null(stub_scallexternal_ce, SL("sproperty"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(Stub_ScallExternal, testCall1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_RETURN_CALL_CE_STATIC(stub_scall_ce, "testmethod1", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_ScallExternal, testCall2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *a, a_sub, *b, b_sub;

	ZVAL_UNDEF(&a_sub);
	ZVAL_UNDEF(&b_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(a)
		Z_PARAM_ZVAL(b)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a, &b);
	ZEPHIR_RETURN_CALL_CE_STATIC(stub_scall_ce, "testmethod4", NULL, 0, a, b);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_ScallExternal, testMethod3)
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

