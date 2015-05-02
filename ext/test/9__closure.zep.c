
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


ZEPHIR_INIT_CLASS(test_9__closure) {

	ZEPHIR_REGISTER_CLASS(test, 9__closure, test, 9__closure, test_9__closure_method_entry, ZEND_ACC_FINAL_CLASS);

	return SUCCESS;

}

PHP_METHOD(test_9__closure, __invoke) {


	RETURN_LONG(5);

}

