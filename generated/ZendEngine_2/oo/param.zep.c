
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Oo_Param) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, Param, test, oo_param, NULL, 0);

	zend_declare_property_null(test_oo_param_ce, SL("value"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

