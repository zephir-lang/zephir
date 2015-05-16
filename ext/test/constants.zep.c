
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


ZEPHIR_INIT_CLASS(Test_Constants) {

	ZEPHIR_REGISTER_CLASS_EX(Test, Constants, test, constants, test_constantsparent_ce, test_constants_method_entry, 0);

	zend_declare_property_null(test_constants_ce, SL("propertyC1"), ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_bool(test_constants_ce, SL("propertyC2"), 0, ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_bool(test_constants_ce, SL("propertyC3"), 1, ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_long(test_constants_ce, SL("propertyC4"), 10, ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_double(test_constants_ce, SL("propertyC5"), 10.25, ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_string(test_constants_ce, SL("propertyC6"), "test", ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_string(test_constants_ce, SL("propertyC7"), "/@(\\w+)(?:\\s*(?:\\(\\s*)?(.*?)(?:\\s*\\))?)??\\s*(?:\n|\\*\\/)/", ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_property_string(test_constants_ce, SL("propertyC8"), "/(\\w+)\\s*=\\s*(\\[[^\\]]*\\]|\"[^\"]*\"|[^,)]*)\\s*(?:,|$)/", ZEND_ACC_PROTECTED TSRMLS_CC);

	zend_declare_class_constant_null(test_constants_ce, SL("C1") TSRMLS_CC);

	zend_declare_class_constant_bool(test_constants_ce, SL("C2"), 0 TSRMLS_CC);

	zend_declare_class_constant_bool(test_constants_ce, SL("C3"), 1 TSRMLS_CC);

	zend_declare_class_constant_long(test_constants_ce, SL("C4"), 10 TSRMLS_CC);

	zend_declare_class_constant_double(test_constants_ce, SL("C5"), 10.25 TSRMLS_CC);

	zend_declare_class_constant_string(test_constants_ce, SL("C6"), "test" TSRMLS_CC);

	zend_declare_class_constant_string(test_constants_ce, SL("className"), "Test\\Constants" TSRMLS_CC);

	zend_declare_class_constant_long(test_constants_ce, SL("STD_PROP_LIST"), 1 TSRMLS_CC);

	/**
	 * Test property addSlashes for constants
	 */
	zend_declare_class_constant_string(test_constants_ce, SL("ANNOTATION_REGEX"), "/@(\\w+)(?:\\s*(?:\\(\\s*)?(.*?)(?:\\s*\\))?)??\\s*(?:\n|\\*\\/)/" TSRMLS_CC);

	zend_declare_class_constant_string(test_constants_ce, SL("PARAMETER_REGEX"), "/(\\w+)\\s*=\\s*(\\[[^\\]]*\\]|\"[^\"]*\"|[^,)]*)\\s*(?:,|$)/" TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Constants, getPropertyC1) {


	RETURN_MEMBER(this_ptr, "propertyC1");

}

PHP_METHOD(Test_Constants, getPropertyC2) {


	RETURN_MEMBER(this_ptr, "propertyC2");

}

PHP_METHOD(Test_Constants, getPropertyC3) {


	RETURN_MEMBER(this_ptr, "propertyC3");

}

PHP_METHOD(Test_Constants, getPropertyC4) {


	RETURN_MEMBER(this_ptr, "propertyC4");

}

PHP_METHOD(Test_Constants, getPropertyC5) {


	RETURN_MEMBER(this_ptr, "propertyC5");

}

PHP_METHOD(Test_Constants, getPropertyC6) {


	RETURN_MEMBER(this_ptr, "propertyC6");

}

PHP_METHOD(Test_Constants, getPropertyC7) {


	RETURN_MEMBER(this_ptr, "propertyC7");

}

PHP_METHOD(Test_Constants, getPropertyC8) {


	RETURN_MEMBER(this_ptr, "propertyC8");

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

PHP_METHOD(Test_Constants, testPHPVersionEnvConstant) {


	ZEPHIR_GET_CONSTANT(return_value, "PHP_VERSION");
	return;

}

PHP_METHOD(Test_Constants, testClassMagicConstant) {


	RETURN_STRING("Test\\Constants", 1);

}

PHP_METHOD(Test_Constants, testMethodMagicConstant) {


	RETURN_STRING("Constants:testMethodMagicConstant", 1);

}

PHP_METHOD(Test_Constants, testFunctionMagicConstant) {


	RETURN_STRING("testFunctionMagicConstant", 1);

}

PHP_METHOD(Test_Constants, testNamespaceMagicConstant) {


	RETURN_STRING("Test", 1);

}

PHP_METHOD(Test_Constants, testDirConstant) {


	RETURN_NULL();

}

PHP_METHOD(Test_Constants, testPHPVersionEnvConstantInExpValue) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZEPHIR_GET_CONSTANT(a, "PHP_VERSION");
	RETURN_CCTOR(a);

}

