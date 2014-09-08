
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


ZEPHIR_INIT_CLASS(Test_Closures) {

	ZEPHIR_REGISTER_CLASS(Test, Closures, test, closures, test_closures_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Closures, simple1) {


	RETURN_NULL();

}

PHP_METHOD(Test_Closures, simple2) {


	RETURN_NULL();

}

PHP_METHOD(Test_Closures, simple3) {


	RETURN_NULL();

}

PHP_METHOD(Test_Closures, simple4) {


	RETURN_NULL();

}

PHP_METHOD(Test_Closures, simple5) {


	RETURN_NULL();

}

