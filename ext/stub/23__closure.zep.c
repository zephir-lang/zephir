
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


ZEPHIR_INIT_CLASS(stub_23__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 23__closure, stub, 23__closure, stub_23__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_23__closure_ce, SL("name"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(stub_23__closure, __invoke)
{
	zval name;

	ZVAL_UNDEF(&name);
	zephir_read_static_property_ce(&name, stub_23__closure_ce, SL("name"), PH_NOISY_CC);

	RETVAL_ZVAL(&name, 1, 0);
	return;
}

