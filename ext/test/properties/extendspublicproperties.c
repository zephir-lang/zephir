
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


ZEPHIR_INIT_CLASS(Test_Properties_ExtendsPublicProperties) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Properties, ExtendsPublicProperties, test, properties_extendspublicproperties, test_properties_publicproperties_ce, NULL, 0);

	zend_declare_property_long(test_properties_extendspublicproperties_ce, SL("testPropertyFromClassNameConstantValue"), 123, ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

