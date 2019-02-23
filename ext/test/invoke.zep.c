
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
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Invoke) {

	ZEPHIR_REGISTER_CLASS(Test, Invoke, test, invoke, test_invoke_method_entry, 0);

	zend_declare_property_null(test_invoke_ce, SL("a"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Invoke, __construct) {

	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "aaa");
	zephir_update_property_zval(this_ptr, SL("a"), &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Invoke, __invoke) {

	zval _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	zephir_read_property(&_0, this_ptr, SL("a"), PH_NOISY_CC | PH_READONLY);
	zephir_gettype(return_value, &_0 TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Invoke, test) {

	zval func;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&func);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&func);
	object_init_ex(&func, test_invoke_ce);
	ZEPHIR_CALL_METHOD(NULL, &func, "__construct", NULL, 43);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_ZVAL_FUNCTION(&func, NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

