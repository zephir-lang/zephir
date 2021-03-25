
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


ZEPHIR_INIT_CLASS(Stub_Globals) {

	ZEPHIR_REGISTER_CLASS(Stub, Globals, stub, globals, stub_globals_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Stub_Globals, setBoolValueUsingDotNotation) {

	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &value);



	ZEPHIR_GLOBAL(db).my_setting_1 = zend_is_true(value);

}

PHP_METHOD(Stub_Globals, setIntValueUsingDotNotation) {

	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &value);



	ZEPHIR_GLOBAL(db).my_setting_2 = Z_LVAL_P(value);

}

PHP_METHOD(Stub_Globals, setCharValue) {

	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &value);



	ZEPHIR_GLOBAL(my_setting_4) = (Z_TYPE_P(value) == IS_STRING ? (Z_STRLEN_P(value) ? Z_STRVAL_P(value)[0] : NULL) : Z_LVAL_P(value));

}

PHP_METHOD(Stub_Globals, setBoolValue) {

	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &value);



	ZEPHIR_GLOBAL(my_setting_1) = zend_is_true(value);

}

PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmCacheLevel) {

	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
#if PHP_VERSION_ID >= 80000
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();

#endif


	zephir_fetch_params_without_memory_grow(1, 0, &value);



	ZEPHIR_GLOBAL(orm).cache_level = Z_LVAL_P(value);

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals1) {

	zval *this_ptr = getThis();



	RETURN_BOOL(ZEPHIR_GLOBAL(db).my_setting_1);

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals2) {

	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(db).my_setting_2);

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals3) {

	zval *this_ptr = getThis();



	RETURN_DOUBLE(ZEPHIR_GLOBAL(db).my_setting_3);

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals4) {

	zval *this_ptr = getThis();



	RETURN_BOOL(ZEPHIR_GLOBAL(my_setting_1));

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals5) {

	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(my_setting_2));

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals6) {

	zval *this_ptr = getThis();



	RETURN_DOUBLE(ZEPHIR_GLOBAL(my_setting_3));

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals7) {

	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(my_setting_4));

}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCacheLevel) {

	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(orm).cache_level);

}

