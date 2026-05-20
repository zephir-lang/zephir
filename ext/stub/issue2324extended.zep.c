
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


ZEPHIR_INIT_CLASS(Stub_Issue2324Extended)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2324Extended, stub, issue2324extended, stub_issue2324_ce, NULL, 0);

	return SUCCESS;
}

