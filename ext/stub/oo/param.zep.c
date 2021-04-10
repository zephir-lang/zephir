
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
ZEPHIR_INIT_CLASS(Stub_Oo_Param)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Oo, Param, stub, oo_param, NULL, 0);

	zend_declare_property_null(stub_oo_param_ce, SL("value"), ZEND_ACC_PUBLIC);
	return SUCCESS;
}

