
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


ZEPHIR_INIT_CLASS(Test_TypeHinting_TestAbstract) {

	ZEPHIR_REGISTER_CLASS(Test\\TypeHinting, TestAbstract, test, typehinting_testabstract, test_typehinting_testabstract_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;

}

PHP_METHOD(Test_TypeHinting_TestAbstract, testFunc) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnOneOfScalar) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnInt) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnUint) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnLong) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnFloat) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnDouble) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnString) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnBoolean) {

}

PHP_METHOD(Test_TypeHinting_TestAbstract, returnChar) {

}

