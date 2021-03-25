
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
#include "kernel/fcall.h"
#include "kernel/array.h"
#include "kernel/object.h"


ZEPHIR_INIT_CLASS(Stub_ArrayAccessTest) {

	ZEPHIR_REGISTER_CLASS(Stub, ArrayAccessTest, stub, arrayaccesstest, stub_arrayaccesstest_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_ArrayAccessTest, exits) {

	zval arr;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&arr);
	object_init_ex(&arr, stub_arrayaccessobj_ce);
	ZEPHIR_CALL_METHOD(NULL, &arr, "__construct", NULL, 3);
	zephir_check_call_status();
	RETURN_MM_BOOL(zephir_array_isset_string(&arr, SL("one")));

}

PHP_METHOD(Stub_ArrayAccessTest, get) {

	zval arr, _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&arr);
	object_init_ex(&arr, stub_arrayaccessobj_ce);
	ZEPHIR_CALL_METHOD(NULL, &arr, "__construct", NULL, 3);
	zephir_check_call_status();
	zephir_array_fetch_string(&_0, &arr, SL("two"), PH_NOISY | PH_READONLY, "stub/arrayaccesstest.zep", 18);
	RETURN_CTOR(&_0);

}

