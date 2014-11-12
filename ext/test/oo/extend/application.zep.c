
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_Oo_Extend_Application) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Oo\\Extend, Application, test, oo_extend_application, zephir_get_internal_ce(SS("phalcon\\mvc\\application") TSRMLS_CC), NULL, 0);

	return SUCCESS;

}

