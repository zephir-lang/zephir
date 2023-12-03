
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_Invokes_InvokeProtected)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Invokes, InvokeProtected, stub, invokes_invokeprotected, stub_invokes_abstractinvoker_ce, stub_invokes_invokeprotected_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Invokes_InvokeProtected, __construct)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *text_param = NULL;
	zval text;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(text)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &text_param);
	zephir_get_strval(&text, text_param);


	zephir_update_property_zval(this_ptr, ZEND_STRL("text"), &text);
	ZEPHIR_MM_RESTORE();
}

