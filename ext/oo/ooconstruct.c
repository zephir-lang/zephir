
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
 * Class with constructor
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoConstruct) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoConstruct, oo_ooconstruct, test_oo_ooconstruct_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Oo_OoConstruct, __construct) {



}

