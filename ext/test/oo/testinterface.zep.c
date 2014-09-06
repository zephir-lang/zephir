
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Oo_TestInterface) {

	ZEPHIR_REGISTER_INTERFACE(Test\\Oo, TestInterface, test, oo_testinterface, test_oo_testinterface_method_entry);

	return SUCCESS;

}

ZEPHIR_DOC_METHOD(Test_Oo_TestInterface, setParam);

