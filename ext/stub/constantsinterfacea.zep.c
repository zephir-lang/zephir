
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


ZEPHIR_INIT_CLASS(Stub_ConstantsInterfaceA)
{
	ZEPHIR_REGISTER_CLASS(Stub, ConstantsInterfaceA, stub, constantsinterfacea, stub_constantsinterfacea_method_entry, 0);

	zend_class_implements(stub_constantsinterfacea_ce, 1, stub_testinterface_ce);
	return SUCCESS;
}

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant1)
{
	zval *this_ptr = getThis();



	RETURN_NULL();
}

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant2)
{
	zval *this_ptr = getThis();



	RETURN_BOOL(0);
}

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant3)
{
	zval *this_ptr = getThis();



	RETURN_BOOL(1);
}

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant4)
{
	zval *this_ptr = getThis();



	RETURN_LONG(10);
}

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant5)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(10.25);
}

PHP_METHOD(Stub_ConstantsInterfaceA, testReadInterfaceConstant6)
{
	zval *this_ptr = getThis();



	RETURN_STRING("test");
}

