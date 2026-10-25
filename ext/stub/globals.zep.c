
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


ZEPHIR_INIT_CLASS(Stub_Globals)
{
	ZEPHIR_REGISTER_CLASS(Stub, Globals, stub, globals, stub_globals_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Globals, setBoolValueUsingDotNotation)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	ZEPHIR_GLOBAL(db).my_setting_1 = zend_is_true(value);
}

PHP_METHOD(Stub_Globals, setIntValueUsingDotNotation)
{
	zend_long _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = zval_get_long(value);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	ZEPHIR_GLOBAL(db).my_setting_2 = _0;
}

PHP_METHOD(Stub_Globals, setCharValue)
{
	char _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = (Z_TYPE_P(value) == IS_STRING ? (Z_STRLEN_P(value) ? Z_STRVAL_P(value)[0] : (char)0) : (char) zval_get_long(value));
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	ZEPHIR_GLOBAL(my_setting_4) = _0;
}

PHP_METHOD(Stub_Globals, setStringValue)
{
	zval value_zv;
	zend_string *value = NULL;

	ZVAL_UNDEF(&value_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&value_zv, value);
	zephir_globals_set_string(&ZEPHIR_GLOBAL(my_setting_5), &ZEPHIR_GLOBAL(zephir_owned_my_setting_5), &value_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

PHP_METHOD(Stub_Globals, setBoolValue)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	ZEPHIR_GLOBAL(my_setting_1) = zend_is_true(value);
}

PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmCacheLevel)
{
	zend_long _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = zval_get_long(value);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	ZEPHIR_GLOBAL(orm).cache_level = _0;
}

PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmCachePrefix)
{
	zval value_zv;
	zend_string *value = NULL;

	ZVAL_UNDEF(&value_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(value)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&value_zv, value);
	zephir_globals_set_string(&ZEPHIR_GLOBAL(orm).cache_prefix, &ZEPHIR_GLOBAL(orm).zephir_owned_cache_prefix, &value_zv);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals1)
{

	RETURN_BOOL(ZEPHIR_GLOBAL(db).my_setting_1);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals2)
{

	RETURN_LONG(ZEPHIR_GLOBAL(db).my_setting_2);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals3)
{

	RETURN_DOUBLE(ZEPHIR_GLOBAL(db).my_setting_3);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals4)
{

	RETURN_BOOL(ZEPHIR_GLOBAL(my_setting_1));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals5)
{

	RETURN_LONG(ZEPHIR_GLOBAL(my_setting_2));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals6)
{

	RETURN_DOUBLE(ZEPHIR_GLOBAL(my_setting_3));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals7)
{

	RETURN_LONG((unsigned char) (ZEPHIR_GLOBAL(my_setting_4)));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobals8)
{

	ZVAL_STRING(return_value, ZEPHIR_GLOBAL(my_setting_5));
	return;
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCacheLevel)
{

	RETURN_LONG(ZEPHIR_GLOBAL(orm).cache_level);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCachePrefix)
{

	ZVAL_STRING(return_value, ZEPHIR_GLOBAL(orm).cache_prefix);
	return;
}

PHP_METHOD(Stub_Globals, setUIntValue)
{
	zend_long _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = zval_get_long(value);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	ZEPHIR_GLOBAL(my_setting_6) = _0;
}

PHP_METHOD(Stub_Globals, setIntValue)
{
	zend_long _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = zval_get_long(value);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	ZEPHIR_GLOBAL(my_setting_2) = _0;
}

PHP_METHOD(Stub_Globals, setDoubleValue)
{
	double _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = zval_get_double(value);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
	ZEPHIR_GLOBAL(my_setting_3) = _0;
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getUIntValue)
{

	RETURN_LONG(ZEPHIR_GLOBAL(my_setting_6));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getTestSetting1)
{

	RETURN_BOOL(ZEPHIR_GLOBAL(test_setting_1));
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmCacheEnable)
{

	RETURN_BOOL(ZEPHIR_GLOBAL(orm).cache_enable);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getTestIniVariable)
{

	RETURN_BOOL(ZEPHIR_GLOBAL(extension).test_ini_variable);
}

/**
 * @return mixed
 */
PHP_METHOD(Stub_Globals, getModuleSetting)
{

	RETURN_BOOL(ZEPHIR_GLOBAL(module_setting));
}

PHP_METHOD(Stub_Globals, setModuleSetting)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	ZEPHIR_GLOBAL(module_setting) = zend_is_true(value);
}

PHP_METHOD(Stub_Globals, setHashValue)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_globals_set_hash(&ZEPHIR_GLOBAL(my_setting_7), value);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

/**
 * @return array|null
 */
PHP_METHOD(Stub_Globals, getHashValue)
{

	zephir_globals_get_hash(return_value, ZEPHIR_GLOBAL(my_setting_7));
	return;
}

PHP_METHOD(Stub_Globals, setDefaultGlobalsOrmAstCache)
{
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	zephir_globals_set_hash(&ZEPHIR_GLOBAL(orm).ast_cache, value);
	if (UNEXPECTED(EG(exception))) {
		return;
	}
}

/**
 * @return array|null
 */
PHP_METHOD(Stub_Globals, getDefaultGlobalsOrmAstCache)
{

	zephir_globals_get_hash(return_value, ZEPHIR_GLOBAL(orm).ast_cache);
	return;
}

