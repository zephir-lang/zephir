
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
 * Class without constructor
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoNoConstruct) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, OoNoConstruct, test, oo_oonoconstruct, NULL, 0);

	return SUCCESS;

}

