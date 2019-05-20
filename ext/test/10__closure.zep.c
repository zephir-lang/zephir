
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
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(test_10__closure) {

	ZEPHIR_REGISTER_CLASS(test, 10__closure, test, 10__closure, test_10__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;

}

PHP_METHOD(test_10__closure, __invoke) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG(5);

}

