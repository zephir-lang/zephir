
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
 * Function calls
 */
ZEPHIR_INIT_CLASS(Test_Fcall) {

	ZEPHIR_REGISTER_CLASS(Test, Fcall, fcall, test_fcall_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Fcall, testCall1) {

	zval _0, _1;

	ZEPHIR_MM_GROW();

	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "hello", 0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, "h", 0);
	zephir_call_func_p2(return_value, "strpos", &_0, &_1);
	RETURN_MM();

}

