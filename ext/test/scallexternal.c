
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

	zend_declare_property_null(test_scallexternal_ce, SL("sproperty"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_ScallExternal, testCall1) {

	ZEPHIR_MM_GROW();

	zephir_call_static(return_value, "Test\\Scall", "testmethod1");
	RETURN_MM();

}

PHP_METHOD(Test_ScallExternal, testCall2) {

	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	zephir_call_static_p2(return_value, "Test\\Scall", "testmethod4", a, b);
	RETURN_MM();

}

