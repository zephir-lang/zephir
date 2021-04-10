
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_ScopeTesterInterface)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Oo\\Scopes, ScopeTesterInterface, stub, oo_scopes_scopetesterinterface, stub_oo_scopes_scopetesterinterface_method_entry);

	return SUCCESS;
}

ZEPHIR_DOC_METHOD(Stub_Oo_Scopes_ScopeTesterInterface, run);
