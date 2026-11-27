
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


ZEPHIR_INIT_CLASS(Stub_Properties_ExtendsPublicProperties)
{
	ZEPHIR_REGISTER_CLASS_EX(Stub\\Properties, ExtendsPublicProperties, stub, properties_extendspublicproperties, stub_properties_publicproperties_ce, NULL, 0);

	zend_declare_property_long(stub_properties_extendspublicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC);
	{
		zval _zc0;
		array_init_size(&_zc0, 2);
		add_next_index_stringl(&_zc0, SL("holy"));
		zephir_declare_property_array(stub_properties_extendspublicproperties_ce, SL("someGetterSetterArray"), &_zc0, ZEND_ACC_PROTECTED);
	}

	return SUCCESS;
}

