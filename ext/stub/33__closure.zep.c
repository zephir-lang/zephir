
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


ZEPHIR_INIT_CLASS(stub_33__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 33__closure, stub, 33__closure, stub_33__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_33__closure_ce, SL("value"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_33__closure, __invoke)
{
	zval value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);
	zephir_read_property(&value, this_ptr, SL("value"), PH_NOISY_CC | PH_READONLY);

	RETVAL_ZVAL(&value, 1, 0);
	return;
}

