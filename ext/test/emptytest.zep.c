
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
#include "kernel/operators.h"
#include "kernel/array.h"


/**
 * Control Flow
 */
/**
 * @link http://ru2.php.net/empty
 */
ZEPHIR_INIT_CLASS(Test_EmptyTest) {

	ZEPHIR_REGISTER_CLASS(Test, EmptyTest, test, emptytest, test_emptytest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_EmptyTest, testDynamicVarArrayEmpty) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	array_init(a);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(a));

}

PHP_METHOD(Test_EmptyTest, testDynamicVarArrayNotEmpty) {

	zval *a, *_0 = NULL;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	zephir_create_array(a, 4, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_NVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_fast_append(a, _0);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(a));

}

PHP_METHOD(Test_EmptyTest, testEmptyString) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "", 1);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(a));

}

PHP_METHOD(Test_EmptyTest, testNotEmptyString) {

	zval *a;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(a);
	ZVAL_STRING(a, "test string", 1);
	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(a));

}

PHP_METHOD(Test_EmptyTest, testString) {

	zval *a_param = NULL;
	zval *a = NULL;

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &a_param);

	zephir_get_strval(a, a_param);


	RETURN_MM_BOOL(ZEPHIR_IS_EMPTY(a));

}

