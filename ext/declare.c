
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
 * Function calls
 */
ZEPHIR_INIT_CLASS(Test_Declare) {

	ZEPHIR_REGISTER_CLASS(Test, Declare, declare, test_declare_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Declare, testDeclare1) {

	int a = 0;

	RETURN_LONG(a);

}

