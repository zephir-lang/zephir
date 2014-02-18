
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
#include "kernel/require.h"
#include "kernel/memory.h"


ZEPHIR_INIT_CLASS(Test_Requires) {

	ZEPHIR_REGISTER_CLASS(Test, Requires, test, requires, test_requires_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Requires, requireExternal1) {

	zval *path;

	zephir_fetch_params(0, 1, 0, &path);



	if (zephir_require_ret(return_value, path TSRMLS_CC) == FAILURE) {
		RETURN_NULL();
	}
	return;

}

PHP_METHOD(Test_Requires, requireExternal2) {

	zval *path, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_INIT_VAR(_0);
	if (zephir_require_ret(_0, path TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_MM_BOOL(1);

}

