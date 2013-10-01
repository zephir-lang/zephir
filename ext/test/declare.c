
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


/**
 * Variable declaration
 */
ZEPHIR_INIT_CLASS(Test_Declare) {

	ZEPHIR_REGISTER_CLASS(Test, Declare, test, declare, test_declare_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Declare, testDeclare1) {

	int a = 1;


	RETURN_LONG(a);

}

PHP_METHOD(Test_Declare, testDeclare2) {

	unsigned int a = 1;


	RETURN_LONG(a);

}

PHP_METHOD(Test_Declare, testDeclare3) {

	double a = 1.0;


	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Declare, testDeclare4) {

	double a = 1.0;


	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Declare, testDeclare5) {

	char a = 'A';


	RETURN_LONG(a);

}

PHP_METHOD(Test_Declare, testDeclare6) {

	unsigned char a = 'A';


	RETURN_LONG(a);

}

PHP_METHOD(Test_Declare, testDeclare7) {

	long a = 1;


	RETURN_LONG(a);

}

PHP_METHOD(Test_Declare, testDeclare8) {

	zend_bool a = 1;


	RETURN_BOOL(a);

}

PHP_METHOD(Test_Declare, testDeclare9) {

	zend_bool a = 0;


	RETURN_BOOL(a);

}

PHP_METHOD(Test_Declare, testDeclare10) {

	int a = 10;


	RETURN_LONG(a);

}

PHP_METHOD(Test_Declare, testDeclare11) {

	double a = 10.5;


	RETURN_DOUBLE(a);

}

PHP_METHOD(Test_Declare, testDeclare12) {

	zend_bool a = 0;


	RETURN_BOOL(a);

}

PHP_METHOD(Test_Declare, testDeclare13) {

	zend_bool a = 1;


	RETURN_BOOL(a);

}

PHP_METHOD(Test_Declare, testDeclare14) {

	zval *a;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "hello", 1);

	RETURN_CCTOR(a);

}

PHP_METHOD(Test_Declare, testDeclare15) {



}

