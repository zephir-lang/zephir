
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


/**
 * Call external static functions
 */
ZEPHIR_INIT_CLASS(Test_ScallExternal) {

	ZEPHIR_REGISTER_CLASS(Test, ScallExternal, test, scallexternal, test_scallexternal_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_ScallExternal, testCall1) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_static_p2(return_value, "Test\\Scall", "testmethod4", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_ScallExternal, testCall2) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_static_p2(return_value, "Test\\Scall", "testmethod5", a, b);
	RETURN_MM();

}

PHP_METHOD(Test_ScallExternal, testCall3) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_static_p2(return_value, "Test\\Scall", "testmethod6", a, b);
	RETURN_MM();

}

