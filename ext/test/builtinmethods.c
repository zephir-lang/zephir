
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
#include "kernel/string.h"
#include "kernel/memory.h"
#include "kernel/concat.h"
#include "kernel/operators.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Test_BuiltInMethods) {

	ZEPHIR_REGISTER_CLASS(Test, BuiltInMethods, test, builtinmethods, test_builtinmethods_method_entry, 0);


	return SUCCESS;

}

PHP_METHOD(Test_BuiltInMethods, stringMethodLength1) {

	zval _0;


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "hello", 0);
	RETURN_LONG(zephir_fast_strlen_ev(&_0));

}

PHP_METHOD(Test_BuiltInMethods, stringMethodLength2) {

	zval _0;


	ZEPHIR_SINIT_VAR(_0);
	ZVAL_STRING(&_0, "hello", 0);
	RETURN_LONG(zephir_fast_strlen_ev(&_0));

}

PHP_METHOD(Test_BuiltInMethods, stringMethodLength3) {

	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CONCAT_SS(_0, "hello", "hello");
	RETURN_MM_LONG(zephir_fast_strlen_ev(_0));

}

PHP_METHOD(Test_BuiltInMethods, stringMethodLength4) {

	zval *a_param = NULL;
	zval *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

		zephir_get_strval(a, a_param);


	RETURN_MM_LONG(zephir_fast_strlen_ev(a));

}

PHP_METHOD(Test_BuiltInMethods, stringMethodLength5) {

	zval *a_param = NULL;
	zval *a = NULL, *_0;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

		zephir_get_strval(a, a_param);


	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_CONCAT_SV(_0, "hello", a);
	RETURN_MM_LONG(zephir_fast_strlen_ev(_0));

}

PHP_METHOD(Test_BuiltInMethods, stringMethodTrim1) {

	zval *_0, _1;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	ZEPHIR_SINIT_VAR(_1);
	ZVAL_STRING(&_1, " hello \t\n", 0);
	zephir_fast_trim(_0, &_1, NULL , ZEPHIR_TRIM_BOTH TSRMLS_CC);
	RETURN_CCTOR(_0);

}

