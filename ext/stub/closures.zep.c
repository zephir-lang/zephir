
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
#include "kernel/object.h"
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/memory.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Closures)
{
	ZEPHIR_REGISTER_CLASS(Stub, Closures, stub, closures, stub_closures_method_entry, 0);

	zend_declare_property_null(stub_closures_ce, SL("_argument"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_closures_ce, SL("_function"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Closures, simple1)
{

	zephir_create_closure_ex(return_value, NULL, stub_1__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple2)
{

	zephir_create_closure_ex(return_value, NULL, stub_2__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple3)
{

	zephir_create_closure_ex(return_value, NULL, stub_3__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple4)
{

	zephir_create_closure_ex(return_value, NULL, stub_4__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, simple5)
{

	zephir_create_closure_ex(return_value, NULL, stub_5__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, arrow1)
{

	zephir_create_closure_ex(return_value, NULL, stub_6__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, arrow2)
{

	zephir_create_closure_ex(return_value, NULL, stub_7__closure_ce, SL("__invoke"));
	return;
}

PHP_METHOD(Stub_Closures, testUseCommand)
{
	zval _0;
	zend_long abc;

	ZVAL_UNDEF(&_0);
	abc = 1;
	zephir_create_closure_ex(return_value, NULL, stub_8__closure_ce, SL("__invoke"));
	ZVAL_UNDEF(&_0);
	ZVAL_LONG(&_0, abc);
	zephir_update_static_property_ce(stub_8__closure_ce, ZEND_STRL("abc"), &_0);
	return;
}

PHP_METHOD(Stub_Closures, issue1860)
{
	zval *abc_param = NULL;
	zval abc;

	ZVAL_UNDEF(&abc);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ARRAY(abc)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params(1, 1, 0, &abc_param);
	ZVAL_COPY(&abc, abc_param);
	zephir_create_closure_ex(return_value, NULL, stub_9__closure_ce, SL("__invoke"));
	zephir_update_static_property_ce(stub_9__closure_ce, ZEND_STRL("abc"), &abc);
	return;
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/642
 */
PHP_METHOD(Stub_Closures, issue642)
{
	zval _0;
	zval obj, closure;
	zend_long ZEPHIR_LAST_CALL_STATUS;

	ZVAL_UNDEF(&obj);
	ZVAL_UNDEF(&closure);
	ZVAL_UNDEF(&_0);
	ZVAL_NULL(&obj);
	object_init(&obj);
	ZVAL_NULL(&closure);
	zephir_create_closure_ex(&closure, NULL, stub_10__closure_ce, SL("__invoke"));
	ZVAL_NULL(&_0);
	zephir_create_array(&_0, 1, 0);
	zephir_array_fast_append(&_0, &obj);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &closure, &_0);
	zephir_check_call_status();
	return;
}

PHP_METHOD(Stub_Closures, issue1036SetArgument)
{
	zval *argument, argument_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&argument_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(argument)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &argument);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_argument"), argument);
}

PHP_METHOD(Stub_Closures, issue1036SetFunction)
{
	zval *func, func_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&func_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(func)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &func);
	zephir_update_property_zval(this_ptr, ZEND_STRL("_function"), func);
}

/**
 * @issue https://github.com/zephir-lang/zephir/issues/1036
 */
PHP_METHOD(Stub_Closures, issue1036Call)
{
	zval _0, _1;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	zephir_read_property(&_0, this_ptr, ZEND_STRL("_function"), PH_NOISY_CC | PH_READONLY);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("_argument"), PH_NOISY_CC | PH_READONLY);
	ZEPHIR_RETURN_CALL_FUNCTION("call_user_func", NULL, 30, &_0, &_1);
	zephir_check_call_status();
	return;
}

