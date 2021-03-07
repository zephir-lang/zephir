
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
#include "kernel/object.h"
#include "kernel/memory.h"


/**
 * Static Function call via constructor and late binding
 */
ZEPHIR_INIT_CLASS(Stub_ScallLateConstruct) {

	ZEPHIR_REGISTER_CLASS(Stub, ScallLateConstruct, stub, scalllateconstruct, stub_scalllateconstruct_method_entry, 0);

	zend_declare_property_null(stub_scalllateconstruct_ce, SL("protectedVar"), ZEND_ACC_PROTECTED);

	return SUCCESS;

}

PHP_METHOD(Stub_ScallLateConstruct, __construct) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();



	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(NULL, this_ptr, "testpublicinit", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_ScallLateConstruct, testStaticInit) {

	zval *this_ptr = getThis();



	RETURN_STRING("hello public");

}

PHP_METHOD(Stub_ScallLateConstruct, testPublicInit) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_SELF(&_0, "teststaticinit", NULL, 0);
	zephir_check_call_status();
	zephir_update_property_zval(this_ptr, ZEND_STRL("protectedVar"), &_0);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Stub_ScallLateConstruct, varValue) {

	zval *this_ptr = getThis();



	RETURN_MEMBER(getThis(), "protectedVar");

}

