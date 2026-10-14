
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


ZEPHIR_INIT_CLASS(Stub_Issue2391_Collaborator)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Issue2391, Collaborator, stub, issue2391_collaborator, NULL, 0);

	return SUCCESS;
}

