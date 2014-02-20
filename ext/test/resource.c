
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
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Resource) {

	ZEPHIR_REGISTER_CLASS(Test, Resource, test, resource, test_resource_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Resource, testLetStatementSTDIN) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZEPHIR_GET_CONSTANT(a, "STDIN");
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Resource, testLetStatementSTDOUT) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZEPHIR_GET_CONSTANT(a, "STDOUT");
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Resource, testLetStatementSTDERR) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZEPHIR_GET_CONSTANT(a, "STDERR");
	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Resource, testTypeOffResource) {

	zval *a = NULL, *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZEPHIR_GET_CONSTANT(a, "STDIN");
	ZEPHIR_INIT_VAR(_0);
	zephir_gettype(_0, a TSRMLS_CC);
	RETURN_CCTOR(_0);

}

