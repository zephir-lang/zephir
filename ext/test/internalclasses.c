
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


ZEPHIR_INIT_CLASS(Test_InternalClasses) {

	ZEPHIR_REGISTER_CLASS(Test, InternalClasses, test, internalclasses, test_internalclasses_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_InternalClasses, testStaticCall) {

	int ZEPHIR_LAST_CALL_STATUS;

	ZEPHIR_MM_GROW();

	ZEPHIR_RETURN_CALL_CE_STATIC(zend_fetch_class(SL("\\Phalcon\\DI"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC), "getdefault", NULL);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_InternalClasses, testStaticPropertyFetch) {


	RETURN_LONG(303);

}

