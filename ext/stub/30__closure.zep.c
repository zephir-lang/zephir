
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


ZEPHIR_INIT_CLASS(stub_30__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 30__closure, stub, 30__closure, stub_30__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;
}

PHP_METHOD(stub_30__closure, __invoke)
{

	RETURN_LONG(42);
}

