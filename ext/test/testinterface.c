
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "kernel/main.h"


/**
 * Sample exception class
 */
ZEPHIR_INIT_CLASS(Test_TestInterface) {

	ZEPHIR_REGISTER_INTERFACE(Test, TestInterface, test, testinterface, test_testinterface_method_entry);


	return SUCCESS;

}

/**
 * Sets a default action
 *
 * @param int defaultAccess
 */
ZEPHIR_DOC_METHOD(Test_TestInterface, setAction);
