
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


ZEPHIR_INIT_CLASS(Test_Oo_ConcreteStatic) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Oo, ConcreteStatic, test, oo_concretestatic, test_oo_abstractstatic_ce, NULL, 0);

	return SUCCESS;

}

