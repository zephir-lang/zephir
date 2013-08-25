
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
ZEPHIR_INIT_CLASS(Test_Mcall) {

	ZEPHIR_REGISTER_CLASS(Test, Mcall, mcall, test_mcall_method_entry, 0);


	return SUCCESS;

}

