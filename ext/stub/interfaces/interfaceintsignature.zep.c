
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Interfaces_InterfaceIntSignature)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Interfaces, InterfaceIntSignature, stub, interfaces_interfaceintsignature, stub_interfaces_interfaceintsignature_method_entry);

	return SUCCESS;
}

ZEPHIR_DOC_METHOD(Stub_Interfaces_InterfaceIntSignature, get);
