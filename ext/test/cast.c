
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
 * X
 */
ZEPHIR_INIT_CLASS(Test_Cast) {

	ZEPHIR_REGISTER_CLASS(Test, Cast, test, cast, test_cast_method_entry, 0);

	return SUCCESS;

}

/** To int cast */
PHP_METHOD(Test_Cast, testIntCastFromFloat) {


	RETURN_LONG((int) 5.0);

}

PHP_METHOD(Test_Cast, testIntCastFromBoolean) {


	RETURN_LONG(1);

}

/** To boolean cast */
PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue1) {


	RETURN_BOOL((zend_bool) 1);

}

PHP_METHOD(Test_Cast, testBooleanCastFromIntTrue2) {


	RETURN_BOOL((zend_bool) 1000);

}

PHP_METHOD(Test_Cast, testBooleanCastFromIntFalse) {


	RETURN_BOOL((zend_bool) 0);

}

PHP_METHOD(Test_Cast, testBooleanCastFromObject) {

	zval *simpleObject;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(simpleObject);
	object_init(simpleObject);
	RETURN_MM_BOOL(zephir_get_boolval(simpleObject));

}

PHP_METHOD(Test_Cast, testBooleanCastFromEmptyArray) {

	zval *a;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	RETURN_MM_BOOL(zephir_get_boolval(a));

}

PHP_METHOD(Test_Cast, testBooleanCastFromArray) {

	zval *a, *_0;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	array_init(a);

	ZEPHIR_INIT_VAR(a);
	array_init_size(a, 7);
	ZEPHIR_INIT_VAR(_0);
	ZVAL_LONG(_0, 1);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 2);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 3);
	zephir_array_fast_append(a, _0);
	ZEPHIR_INIT_BNVAR(_0);
	ZVAL_LONG(_0, 4);
	zephir_array_fast_append(a, _0);
	RETURN_MM_BOOL(zephir_get_boolval(a));

}

PHP_METHOD(Test_Cast, testBooleanCaseFromNull) {

	zval *a = NULL;

	ZEPHIR_MM_GROW();
	ZEPHIR_INIT_VAR(a);
	ZVAL_NULL(a);

	RETURN_MM_BOOL(zephir_get_boolval(a));

}

