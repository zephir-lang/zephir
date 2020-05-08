
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_Oo_ConstantsInterface) {

	ZEPHIR_REGISTER_CLASS(Stub\\Oo, ConstantsInterface, stub, oo_constantsinterface, stub_oo_constantsinterface_method_entry, 0);

	zephir_declare_class_constant_string(stub_oo_constantsinterface_ce, SL("CLASSNAME"), "Stub\\Oo\\ConstantsInterface");

	zephir_declare_class_constant_string(stub_oo_constantsinterface_ce, SL("NAMESPACENAME"), "Stub\\Oo");

	zend_class_implements(stub_oo_constantsinterface_ce, 1, stub_testinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Stub_Oo_ConstantsInterface, testReadInterfaceConstant1) {

	zval *this_ptr = getThis();


	RETURN_NULL();

}

PHP_METHOD(Stub_Oo_ConstantsInterface, testReadInterfaceConstant2) {

	zval *this_ptr = getThis();


	RETURN_BOOL(0);

}

PHP_METHOD(Stub_Oo_ConstantsInterface, testReadInterfaceConstant3) {

	zval *this_ptr = getThis();


	RETURN_BOOL(1);

}

PHP_METHOD(Stub_Oo_ConstantsInterface, testReadInterfaceConstant4) {

	zval *this_ptr = getThis();


	RETURN_LONG(10);

}

PHP_METHOD(Stub_Oo_ConstantsInterface, testReadInterfaceConstant5) {

	zval *this_ptr = getThis();


	RETURN_DOUBLE(10.25);

}

PHP_METHOD(Stub_Oo_ConstantsInterface, testReadInterfaceConstant6) {

	zval *this_ptr = getThis();


	RETURN_STRING("test");

}

