
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_ConstantsInterface) {

	ZEPHIR_REGISTER_CLASS(Test, ConstantsInterface, test, constantsinterface, test_constantsinterface_method_entry, 0);

	zend_class_implements(test_constantsinterface_ce TSRMLS_CC, 1, test_testinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant1) {

	zval *this_ptr = getThis();


	RETURN_NULL();

}

PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant2) {

	zval *this_ptr = getThis();


	RETURN_BOOL(0);

}

PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant3) {

	zval *this_ptr = getThis();


	RETURN_BOOL(1);

}

PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant4) {

	zval *this_ptr = getThis();


	RETURN_LONG(10);

}

PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant5) {

	zval *this_ptr = getThis();


	RETURN_DOUBLE(10.25);

}

PHP_METHOD(Test_ConstantsInterface, testReadInterfaceConstant6) {

	zval *this_ptr = getThis();


	RETURN_STRING("test");

}

PHP_METHOD(Test_ConstantsInterface, testReadInheritanceFromInterfaceConstant1) {

	zval *this_ptr = getThis();


	RETURN_NULL();

}

PHP_METHOD(Test_ConstantsInterface, testReadInheritanceFromInterfaceConstant2) {

	zval *this_ptr = getThis();


	RETURN_BOOL(0);

}

PHP_METHOD(Test_ConstantsInterface, testReadInheritanceFromInterfaceConstant3) {

	zval *this_ptr = getThis();


	RETURN_BOOL(1);

}

PHP_METHOD(Test_ConstantsInterface, testReadInheritanceFromInterfaceConstant4) {

	zval *this_ptr = getThis();


	RETURN_LONG(10);

}

PHP_METHOD(Test_ConstantsInterface, testReadInheritanceFromInterfaceConstant5) {

	zval *this_ptr = getThis();


	RETURN_DOUBLE(10.25);

}

PHP_METHOD(Test_ConstantsInterface, testReadInheritanceFromInterfaceConstant6) {

	zval *this_ptr = getThis();


	RETURN_STRING("test");

}

