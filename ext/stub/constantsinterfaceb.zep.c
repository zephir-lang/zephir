
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


ZEPHIR_INIT_CLASS(Stub_ConstantsInterfaceB)
{
	ZEPHIR_REGISTER_CLASS(Stub, ConstantsInterfaceB, stub, constantsinterfaceb, stub_constantsinterfaceb_method_entry, 0);

	zend_class_implements(stub_constantsinterfaceb_ce, 1, stub_testinterface_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant1)
{

	RETURN_NULL();
}

PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant2)
{

	RETURN_BOOL(0);
}

PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant3)
{

	RETURN_BOOL(1);
}

PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant4)
{

	RETURN_LONG(10);
}

PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant5)
{

	RETURN_DOUBLE(10.25);
}

PHP_METHOD(Stub_ConstantsInterfaceB, testReadInterfaceConstant6)
{

	RETURN_STRING("test");
}

