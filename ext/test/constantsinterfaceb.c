
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


ZEPHIR_INIT_CLASS(Test_ConstantsInterfaceB) {

	ZEPHIR_REGISTER_CLASS(Test, ConstantsInterfaceB, test, constantsinterfaceb, test_constantsinterfaceb_method_entry, 0);

	zend_class_implements(test_constantsinterfaceb_ce TSRMLS_CC, 1, test_testinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant1) {


	RETURN_NULL();

}

PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant2) {


	RETURN_BOOL(0);

}

PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant3) {


	RETURN_BOOL(1);

}

PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant4) {


	RETURN_LONG(10);

}

PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant5) {


	RETURN_DOUBLE(10.25);

}

PHP_METHOD(Test_ConstantsInterfaceB, testReadInterfaceConstant6) {


	RETURN_STRING("test", 1);

}

