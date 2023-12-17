
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


/**
 * Chained method calls
 */
ZEPHIR_INIT_CLASS(Stub_McallChained)
{
	ZEPHIR_REGISTER_CLASS(Stub, McallChained, stub, mcallchained, stub_mcallchained_method_entry, 0);

	zend_declare_property_null(stub_mcallchained_ce, SL("temp"), ZEND_ACC_PRIVATE);
	return SUCCESS;
}

PHP_METHOD(Stub_McallChained, testMethod1)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

PHP_METHOD(Stub_McallChained, testMethod2)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

PHP_METHOD(Stub_McallChained, testMethod3)
{
	zval *this_ptr = getThis();
	RETURN_THISW();
}

PHP_METHOD(Stub_McallChained, testChained1)
{
	zval _0;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_0, "testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_McallChained, testChained2)
{
	zval _0, _1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, &_0, "testmethod3", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_1, "testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_McallChained, testChained3)
{
	zval _0, _1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod3", NULL, 64);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, &_0, "testmethod2", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_1, "testmethod1", NULL, 0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_McallChained, testChained4)
{
	zval _0, _1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	zephir_update_property_zval(this_ptr, ZEND_STRL("temp"), this_ptr);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("temp"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_CALL_METHOD(&_1, &_0, "testmethod1", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(&_1, "testmethod2", NULL, 0);
	zephir_check_call_status();
	return;
}

