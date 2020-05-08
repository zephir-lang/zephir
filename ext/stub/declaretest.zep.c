
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
#include "kernel/object.h"
#include "kernel/fcall.h"


/**
 * Variable declaration
 */
ZEPHIR_INIT_CLASS(Stub_DeclareTest) {

	ZEPHIR_REGISTER_CLASS(Stub, DeclareTest, stub, declaretest, stub_declaretest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_DeclareTest, testStringDeclare1) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "/@(\\w+)(?:\\s*(?:\\(\\s*)?(.*?)(?:\\s*\\))?)??\\s*(?:\n|\\*\\/)/");
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_DeclareTest, testStringDeclare2) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "/(\\w+)\\s*=\\s*(\\[[^\\]]*\\]|\"[^\"]*\"|[^,)]*)\\s*(?:,|$)/");
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare1) {

	zend_long a;
	zval *this_ptr = getThis();



	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare2) {

	zend_ulong a;
	zval *this_ptr = getThis();



	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare3) {

	double a;
	zval *this_ptr = getThis();



	a = 1.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare4) {

	double a;
	zval *this_ptr = getThis();



	a = 1.0;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare5) {

	char a;
	zval *this_ptr = getThis();



	a = 'A';
	RETURN_LONG(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare6) {

	unsigned char a;
	zval *this_ptr = getThis();



	a = 'A';
	RETURN_LONG(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare7) {

	long a;
	zval *this_ptr = getThis();



	a = 1;
	RETURN_LONG(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare8) {

	zend_bool a;
	zval *this_ptr = getThis();



	a = 1;
	RETURN_BOOL(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare9) {

	zend_bool a;
	zval *this_ptr = getThis();



	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare10) {

	zend_long a;
	zval *this_ptr = getThis();



	a = 10;
	RETURN_LONG(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare11) {

	double a;
	zval *this_ptr = getThis();



	a = 10.5;
	RETURN_DOUBLE(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare12) {

	zend_bool a;
	zval *this_ptr = getThis();



	a = 0;
	RETURN_BOOL(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare13) {

	zend_bool a;
	zval *this_ptr = getThis();



	a = 1;
	RETURN_BOOL(a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare14) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_STRING(&a, "hello");
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_DeclareTest, testDeclare15) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&a);
	ZVAL_NULL(&a);
	RETURN_CCTOR(&a);

}

PHP_METHOD(Stub_DeclareTest, testDeclareMcallExpression) {

	zval a;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&a);

	ZEPHIR_MM_GROW();

	ZEPHIR_CALL_METHOD(&a, this_ptr, "testdeclare14", NULL, 0);
	zephir_check_call_status();
	RETURN_CCTOR(&a);

}

