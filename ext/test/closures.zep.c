
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


ZEPHIR_INIT_CLASS(Test_Closures) {

	ZEPHIR_REGISTER_CLASS(Test, Closures, test, closures, test_closures_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Closures, simple1) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, test_1__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Test_Closures, simple2) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, test_2__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Test_Closures, simple3) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, test_3__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Test_Closures, simple4) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, test_4__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Test_Closures, simple5) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, test_5__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Test_Closures, arrow1) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, test_6__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Test_Closures, arrow2) {

	zval *this_ptr = getThis();


	zephir_create_closure_ex(return_value, NULL, test_7__closure_ce, SL("__invoke"));
	return;

}

PHP_METHOD(Test_Closures, testUseCommand) {

	zval _0;
	zend_long abc;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);


	abc = 1;
	zephir_create_closure_ex(return_value, NULL, test_8__closure_ce, SL("__invoke"));
	ZEPHIR_INIT_ZVAL_NREF(_0);
	ZVAL_LONG(&_0, abc);
	zend_update_static_property(test_8__closure_ce, ZEND_STRL("abc"), &_0);
	return;

}

