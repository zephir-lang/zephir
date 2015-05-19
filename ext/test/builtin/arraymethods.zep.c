
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
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/string.h"
#include "kernel/fcall.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Test_BuiltIn_ArrayMethods) {

	ZEPHIR_REGISTER_CLASS(Test\\BuiltIn, ArrayMethods, test, builtin_arraymethods, test_builtin_arraymethods_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_BuiltIn_ArrayMethods, getJoin1) {

	zval *_1 = NULL;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	zephir_fast_join_str(_1, SL("-"), _0 TSRMLS_CC);
	RETURN_CCTOR(_1);

}

PHP_METHOD(Test_BuiltIn_ArrayMethods, getReversed1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1 = NULL, *_2 = NULL;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_CALL_FUNCTION(&_2, "array_reverse", NULL, 2, _0);
	zephir_check_call_status();
	RETURN_CCTOR(_2);

}

PHP_METHOD(Test_BuiltIn_ArrayMethods, getMap1) {

	int ZEPHIR_LAST_CALL_STATUS;
	zval *_1 = NULL, *_2 = NULL;
	zval *_0;

	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(_0);
	zephir_create_array(_0, 3, 0 TSRMLS_CC);
	ZEPHIR_INIT_VAR(_1);
	ZVAL_LONG(_1, 1);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 2);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZVAL_LONG(_1, 3);
	zephir_array_fast_append(_0, _1);
	ZEPHIR_INIT_NVAR(_1);
	ZEPHIR_INIT_NVAR(_1);
	zephir_create_closure_ex(_1, NULL, test_0__closure_ce, SS("__invoke") TSRMLS_CC);
	ZEPHIR_CALL_FUNCTION(&_2, "array_map", NULL, 3, _1, _0);
	zephir_check_call_status();
	RETURN_CCTOR(_2);

}

