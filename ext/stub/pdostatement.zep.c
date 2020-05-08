
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


ZEPHIR_INIT_CLASS(Stub_PdoStatement) {

	ZEPHIR_REGISTER_CLASS_EX(Stub, PdoStatement, stub, pdostatement, zephir_get_internal_ce(SL("pdostatement")), NULL, 0);

	return SUCCESS;

}

