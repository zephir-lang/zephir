
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
#include "kernel/memory.h"
#include "kernel/fcall.h"


/**
 * Static Function calls
 */
ZEPHIR_INIT_CLASS(Test_Scall) {

	ZEPHIR_REGISTER_CLASS(Test, Scall, scall, test_scall_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Scall, testMethod1) {

	RETURN_STRING("hello public", 1);


}

PHP_METHOD(Test_Scall, testMethod2) {

	RETURN_STRING("hello protected", 1);


}

PHP_METHOD(Test_Scall, testCall1) {

	zval *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CALL_SELF(_0, this_ptr, test_scall_ce, "testmethod1");
	RETURN_CCTOR(_0);


}

PHP_METHOD(Test_Scall, testCall2) {

	zval *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CALL_SELF(_0, this_ptr, test_scall_ce, "testmethod2");
	RETURN_CCTOR(_0);


}

