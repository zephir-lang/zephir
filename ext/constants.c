
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
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_Constants) {

	ZEPHIR_REGISTER_CLASS_EX(Test, Constants, constants, "test\\constantsparent", test_constants_method_entry, 0);

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


	zephir_get_class_constant(return_value, test_constants_ce, SS("C4") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Constants, testReadClassConstant2) {


	zephir_get_class_constant(return_value, test_constants_ce, SS("C4") TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Constants, testReadClassConstant3) {


	zephir_get_class_constant(return_value, test_constantsparent_ce, SS("P4") TSRMLS_CC);
	return;

}

