
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
#include "kernel/array.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_Issue1521) {

	ZEPHIR_REGISTER_CLASS(Test, Issue1521, test, issue1521, test_issue1521_method_entry, 0);

	zend_declare_property_null(test_issue1521_ce, SL("params"), ZEND_ACC_PUBLIC TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Issue1521, test) {

	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0 TSRMLS_CC);
	add_assoc_stringl_ex(&_0, SL("merchantNumber"), SL("0818217122"));
	add_assoc_stringl_ex(&_0, SL("zero"), SL("0"));
	zephir_update_property_zval(this_ptr, SL("params"), &_0);
	RETURN_MM_MEMBER(getThis(), "params");

}

