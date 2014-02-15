
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


ZEPHIR_INIT_CLASS(Test_UseTest) {

	ZEPHIR_REGISTER_CLASS(Test, UseTest, test, usetest, test_usetest_method_entry, 0);

	zend_class_implements(test_usetest_ce TSRMLS_CC, 1, spl_ce_Countable);

	return SUCCESS;

}

PHP_METHOD(Test_UseTest, createInstance) {


	object_init(return_value);
	return;

}

PHP_METHOD(Test_UseTest, count) {



}

