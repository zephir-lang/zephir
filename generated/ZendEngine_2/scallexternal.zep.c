
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
#include "kernel/memory.h"
#include "kernel/operators.h"


/**
 * Call external static functions
 */
ZEPHIR_INIT_CLASS(Test_ScallExternal) {

	ZEPHIR_REGISTER_CLASS(Test, ScallExternal, test, scallexternal, test_scallexternal_method_entry, 0);

	zend_declare_property_null(test_scallexternal_ce, SL("sproperty"), ZEND_ACC_PUBLIC|ZEND_ACC_STATIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_ScallExternal, testCall1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_CE_STATIC(test_scall_ce, "testmethod1", &_0, 66);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_ScallExternal, testCall2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_fcall_cache_entry *_0 = NULL;
	zval *a, *b;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &a, &b);



	ZEPHIR_RETURN_CALL_CE_STATIC(test_scall_ce, "testmethod4", &_0, 67, a, b);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_ScallExternal, testMethod3) {

	zval *a_param = NULL, *b_param = NULL;
	long a, b;

	zephir_fetch_params(0, 2, 0, &a_param, &b_param);

	a = zephir_get_intval(a_param);
	b = zephir_get_intval(b_param);


	RETURN_LONG((a + b));

}

