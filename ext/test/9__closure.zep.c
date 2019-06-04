
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
#include "kernel/array.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(test_9__closure) {

	ZEPHIR_REGISTER_CLASS(test, 9__closure, test, 9__closure, test_9__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	zend_declare_property_null(test_9__closure_ce, SL("abc"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(test_9__closure, __invoke) {

	zval abc, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&abc);
	ZVAL_UNDEF(&_0);

	zephir_read_static_property_ce(&abc, test_9__closure_ce, SL("abc"), PH_NOISY_CC);

	zephir_array_fetch_string(&_0, &abc, SL("a"), PH_NOISY | PH_READONLY, "test/closures.zep", 63 TSRMLS_CC);
	RETURN_CTORW(&_0);

}

