
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
#include "kernel/fcall.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_MethodAbstract) {

	ZEPHIR_REGISTER_CLASS(Test, MethodAbstract, test, methodabstract, test_methodabstract_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_class_implements(test_methodabstract_ce TSRMLS_CC, 1, test_methodinterface_ce);
	return SUCCESS;

}

PHP_METHOD(Test_MethodAbstract, testInterfaceMetho) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(NULL, this_ptr, "testmethod", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

