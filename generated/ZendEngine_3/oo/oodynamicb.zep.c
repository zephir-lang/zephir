
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
 * Class with dynamic new
 */
ZEPHIR_INIT_CLASS(Test_Oo_OoDynamicB) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Oo, OoDynamicB, test, oo_oodynamicb, test_oo_oodynamica_ce, NULL, 0);

	return SUCCESS;

}

