
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


ZEPHIR_INIT_CLASS(Stub_InternalClasses) {

	ZEPHIR_REGISTER_CLASS(Stub, InternalClasses, stub, internalclasses, stub_internalclasses_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_InternalClasses, testStaticCall) {

	zend_class_entry *_0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();



	ZEPHIR_MM_GROW();

	_0 = zephir_fetch_class_str_ex(SL("Phalcon\\Di"), ZEND_FETCH_CLASS_AUTO);
	ZEPHIR_RETURN_CALL_CE_STATIC(_0, "getdefault", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Stub_InternalClasses, testStaticPropertyFetch) {

	zval *this_ptr = getThis();



	RETURN_LONG(303);

}

