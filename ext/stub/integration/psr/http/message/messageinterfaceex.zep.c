
#ifdef HAVE_CONFIG_H
#include "../../../../../ext_config.h"
#endif

#include <php.h>
#include "../../../../../php_ext.h"
#include "../../../../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Integration_Psr_Http_Message_MessageInterfaceEx)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Integration\\Psr\\Http\\Message, MessageInterfaceEx, stub, integration_psr_http_message_messageinterfaceex, NULL);

	zend_class_implements(stub_integration_psr_http_message_messageinterfaceex_ce, 1, zephir_get_internal_ce(SL("psr\\http\\message\\messageinterface")));
	return SUCCESS;
}

