
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


ZEPHIR_INIT_CLASS(Stub_Issue2542)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue2542, stub, issue2542, NULL, 0);

	zephir_declare_class_constant_long(stub_issue2542_ce, SL("VERSION_ID"), 80231);

	zephir_declare_class_constant_long(stub_issue2542_ce, SL("INT_SIZE"), 8);

	zephir_declare_class_constant_string(stub_issue2542_ce, SL("OS"), "Linux");

	return SUCCESS;
}

