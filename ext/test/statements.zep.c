
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
#include "kernel/operators.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Statements) {

	ZEPHIR_REGISTER_CLASS(Test, Statements, test, statements, test_statements_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Statements, testElseIf) {

	zval *num_param = NULL;
	int num;

	zephir_fetch_params(0, 1, 0, &num_param);

	num = zephir_get_intval(num_param);


	if (num > 0) {
		RETURN_STRING("more", 1);
	} else if (num == 0) {
		RETURN_STRING("equal", 1);
	} else if (num == -1) {
		RETURN_STRING("-1", 1);
	} else {
		RETURN_STRING("less", 1);
	}

}

