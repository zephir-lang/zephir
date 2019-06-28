
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
#include "kernel/fcall.h"
#include "kernel/memory.h"


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_ScallDynamic) {

	ZEPHIR_REGISTER_CLASS_EX(Test, ScallDynamic, test, scalldynamic, test_scallparent_ce, test_scalldynamic_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_ScallDynamic, testMethod1) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello public");

}

PHP_METHOD(Test_ScallDynamic, testMethod2) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello protected");

}

PHP_METHOD(Test_ScallDynamic, testMethod3) {

	zval *this_ptr = getThis();


	RETURN_STRING("hello private");

}

PHP_METHOD(Test_ScallDynamic, selfDynamicCall1) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *methodName, methodName_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&methodName_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &methodName);



	return;

}

