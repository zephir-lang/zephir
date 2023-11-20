
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
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Globals)
{
	ZEPHIR_REGISTER_CLASS(Stub, Globals, stub, globals, stub_globals_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Globals, setBoolValueUsingDotNotation)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &value);


	ZEPHIR_GLOBAL(db).my_setting_1 = zend_is_true(value);
}

PHP_METHOD(Stub_Globals, setIntValueUsingDotNotation)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &value);


	ZEPHIR_GLOBAL(db).my_setting_2 = zval_get_long(value);
}

PHP_METHOD(Stub_Globals, setCharValue)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &value);


	ZEPHIR_GLOBAL(my_setting_4) = (Z_TYPE_P(value) == IS_STRING ? (Z_STRLEN_P(value) ? Z_STRVAL_P(value)[0] : NULL) : zval_get_long(value));
}

PHP_METHOD(Stub_Globals, setStringValue)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value_param = NULL;
	zval value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value_param);
	zephir_get_strval(&value, value_param);


	ZEPHIR_GLOBAL(my_setting_5) = ZSTR_VAL(zval_get_string(&value));
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Globals, setBoolValue)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &value);


	ZEPHIR_GLOBAL(my_setting_1) = zend_is_true(value);
}

PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmCacheLevel)
{
	zval *value, value_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	zephir_fetch_params_without_memory_grow(1, 0, &value);


	ZEPHIR_GLOBAL(orm).cache_level = zval_get_long(value);
}

PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmCachePrefix)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *value_param = NULL;
	zval value;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&value);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &value_param);
	zephir_get_strval(&value, value_param);


	ZEPHIR_GLOBAL(orm).cache_prefix = ZSTR_VAL(zval_get_string(&value));
	ZEPHIR_MM_RESTORE();
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals1)
{
	zval *this_ptr = getThis();



	RETURN_BOOL(ZEPHIR_GLOBAL(db).my_setting_1);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals2)
{
	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(db).my_setting_2);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals3)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(ZEPHIR_GLOBAL(db).my_setting_3);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals4)
{
	zval *this_ptr = getThis();



	RETURN_BOOL(ZEPHIR_GLOBAL(my_setting_1));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals5)
{
	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(my_setting_2));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals6)
{
	zval *this_ptr = getThis();



	RETURN_DOUBLE(ZEPHIR_GLOBAL(my_setting_3));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals7)
{
	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(my_setting_4));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals8)
{
	zval *this_ptr = getThis();



	RETURN_STRING(ZEPHIR_GLOBAL(my_setting_5));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCacheLevel)
{
	zval *this_ptr = getThis();



	RETURN_LONG(ZEPHIR_GLOBAL(orm).cache_level);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCachePrefix)
{
	zval *this_ptr = getThis();



	RETURN_STRING(ZEPHIR_GLOBAL(orm).cache_prefix);
}

