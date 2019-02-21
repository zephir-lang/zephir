
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Mcall_Caller) {

	ZEPHIR_REGISTER_CLASS(Test\\Mcall, Caller, test, mcall_caller, test_mcall_caller_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Mcall_Caller, start) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *f, f_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&f_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &f);



	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(f, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Mcall_Caller, perform) {

	zval *this_ptr = getThis();


	RETURN_STRING("Caller:perform");

}

