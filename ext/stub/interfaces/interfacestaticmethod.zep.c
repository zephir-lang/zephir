
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Interfaces_InterfaceStaticMethod)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Interfaces, InterfaceStaticMethod, stub, interfaces_interfacestaticmethod, stub_interfaces_interfacestaticmethod_method_entry);

	return SUCCESS;
}

ZEPHIR_DOC_METHOD(Stub_Interfaces_InterfaceStaticMethod, reset);
