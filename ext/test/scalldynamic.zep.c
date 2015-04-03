
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


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_ScallDynamic) {

	ZEPHIR_REGISTER_CLASS_EX(Test, ScallDynamic, test, scalldynamic, test_scallparent_ce, test_scalldynamic_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_ScallDynamic, testMethod1) {


	RETURN_STRING("hello public", 1);

}

PHP_METHOD(Test_ScallDynamic, testMethod2) {


	RETURN_STRING("hello protected", 1);

}

PHP_METHOD(Test_ScallDynamic, testMethod3) {


	RETURN_STRING("hello private", 1);

}

PHP_METHOD(Test_ScallDynamic, selfDynamicCall1) {

	zval *methodName;

	zephir_fetch_params(0, 1, 0, &methodName);




}

