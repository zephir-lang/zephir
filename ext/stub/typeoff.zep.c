
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
#include "kernel/object.h"


/**
 * OO operations
 */
ZEPHIR_INIT_CLASS(Stub_Typeoff)
{
	ZEPHIR_REGISTER_CLASS(Stub, Typeoff, stub, typeoff, stub_typeoff_method_entry, 0);

	zend_declare_property_null(stub_typeoff_ce, SL("property"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Typeoff, testNativeStringFalse)
{
	zval testVar;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&testVar);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&testVar);
	ZVAL_STRING(&testVar, "sdfsdf");
	RETURN_MM_BOOL(1 == 0);
}

PHP_METHOD(Stub_Typeoff, testNativeStringTrue)
{
	zval testVar;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&testVar);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&testVar);
	ZVAL_STRING(&testVar, "sdfsdf");
	RETURN_MM_BOOL(1 == 1);
}

PHP_METHOD(Stub_Typeoff, testNativeIntFalse)
{
	zend_long testVar = 0;
	zval *this_ptr = getThis();




	testVar = 12345;
	RETURN_BOOL(1 == 0);
}

PHP_METHOD(Stub_Typeoff, testNativeIntTrue)
{
	zend_long testVar = 0;
	zval *this_ptr = getThis();




	testVar = 12345;
	RETURN_BOOL(1 == 1);
}

PHP_METHOD(Stub_Typeoff, testNativeDoubleTrue)
{
	double testVar = 0;
	zval *this_ptr = getThis();




	testVar = (double) (12345);
	RETURN_BOOL(1 == 1);
}

PHP_METHOD(Stub_Typeoff, testNativeBoolTrue)
{
	zend_bool testVar = 0;
	zval *this_ptr = getThis();




	testVar = 1;
	RETURN_BOOL(1 == 1);
}

PHP_METHOD(Stub_Typeoff, testNotBoolTrue)
{
	zval testVar;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&testVar);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&testVar);
	ZVAL_STRING(&testVar, "");
	ZEPHIR_INIT_NVAR(&testVar);
	ZVAL_BOOL(&testVar, 1);
	RETURN_MM_BOOL(((Z_TYPE_P(&testVar) == IS_TRUE || Z_TYPE_P(&testVar) == IS_FALSE) != 1));
}

PHP_METHOD(Stub_Typeoff, testNativeBoolFalse)
{
	zend_bool testVar = 0;
	zval *this_ptr = getThis();




	testVar = 0;
	RETURN_BOOL(1 == 0);
}

PHP_METHOD(Stub_Typeoff, testArrayFalse)
{
	zval testVar;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&testVar);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&testVar);
	array_init(&testVar);
	RETURN_MM_BOOL(Z_TYPE_P(&testVar) == IS_STRING);
}

PHP_METHOD(Stub_Typeoff, testArrayTrue)
{
	zval testVar;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&testVar);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&testVar);
	array_init(&testVar);
	RETURN_MM_BOOL(Z_TYPE_P(&testVar) == IS_ARRAY);
}

PHP_METHOD(Stub_Typeoff, testClassPropertyAccess)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);


	ZEPHIR_MM_GROW();

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_STRING(&_0, "test string");
	zephir_update_property_zval(this_ptr, ZEND_STRL("property"), &_0);
	zephir_memory_observe(&_1);
	zephir_read_property(&_1, this_ptr, ZEND_STRL("property"), PH_NOISY_CC);
	RETURN_MM_BOOL(Z_TYPE_P(&_1) == IS_STRING);
}

PHP_METHOD(Stub_Typeoff, testUnknownTypeOf)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *u, u_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&u_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(u)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &u);


	ZEPHIR_INIT_VAR(&_0);
	zephir_gettype(&_0, u);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Typeoff, testCallableTypeOf)
{
	zval *cb, cb_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&cb_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(cb)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &cb);


	RETURN_BOOL(zephir_is_callable(cb) == 1);
}

