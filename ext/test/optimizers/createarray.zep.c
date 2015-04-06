
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
#include "kernel/array.h"
#include "kernel/memory.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Test_Optimizers_CreateArray) {

	ZEPHIR_REGISTER_CLASS(Test\\Optimizers, CreateArray, test, optimizers_createarray, test_optimizers_createarray_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Optimizers_CreateArray, createNoSize) {

	int ZEPHIR_LAST_CALL_STATUS;


	zephir_create_array(return_value, 0, 1 TSRMLS_CC);
	return;

}

PHP_METHOD(Test_Optimizers_CreateArray, createSize) {

	zval *n_param = NULL, _0;
	int n, ZEPHIR_LAST_CALL_STATUS;

	zephir_fetch_params(0, 1, 0, &n_param);

	n = zephir_get_intval(n_param);


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_LONG(&_0, n);
	zephir_create_array(return_value, zephir_get_intval(&_0), 1 TSRMLS_CC);
	return;

}

