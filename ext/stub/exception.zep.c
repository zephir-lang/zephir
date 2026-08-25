
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


/**
 * Sample exception class
 */
ZEPHIR_INIT_CLASS(Stub_Exception)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub, Exception, stub, exception, zend_ce_exception, stub_exception_method_entry, 0);

	return SUCCESS;
}

/**
 * @link https://github.com/zephir-lang/zephir/issues/915
 */
PHP_METHOD(Stub_Exception, testRuntimePropertyFetch)
{
	zval message_zv;
	zend_string *message = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&message_zv);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("message", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(message)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&message_zv, message);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 31, &message_zv);
}

