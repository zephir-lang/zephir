
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


ZEPHIR_INIT_CLASS(stub_60__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 60__closure, stub, 60__closure, stub_60__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_60__closure_ce, SL("s"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_60__closure, __invoke)
{
	zval s;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&s);
	zephir_read_property(&s, this_ptr, SL("s"), PH_NOISY_CC | PH_READONLY);

	RETVAL_ZVAL(Z_REFVAL_P(&s), 1, 0);
	return;
}

