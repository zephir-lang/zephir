
#ifdef HAVE_CONFIG_H
#include "../ext_config.h"
#endif

#include <php.h>
#include "../php_ext.h"
#include "../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/main.h"


ZEPHIR_INIT_CLASS(Test_PdoStatement) {

	ZEPHIR_REGISTER_CLASS_EX(Test, PdoStatement, test, pdostatement, zephir_get_internal_ce(SS("pdostatement") TSRMLS_CC), NULL, 0);

	return SUCCESS;

}

