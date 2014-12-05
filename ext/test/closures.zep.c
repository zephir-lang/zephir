
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


ZEPHIR_INIT_CLASS(Test_Closures) {

	ZEPHIR_REGISTER_CLASS(Test, Closures, test, closures, test_closures_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Closures, simple1) {


	zephir_create_closure_ex(return_value, NULL, test_1__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Closures, simple2) {


	zephir_create_closure_ex(return_value, NULL, test_2__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Closures, simple3) {


	zephir_create_closure_ex(return_value, NULL, test_3__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Closures, simple4) {


	zephir_create_closure_ex(return_value, NULL, test_4__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Closures, simple5) {


	zephir_create_closure_ex(return_value, NULL, test_5__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Closures, arrow1) {


	zephir_create_closure_ex(return_value, NULL, test_6__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Closures, arrow2) {


	zephir_create_closure_ex(return_value, NULL, test_7__closure_ce, SS("__invoke") TSRMLS_CC);
	return;

}

