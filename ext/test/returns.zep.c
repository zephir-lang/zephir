
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


/**
 * X
 */
ZEPHIR_INIT_CLASS(Test_Returns) {

	ZEPHIR_REGISTER_CLASS(Test, Returns, test, returns, test_returns_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Returns, testReturnCast1) {


	RETURN_LONG((int) 5.0);

}

PHP_METHOD(Test_Returns, testReturnCast2) {


	RETURN_LONG(1);

}

PHP_METHOD(Test_Returns, testReturnCast3) {


	RETURN_LONG(0);

}

PHP_METHOD(Test_Returns, testReturnCast4) {


	if (1) {
		RETURN_LONG(1);
	} else {
		RETURN_LONG(0);
	}

}

