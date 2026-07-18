
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_ConstantsParent)
{
	ZEPHIR_REGISTER_CLASS(Stub, ConstantsParent, stub, constantsparent, stub_constantsparent_method_entry, 0);

	zephir_declare_class_constant_null(stub_constantsparent_ce, SL("P1"));

	zephir_declare_class_constant_bool(stub_constantsparent_ce, SL("P2"), 0);

	zephir_declare_class_constant_bool(stub_constantsparent_ce, SL("P3"), 1);

	zephir_declare_class_constant_long(stub_constantsparent_ce, SL("P4"), 10);

	zephir_declare_class_constant_double(stub_constantsparent_ce, SL("P5"), 10.25);

	zephir_declare_class_constant_string(stub_constantsparent_ce, SL("P6"), "test");

	return SUCCESS;
}

PHP_METHOD(Stub_ConstantsParent, testStaticClassInherited)
{

	zephir_get_called_class(return_value);
	return;
}

PHP_METHOD(Stub_ConstantsParent, testSelfClassInherited)
{

	RETURN_STRING("Stub\\ConstantsParent");
}

