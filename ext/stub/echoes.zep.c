
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


/**
 * Printing output
 */
ZEPHIR_INIT_CLASS(Stub_Echoes) {

	ZEPHIR_REGISTER_CLASS(Stub, Echoes, stub, echoes, stub_echoes_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Echoes, testEcho1) {

	zval *this_ptr = getThis();



	php_printf("%d", 1);

}

PHP_METHOD(Stub_Echoes, testEcho2) {

	zval *this_ptr = getThis();



	php_printf("%f", 1.0);

}

PHP_METHOD(Stub_Echoes, testEcho3) {

	zval *this_ptr = getThis();



	php_printf("%s", 1 ? "1": "");

}

PHP_METHOD(Stub_Echoes, testEcho4) {

	zval *this_ptr = getThis();



	php_printf("%s", 0 ? "1": "");

}

PHP_METHOD(Stub_Echoes, testEcho5) {

	zval *this_ptr = getThis();



	php_printf("%s", "hello");

}

