
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


ZEPHIR_INIT_CLASS(Stub_Oo_AbstractClass) {

	ZEPHIR_REGISTER_CLASS(Stub\\Oo, AbstractClass, stub, oo_abstractclass, stub_oo_abstractclass_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	return SUCCESS;

}

PHP_METHOD(Stub_Oo_AbstractClass, testMethodDeclaration) {

}

PHP_METHOD(Stub_Oo_AbstractClass, testMethodDeclarationWithReturnType) {

}

PHP_METHOD(Stub_Oo_AbstractClass, testMethodDeclarationWithParameter) {

}

PHP_METHOD(Stub_Oo_AbstractClass, testMethodDeclarationWithParameterAndReturnType) {

}

PHP_METHOD(Stub_Oo_AbstractClass, testAbstractMethodDeclaration) {

}

PHP_METHOD(Stub_Oo_AbstractClass, testAbstractMethodDeclarationWithReturnType) {

}

PHP_METHOD(Stub_Oo_AbstractClass, testAbstractMethodDeclarationWithParameter) {

}

PHP_METHOD(Stub_Oo_AbstractClass, testAbstractMethodDeclarationWithParameterAndReturnType) {

}

