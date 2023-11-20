
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


ZEPHIR_INIT_CLASS(Stub_Mcall_Caller)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Mcall, Caller, stub, mcall_caller, stub_mcall_caller_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Mcall_Caller, start)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *f, f_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&f_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(f)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &f);


	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(f, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Mcall_Caller, perform)
{
	zval *this_ptr = getThis();



	RETURN_STRING("Caller:perform");
}

