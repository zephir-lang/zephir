
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


ZEPHIR_INIT_CLASS(stub_61__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 61__closure, stub, 61__closure, stub_61__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_61__closure_ce, SL("items"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

PHP_METHOD(stub_61__closure, __invoke)
{
	zval items;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&items);
	zephir_read_property(&items, this_ptr, SL("items"), PH_NOISY_CC | PH_READONLY);

	RETURN_LONG(zephir_fast_count_int(Z_REFVAL_P(&items)));
}

