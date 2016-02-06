
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
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_ScallParent) {

	ZEPHIR_REGISTER_CLASS(Test, ScallParent, test, scallparent, test_scallparent_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_ScallParent, testMethod1) {

	ZEPHIR_INIT_THIS();


	RETURN_STRING("hello parent public");

}

PHP_METHOD(Test_ScallParent, testMethod2) {

	ZEPHIR_INIT_THIS();


	RETURN_STRING("hello parent protected");

}

PHP_METHOD(Test_ScallParent, testCallStatic) {

	int ZEPHIR_LAST_CALL_STATUS;
	ZEPHIR_INIT_THIS();


	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_STATIC("testmethodstatic", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_ScallParent, testMethodStatic) {

	ZEPHIR_INIT_THIS();


	RETURN_STRING("hello ScallParent");

}

