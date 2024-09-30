
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
#include "kernel/operators.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_8__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 8__closure, stub, 8__closure, stub_8__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_8__closure_ce, SL("abc"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(stub_8__closure, __invoke)
{
	zval abc;

	ZVAL_UNDEF(&abc);
	zephir_read_static_property_ce(&abc, stub_8__closure_ce, SL("abc"), PH_NOISY_CC);

	RETURN_LONG((zephir_get_numberval(&abc) + 1));
}

