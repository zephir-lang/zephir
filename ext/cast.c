
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"


/**
 * X
 */
ZEPHIR_INIT_CLASS(Test_Cast) {

	ZEPHIR_REGISTER_CLASS(Test, Cast, cast, test_cast_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Cast, testCast1) {


	RETURN_LONG((int) 5.0);

}

PHP_METHOD(Test_Cast, testCast2) {


	RETURN_LONG(1);

}

