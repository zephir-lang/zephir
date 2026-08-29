
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


ZEPHIR_INIT_CLASS(stub_25__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 25__closure, stub, 25__closure, stub_25__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_25__closure_ce, SL("n"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_25__closure, __invoke)
{
	zval n;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&n);
	zephir_read_property(&n, this_ptr, SL("n"), PH_NOISY_CC | PH_READONLY);

	RETVAL_ZVAL(&n, 1, 0);
	return;
}

