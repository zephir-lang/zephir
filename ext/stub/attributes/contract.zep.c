
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_exceptions.h>

#include "kernel/main.h"


/**
 * An interface carries attributes on itself, on a constant and on a method
 * (including its parameters). Interface members reach the compiler through a
 * different grammar path than class members (#2466).
 */
ZEPHIR_INIT_CLASS(Stub_Attributes_Contract)
{
	ZEPHIR_REGISTER_INTERFACE(Stub\\Attributes, Contract, stub, attributes_contract, stub_attributes_contract_method_entry);

	{
		zend_attribute *_za = zephir_add_class_attribute(stub_attributes_contract_ce, SL("Stub\\Attributes\\Marker"), 1);
		zval _zc0;
		ZVAL_STRINGL(&_zc0, "on-an-interface", sizeof("on-an-interface") - 1);
		zephir_attribute_set_arg(_za, 0, NULL, 0, &_zc0);
	}
	zephir_add_method_attribute(stub_attributes_contract_ce, SL("handle"), SL("Stub\\Attributes\\Marker"), 0);
	zephir_add_parameter_attribute(stub_attributes_contract_ce, SL("handle"), 0, SL("Stub\\Attributes\\Marker"), 0);

	zephir_declare_class_constant_long(stub_attributes_contract_ce, SL("LIMIT"), 10);
	zephir_add_class_constant_attribute(stub_attributes_contract_ce, SL("LIMIT"), SL("Stub\\Attributes\\Marker"), 0);

	return SUCCESS;
}

ZEPHIR_DOC_METHOD(Stub_Attributes_Contract, handle);
