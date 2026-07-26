
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
#include "kernel/concat.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(stub_18__closure)
{
	ZEPHIR_REGISTER_CLASS(stub, 18__closure, stub, 18__closure, stub_18__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(stub_18__closure_ce, SL("prefix"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC);
	return SUCCESS;
}

PHP_METHOD(stub_18__closure, __invoke)
{
	zval prefix, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&prefix);
	ZVAL_UNDEF(&_0);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("property1873", 12, 1);
	}
	zephir_read_static_property_ce(&prefix, stub_18__closure_ce, SL("prefix"), PH_NOISY_CC);

	zephir_read_property_cached(&_0, this_ptr, _zephir_prop_0, 135, PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CONCAT_VSV(return_value, &prefix, ":", &_0);
	return;
}

