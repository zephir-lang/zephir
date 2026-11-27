
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


ZEPHIR_INIT_CLASS(stub_62__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 62__closure, stub, 62__closure, stub_62__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_62__closure_ce, SL("items"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_62__closure, __invoke)
{
	zval items;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&items);
	zephir_read_property(&items, this_ptr, SL("items"), PH_NOISY_CC | PH_READONLY);

	RETURN_LONG(zephir_fast_count_int(Z_REFVAL_P(&items)));
}

