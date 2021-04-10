
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Interfaces_InterfaceInt)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Interfaces, InterfaceInt, stub, interfaces_interfaceint, stub_interfaces_interfaceint_method_entry);

	return SUCCESS;
}

ZEPHIR_DOC_METHOD(Stub_Interfaces_InterfaceInt, set);
ZEPHIR_DOC_METHOD(Stub_Interfaces_InterfaceInt, get);
