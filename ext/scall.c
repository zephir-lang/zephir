
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
ZEPHIR_INIT_CLASS(Test_Scall) {

	ZEPHIR_REGISTER_CLASS(Test, Scall, scall, test_scall_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Scall, testMethod1) {

	RETURN_STRING("hello", 1);


}

PHP_METHOD(Test_Scall, testCall1) {

	RETURN_NULL();


}

