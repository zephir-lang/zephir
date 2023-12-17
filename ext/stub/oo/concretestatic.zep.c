
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
#include "kernel/object.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Oo_ConcreteStatic)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Oo, ConcreteStatic, stub, oo_concretestatic, stub_oo_abstractstatic_ce, stub_oo_concretestatic_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Oo_ConcreteStatic, parentFunction)
{

	RETURN_STRING("ConcreteStatic:parentFunction");
}

PHP_METHOD(Stub_Oo_ConcreteStatic, childFunction)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_SELF("parentfunction", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Oo_ConcreteStatic, callParentFunction)
{
	zend_long ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_RETURN_CALL_PARENT(stub_oo_concretestatic_ce, getThis(), "getcalledclass", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Oo_ConcreteStatic, simpleStaticIntReturn)
{

	RETURN_LONG(1337);
}

