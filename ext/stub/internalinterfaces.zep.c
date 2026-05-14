
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


ZEPHIR_INIT_CLASS(Stub_InternalInterfaces)
{
	ZEPHIR_REGISTER_CLASS(Stub, InternalInterfaces, stub, internalinterfaces, stub_internalinterfaces_method_entry, 0);

	zend_class_implements(stub_internalinterfaces_ce, 1, zend_ce_countable);
	return SUCCESS;
}

PHP_METHOD(Stub_InternalInterfaces, count)
{

	RETURN_LONG(0);
}

