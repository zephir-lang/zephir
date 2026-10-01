
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


ZEPHIR_INIT_CLASS(stub_50__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 50__closure, stub, 50__closure, stub_50__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_50__closure_ce, SL("seed"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_50__closure, __invoke)
{
	zval seed;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&seed);
	zephir_read_property(&seed, this_ptr, SL("seed"), PH_NOISY_CC | PH_READONLY);

	RETVAL_ZVAL(Z_REFVAL_P(&seed), 1, 0);
	return;
}

