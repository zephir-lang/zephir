
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"

/* hello */

TEST_INIT_CLASS(Test_Router) {

	TEST_REGISTER_CLASS(Test, Router, router, test_router_method_entry, 0);

	zend_declare_property_null(test_router_ce, SL("_dependencyInjector"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_null(test_router_ce, SL("_uriSource"), ZEND_ACC_PROTECTED TSRMLS_CC);
	zend_declare_property_bool(test_router_ce, SL("_wasMatched"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

}

PHP_METHOD(Test_Router, __construct) {

}

PHP_METHOD(Test_Router, setDI) {

	int a, b, c;
	double d, e, f;
	fprintf(stdout, "%d", a);
}

PHP_METHOD(Test_Router, getDI) {

}

