
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
#include "kernel/fcall.h"
#include "kernel/object.h"


/**
 * Chained method calls
 */
ZEPHIR_INIT_CLASS(Test_McallChained) {

	ZEPHIR_REGISTER_CLASS(Test, McallChained, test, mcallchained, test_mcallchained_method_entry, 0);

	zend_declare_property_null(test_mcallchained_ce, SL("temp"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_McallChained, testMethod1) {


	RETURN_THISW();

}

PHP_METHOD(Test_McallChained, testMethod2) {


	RETURN_THISW();

}

PHP_METHOD(Test_McallChained, testMethod3) {


	RETURN_THISW();

}

PHP_METHOD(Test_McallChained, testChained1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_0, "testmethod2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_McallChained, testChained2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod1", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, _0, "testmethod3", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_1, "testmethod2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_McallChained, testChained3) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0 = NULL, *_1 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&_0, this_ptr, "testmethod3", NULL, 49);
	zephir_check_call_status();
	ZEPHIR_CALL_METHOD(&_1, _0, "testmethod2", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_1, "testmethod1", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_McallChained, testChained4) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_0, *_1 = NULL;

	ZEPHIR_MM_GROW();

	zephir_update_property_this(this_ptr, SL("temp"), this_ptr TSRMLS_CC);
	_0 = zephir_fetch_nproperty_this(this_ptr, SL("temp"), PH_NOISY_CC);
	ZEPHIR_CALL_METHOD(&_1, _0, "testmethod1", NULL, 0);
	zephir_check_call_status();
	ZEPHIR_RETURN_CALL_METHOD(_1, "testmethod2", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

