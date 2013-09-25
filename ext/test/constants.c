
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


ZEPHIR_INIT_CLASS(Test_Constants) {

	ZEPHIR_REGISTER_CLASS_EX(Test, Constants, test, constants, test_constantsparent_ce, test_constants_method_entry, 0);

	zend_declare_class_constant_null(test_constants_ce, SL("C1") TSRMLS_CC);
	zend_declare_class_constant_bool(test_constants_ce, SL("C2"), 0 TSRMLS_CC);
	zend_declare_class_constant_bool(test_constants_ce, SL("C3"), 1 TSRMLS_CC);
	zend_declare_class_constant_long(test_constants_ce, SL("C4"), 10 TSRMLS_CC);
	zend_declare_class_constant_double(test_constants_ce, SL("C5"), 10.25 TSRMLS_CC);
	zend_declare_class_constant_string(test_constants_ce, SL("C6"), "test" TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Constants, testReadConstant) {


	RETURN_LONG(3);

}

PHP_METHOD(Test_Constants, testReadClassConstant1) {


	RETURN_LONG(10);

}

PHP_METHOD(Test_Constants, testReadClassConstant2) {


	RETURN_LONG(10);

}

PHP_METHOD(Test_Constants, testReadClassConstant3) {


	RETURN_LONG(10);

}

