
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * A top-level function carrying attributes on itself and on a parameter.
 *
 * A function has no class initializer to write into, so these are attached from
 * MINIT next to the class inits; the engine registers a module's functions
 * before MINIT runs, so `CG(function_table)` already holds them (#2466).
 */
ZEPHIR_INIT_CLASS(Stub_Attributes_Tools)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Attributes, Tools, stub, attributes_tools, stub_attributes_tools_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Attributes_Tools, callTagged)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, _0;
	zend_long a, ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	ZVAL_LONG(&_0, a);
	ZEPHIR_RETURN_CALL_FUNCTION("tagged", NULL, 8, &_0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_FUNCTION(f_Stub_Attributes_tagged) {
	zval *a_param = NULL;
	zend_long a;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(a)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &a_param);
	RETURN_LONG(a);
}

