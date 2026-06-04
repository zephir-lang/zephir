
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


ZEPHIR_INIT_CLASS(Stub_Properties_NestedPropertyTarget)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Properties, NestedPropertyTarget, stub, properties_nestedpropertytarget, NULL, 0);

	zend_declare_property_long(stub_properties_nestedpropertytarget_ce, SL("data"), 1, ZEND_ACC_PUBLIC);
	zend_declare_property_string(stub_properties_nestedpropertytarget_ce, SL("name"), "initial", ZEND_ACC_PUBLIC);
	return SUCCESS;
}

