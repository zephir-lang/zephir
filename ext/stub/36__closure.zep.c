
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


ZEPHIR_INIT_CLASS(stub_36__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 36__closure, stub, 36__closure, stub_36__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_36__closure_ce, SL("counter"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_36__closure, __invoke)
{
	zval counter;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&counter);
	zephir_read_property(&counter, this_ptr, SL("counter"), PH_NOISY_CC | PH_READONLY);

	RETVAL_ZVAL(Z_REFVAL_P(&counter), 1, 0);
	return;
}

