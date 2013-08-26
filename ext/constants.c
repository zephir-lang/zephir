
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


ZEPHIR_INIT_CLASS(Test_Constants) {

	ZEPHIR_REGISTER_CLASS(Test, Constants, constants, test_constants_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Constants, testReadConstant) {

	RETURN_LONG(3);


}

