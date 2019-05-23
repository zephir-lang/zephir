
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


ZEPHIR_INIT_CLASS(test_8__closure) {

	ZEPHIR_REGISTER_CLASS(test, 8__closure, test, 8__closure, test_8__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(test_8__closure_ce, SL("abc"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(test_8__closure, __invoke) {

	zend_long abc;
	zval *this_ptr = getThis();


	zephir_read_static_property_ce(&abc, test_8__closure_ce, SL("abc"), PH_NOISY_CC);

	RETURN_LONG((abc + 1));

}

