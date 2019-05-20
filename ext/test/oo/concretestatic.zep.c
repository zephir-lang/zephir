
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/object.h"
#include "kernel/memory.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Oo_ConcreteStatic) {

	ZEPHIR_REGISTER_CLASS_EX(Test\\Oo, ConcreteStatic, test, oo_concretestatic, test_oo_abstractstatic_ce, test_oo_concretestatic_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Oo_ConcreteStatic, parentFunction) {

	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();

	RETURN_MM_STRING("ConcreteStatic:parentFunction");

}

PHP_METHOD(Test_Oo_ConcreteStatic, childFunction) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();


	ZEPHIR_RETURN_CALL_SELF("parentfunction", NULL, 0);
	zephir_check_call_status();
	RETURN_MM();

}

