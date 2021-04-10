
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


ZEPHIR_INIT_CLASS(Stub_ArrayAccessTest)
{
	ZEPHIR_REGISTER_CLASS(Stub, ArrayAccessTest, stub, arrayaccesstest, stub_arrayaccesstest_method_entry, 0);

	zend_declare_property_null(stub_arrayaccesstest_ce, SL("data"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_ArrayAccessTest, exits)
{
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

PHP_METHOD(Stub_ArrayAccessTest, get)
{
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
	zephir_array_fetch_string(&_0, &arr, SL("two"), PH_NOISY | PH_READONLY, "stub/arrayaccesstest.zep", 20);
	RETURN_CTOR(&_0);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/645
 */
PHP_METHOD(Stub_ArrayAccessTest, issue645)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval(this_ptr, ZEND_STRL("data"), &_0);
	ZEPHIR_OBS_VAR(&_1);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("data"), PH_NOISY_CC);
	RETURN_CCTOR(&_1);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1155
 */
PHP_METHOD(Stub_ArrayAccessTest, issue1155)
{
	zval s, _0;
	zval arr;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&arr);
	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&_0);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&arr);
	zephir_create_array(&arr, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "0");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "false");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "");
	zephir_array_fast_append(&arr, &_0);
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "true");
	RETURN_MM_BOOL(zephir_fast_in_array(&s, &arr));
}

