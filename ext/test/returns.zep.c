
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
#include "kernel/memory.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Returns) {

	ZEPHIR_REGISTER_CLASS(Test, Returns, test, returns, test_returns_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Returns, testReturnCast1) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG((int) 5.0);

}

PHP_METHOD(Test_Returns, testReturnCast2) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG(1);

}

PHP_METHOD(Test_Returns, testReturnCast3) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_LONG(0);

}

PHP_METHOD(Test_Returns, testReturnCast4) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	if (1) {
		RETURN_MM_LONG(1);
	} else {
		RETURN_MM_LONG(0);
	}

}

PHP_METHOD(Test_Returns, returnWithParameter) {

	zval *parameter_param = NULL;
	zval parameter;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&parameter);

	zephir_fetch_params(1, 1, 0, &parameter_param);

	zephir_get_strval(&parameter, parameter_param);


	RETURN_MM_STRING("Return back");

}

PHP_METHOD(Test_Returns, returnWithoutParameter) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("Return back");

}

