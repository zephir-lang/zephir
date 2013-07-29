
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
#include "kernel/fcall.h"


/**
 * Arithmetic operations
 */
ZEPHIR_INIT_CLASS(Test_Oo) {

	ZEPHIR_REGISTER_CLASS(Test, Oo, oo, test_oo_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_Oo, testInstance1) {

	zval *o;

	object_init(o);

	RETURN_CCTORW(o);


}

PHP_METHOD(Test_Oo, testInstance2) {

	zval *o;

	object_init_ex(o, test_oo_ooconstruct_ce);
	zephir_call_method_noret(o, "__construct");

	RETURN_CCTORW(o);


}

PHP_METHOD(Test_Oo, testInstance3) {

	zval *o;

	object_init_ex(o, test_oo_oonoconstruct_ce);

	RETURN_CCTORW(o);


}

