
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_MethodInterface)
{
	ZEPHIR_REGISTER_INTERFACE(Stub, MethodInterface, stub, methodinterface, stub_methodinterface_method_entry);

	return SUCCESS;
}

ZEPHIR_DOC_METHOD(Stub_MethodInterface, testMethod);
