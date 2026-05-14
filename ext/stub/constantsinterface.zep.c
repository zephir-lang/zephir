
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


ZEPHIR_INIT_CLASS(Stub_ConstantsInterface)
{
	ZEPHIR_REGISTER_CLASS(Stub, ConstantsInterface, stub, constantsinterface, stub_constantsinterface_method_entry, 0);

	zend_class_implements(stub_constantsinterface_ce, 1, stub_testinterface_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant1)
{

	RETURN_NULL();
}

PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant2)
{

	RETURN_BOOL(0);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant3)
{

	RETURN_BOOL(1);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant4)
{

	RETURN_LONG(10);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant5)
{

	RETURN_DOUBLE(10.25);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInterfaceConstant6)
{

	RETURN_STRING("test");
}

PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant1)
{

	RETURN_NULL();
}

PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant2)
{

	RETURN_BOOL(0);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant3)
{

	RETURN_BOOL(1);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant4)
{

	RETURN_LONG(10);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant5)
{

	RETURN_DOUBLE(10.25);
}

PHP_METHOD(Stub_ConstantsInterface, testReadInheritanceFromInterfaceConstant6)
{

	RETURN_STRING("test");
}

