
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


	RETURN_STRING("hello parent public", 1);

}

PHP_METHOD(Test_ScallParent, testMethod2) {


	RETURN_STRING("hello parent protected", 1);

}

PHP_METHOD(Test_ScallParent, testCallStatic) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_STATIC("testmethodstatic", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_ScallParent, testMethodStatic) {


	RETURN_STRING("hello ScallParent", 1);

}

