
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


ZEPHIR_INIT_CLASS(Test_Oo_ConstantsInterface) {

	ZEPHIR_REGISTER_CLASS(Test\\Oo, ConstantsInterface, test, oo_constantsinterface, test_oo_constantsinterface_method_entry, 0);

	zend_declare_class_constant_string(test_oo_constantsinterface_ce, SL("CLASSNAME"), "Test\\Oo\\ConstantsInterface" TSRMLS_CC);

	zend_declare_class_constant_string(test_oo_constantsinterface_ce, SL("NAMESPACENAME"), "Test\\Oo" TSRMLS_CC);

	zend_class_implements(test_oo_constantsinterface_ce TSRMLS_CC, 1, test_testinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Test_Oo_ConstantsInterface, testReadInterfaceConstant1) {


	RETURN_NULL();

}

PHP_METHOD(Test_Oo_ConstantsInterface, testReadInterfaceConstant2) {


	RETURN_BOOL(0);

}

PHP_METHOD(Test_Oo_ConstantsInterface, testReadInterfaceConstant3) {


	RETURN_BOOL(1);

}

PHP_METHOD(Test_Oo_ConstantsInterface, testReadInterfaceConstant4) {


	RETURN_LONG(10);

}

PHP_METHOD(Test_Oo_ConstantsInterface, testReadInterfaceConstant5) {


	RETURN_DOUBLE(10.25);

}

PHP_METHOD(Test_Oo_ConstantsInterface, testReadInterfaceConstant6) {


	RETURN_STRING("test", 1);

}

