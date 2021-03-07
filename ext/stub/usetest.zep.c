
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
#include "kernel/fcall.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Stub_UseTest) {

	ZEPHIR_REGISTER_CLASS(Stub, UseTest, stub, usetest, stub_usetest_method_entry, 0);

	zend_class_implements(stub_usetest_ce, 1, zend_ce_countable);
	return SUCCESS;

}

PHP_METHOD(Stub_UseTest, createInstance) {

	zval *this_ptr = getThis();



	object_init(return_value);
	return;

}

PHP_METHOD(Stub_UseTest, count) {

	zval *this_ptr = getThis();




}

PHP_METHOD(Stub_UseTest, testUseClass1) {

	zend_class_entry *_0 = NULL;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();



	ZEPHIR_MM_GROW();

	if (!_0) {
	_0 = zephir_fetch_class_str_ex(SL("Oo\\OoConstruct"), ZEND_FETCH_CLASS_AUTO);
	}
	object_init_ex(return_value, _0);
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

PHP_METHOD(Stub_UseTest, testUseClass2) {

	zend_class_entry *_0 = NULL;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();



	ZEPHIR_MM_GROW();

	if (!_0) {
	_0 = zephir_fetch_class_str_ex(SL("Strings"), ZEND_FETCH_CLASS_AUTO);
	}
	object_init_ex(return_value, _0);
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

PHP_METHOD(Stub_UseTest, testUseNamespaceAlias) {

	zend_class_entry *_0 = NULL;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();



	ZEPHIR_MM_GROW();

	if (!_0) {
	_0 = zephir_fetch_class_str_ex(SL("Oo\\OoConstruct"), ZEND_FETCH_CLASS_AUTO);
	}
	object_init_ex(return_value, _0);
	if (zephir_has_constructor(return_value)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

