
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


/**
 * Printing output
 */
ZEPHIR_INIT_CLASS(Test_Echoes) {

	ZEPHIR_REGISTER_CLASS(Test, Echoes, test, echoes, test_echoes_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Echoes, testEcho1) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	php_printf("%d", 1);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Echoes, testEcho2) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	php_printf("%f", 1.0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Echoes, testEcho3) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	php_printf("%s", 1 ? "1": "");
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Echoes, testEcho4) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	php_printf("%s", 0 ? "1": "");
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Echoes, testEcho5) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	php_printf("%s", "hello");
	ZEPHIR_MM_RESTORE();

}

