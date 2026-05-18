
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


ZEPHIR_INIT_CLASS(Stub_Issue2505Extended)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Issue2505Extended, stub, issue2505extended, stub_issue2505_ce, NULL, 0);

	return SUCCESS;
}

