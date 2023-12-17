
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


ZEPHIR_INIT_CLASS(stub_13__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 13__closure, stub, 13__closure, stub_13__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;
}

PHP_METHOD(stub_13__closure, __invoke)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_CE_STATIC(stub_mcall_caller_ce, "perform", NULL, 0);
	zephir_check_call_status();
	return;
}

