
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
#include "kernel/memory.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_26__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 26__closure, stub, 26__closure, stub_26__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;
}

PHP_METHOD(stub_26__closure, __invoke)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *carry, carry_sub, *row, row_sub;

	ZVAL_UNDEF(&carry_sub);
	ZVAL_UNDEF(&row_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(carry)
		Z_PARAM_ZVAL(row)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &carry, &row);
	ZEPHIR_RETURN_CALL_CE_STATIC(stub_issue2167_ce, "imploderow", NULL, 0, carry, row);
	zephir_check_call_status();
	RETURN_MM();
}

