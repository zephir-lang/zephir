
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Integration_Psr_ExtendExternal)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Integration\\Psr, ExtendExternal, stub, integration_psr_extendexternal, zephir_get_internal_ce(SL("psr\\log\\abstractlogger")), NULL, 0);

	return SUCCESS;
}

