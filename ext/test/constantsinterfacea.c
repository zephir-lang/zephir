
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


ZEPHIR_INIT_CLASS(Test_ConstantsInterfaceA) {

	ZEPHIR_REGISTER_CLASS(Test, ConstantsInterfaceA, test, constantsinterfacea, test_constantsinterfacea_method_entry, 0);

	zend_class_implements(test_constantsinterfacea_ce TSRMLS_CC, 1, test_testinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant1) {


	RETURN_NULL();

}

PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant2) {


	RETURN_BOOL(0);

}

PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant3) {


	RETURN_BOOL(1);

}

PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant4) {


	RETURN_LONG(10);

}

PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant5) {


	RETURN_DOUBLE(10.25);

}

PHP_METHOD(Test_ConstantsInterfaceA, testReadInterfaceConstant6) {


	RETURN_STRING("test", 1);

}

