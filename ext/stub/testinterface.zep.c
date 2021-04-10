
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


/**
 * Sample exception class
 */
ZEPHIR_INIT_CLASS(Stub_TestInterface)
{
	ZEPHIR_REGISTER_INTERFACE(Stub, TestInterface, stub, testinterface, NULL);

	zephir_declare_class_constant_null(stub_testinterface_ce, SL("C1"));

	zephir_declare_class_constant_bool(stub_testinterface_ce, SL("C2"), 0);

	zephir_declare_class_constant_bool(stub_testinterface_ce, SL("C3"), 1);

	zephir_declare_class_constant_long(stub_testinterface_ce, SL("C4"), 10);

	zephir_declare_class_constant_double(stub_testinterface_ce, SL("C5"), 10.25);

	zephir_declare_class_constant_string(stub_testinterface_ce, SL("C6"), "test");

	return SUCCESS;
}

