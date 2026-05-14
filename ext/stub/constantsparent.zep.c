
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


ZEPHIR_INIT_CLASS(Stub_ConstantsParent)
{
	ZEPHIR_REGISTER_CLASS(Stub, ConstantsParent, stub, constantsparent, NULL, 0);

	zephir_declare_class_constant_null(stub_constantsparent_ce, SL("P1"));

	zephir_declare_class_constant_bool(stub_constantsparent_ce, SL("P2"), 0);

	zephir_declare_class_constant_bool(stub_constantsparent_ce, SL("P3"), 1);

	zephir_declare_class_constant_long(stub_constantsparent_ce, SL("P4"), 10);

	zephir_declare_class_constant_double(stub_constantsparent_ce, SL("P5"), 10.25);

	zephir_declare_class_constant_string(stub_constantsparent_ce, SL("P6"), "test");

	return SUCCESS;
}

