
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_ScallParent) {

	ZEPHIR_REGISTER_CLASS(Test, ScallParent, scallparent, test_scallparent_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_ScallParent, testMethod1) {


	RETURN_STRING("hello parent public", 1);

}

PHP_METHOD(Test_ScallParent, testMethod2) {


	RETURN_STRING("hello parent protected", 1);

}

