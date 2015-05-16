
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
#include "ext/spl/spl_iterators.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_UseTest) {

	ZEPHIR_REGISTER_CLASS(Test, UseTest, test, usetest, test_usetest_method_entry, 0);

	zend_class_implements(test_usetest_ce TSRMLS_CC, 1, spl_ce_Countable);
	return SUCCESS;

}

PHP_METHOD(Test_UseTest, createInstance) {


	object_init(return_value);
	return;

}

PHP_METHOD(Test_UseTest, count) {



}

PHP_METHOD(Test_UseTest, testUseClass1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zend_class_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	if (!_0) {
		_0 = zend_fetch_class(SL("Oo\\OoConstruct"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	}
	object_init_ex(return_value, _0);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

PHP_METHOD(Test_UseTest, testUseClass2) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zend_class_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	if (!_0) {
		_0 = zend_fetch_class(SL("Strings"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	}
	object_init_ex(return_value, _0);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

PHP_METHOD(Test_UseTest, testUseNamespaceAlias) {

	int ZEPHIR_LAST_CALL_STATUS;
	zephir_nts_static zend_class_entry *_0 = NULL;

	ZEPHIR_MM_GROW();

	if (!_0) {
		_0 = zend_fetch_class(SL("Oo\\OoConstruct"), ZEND_FETCH_CLASS_AUTO TSRMLS_CC);
	}
	object_init_ex(return_value, _0);
	if (zephir_has_constructor(return_value TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, return_value, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	RETURN_MM();

}

