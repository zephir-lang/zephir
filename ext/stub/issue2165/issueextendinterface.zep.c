
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Stub_Issue2165_IssueExtendInterface)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Issue2165, IssueExtendInterface, stub, issue2165_issueextendinterface, NULL);

	return SUCCESS;
}

