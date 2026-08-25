
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
#include "kernel/memory.h"
#include "kernel/object.h"


/**
 * Calling an inherited interface method on a variable typed as the child
 * interface, rather than on `this`.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2635
 */
ZEPHIR_INIT_CLASS(Stub_Issue2635_Caller)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2635, Caller, stub, issue2635_caller, stub_issue2635_caller_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Issue2635_Caller, call)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *obj, obj_sub;

	ZVAL_UNDEF(&obj_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_OBJECT_OF_CLASS(obj, stub_issue2635_outer_ce)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &obj);
	ZEPHIR_RETURN_CALL_METHOD(obj, "go", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

