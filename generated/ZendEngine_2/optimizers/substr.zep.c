
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
#include "kernel/operators.h"
#include "kernel/string.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Optimizers_Substr) {

	ZEPHIR_REGISTER_CLASS(Test\\Optimizers, Substr, test, optimizers_substr, test_optimizers_substr_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Optimizers_Substr, testTwoArguments) {

	zval *str, *start;

	zephir_fetch_params(0, 2, 0, &str, &start);



	zephir_substr(return_value, str, zephir_get_intval(start), 0, ZEPHIR_SUBSTR_NO_LENGTH);
	return;

}

PHP_METHOD(Test_Optimizers_Substr, testThreeArguments) {

	zval *str, *start, *offset;

	zephir_fetch_params(0, 3, 0, &str, &start, &offset);



	zephir_substr(return_value, str, zephir_get_intval(start), zephir_get_intval(offset), 0);
	return;

}

