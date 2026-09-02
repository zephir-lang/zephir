
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
#include "kernel/require.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/string.h"


ZEPHIR_INIT_CLASS(Stub_Requires)
{
	ZEPHIR_REGISTER_CLASS(Stub, Requires, stub, requires, stub_requires_method_entry, 0);

	zend_declare_property_null(stub_requires_ce, SL("content"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Requires, requireExternal1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *path, path_sub, _0;

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(path)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &path);
	ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(&_0);
	if (zephir_require_zval_ret(&_0, path) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Requires, requireExternal2)
{
	zval *path, path_sub;

	ZVAL_UNDEF(&path_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(path)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &path);
	if (zephir_require_zval(path) == FAILURE) {
		RETURN_NULL();
	}
	RETURN_BOOL(1);
}

PHP_METHOD(Stub_Requires, requireExternal3)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *path, path_sub, external3, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&external3);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(path)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &path);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CREATE_SYMBOL_TABLE();
	
	ZEPHIR_INIT_VAR(&external3);
	object_init_ex(&external3, stub_requires_external3_ce);
	if (zephir_has_constructor(&external3)) {
		ZEPHIR_CALL_METHOD(NULL, &external3, "__construct", NULL, 0);
		zephir_check_call_status();
	}

	ZEPHIR_CALL_METHOD(NULL, &external3, "req", NULL, 98, path, this_ptr);
	zephir_check_call_status();
	RETURN_MM_MEMBER(getThis(), "content");
}

PHP_METHOD(Stub_Requires, setContent)
{
	zval *content, content_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&content_sub);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("content", 7, 1);
	}

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(content)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &content);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 96, content);
}

PHP_METHOD(Stub_Requires, renderTemplate)
{
	zend_bool _8$$3;
	zend_ulong _4$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval templatePath_zv, *params, params_sub, _0, key, value, _10, *_1$$3, _2$$3, *_3$$3, _7$$3, _6$$4, _9$$5;
	zend_string *templatePath = NULL, *_5$$3;

	ZVAL_UNDEF(&templatePath_zv);
	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_10);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_6$$4);
	ZVAL_UNDEF(&_9$$5);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(templatePath)
		Z_PARAM_ZVAL(params)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	params = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&templatePath_zv);
	ZVAL_STR_COPY(&templatePath_zv, templatePath);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CREATE_SYMBOL_TABLE();
	
	if (Z_TYPE_P(params) == IS_ARRAY) {
		if (Z_TYPE_P(params) == IS_STRING) {
			ZEPHIR_INIT_VAR(&_2$$3);
			zephir_string_to_char_array(&_2$$3, params);
			_1$$3 = &_2$$3;
		} else {
			_1$$3 = params;
		}
		zephir_is_iterable(_1$$3, 0, "stub/requires.zep", 47);
		if (Z_TYPE_P(_1$$3) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_1$$3), _4$$3, _5$$3, _3$$3)
			{
				ZEPHIR_INIT_NVAR(&key);
				if (_5$$3 != NULL) { 
					ZVAL_STR_COPY(&key, _5$$3);
				} else {
					ZVAL_LONG(&key, _4$$3);
				}
				ZEPHIR_INIT_NVAR(&value);
				ZVAL_COPY(&value, _3$$3);
				ZEPHIR_CPY_WRT(&_6$$4, &value);
				if (zephir_set_symbol(&key, &_6$$4) == FAILURE) {
					return;
				}
			} ZEND_HASH_FOREACH_END();
		} else {
			ZEPHIR_CALL_METHOD(NULL, _1$$3, "rewind", NULL, 0);
			zephir_check_call_status();
			_8$$3 = 1;
			while (1) {
				if (_8$$3) {
					_8$$3 = 0;
				} else {
					ZEPHIR_CALL_METHOD(NULL, _1$$3, "next", NULL, 0);
					zephir_check_call_status();
				}
				ZEPHIR_CALL_METHOD(&_7$$3, _1$$3, "valid", NULL, 0);
				zephir_check_call_status();
				if (!zend_is_true(&_7$$3)) {
					break;
				}
				ZEPHIR_CALL_METHOD(&key, _1$$3, "key", NULL, 0);
				zephir_check_call_status();
				ZEPHIR_CALL_METHOD(&value, _1$$3, "current", NULL, 0);
				zephir_check_call_status();
					ZEPHIR_CPY_WRT(&_9$$5, &value);
					if (zephir_set_symbol(&key, &_9$$5) == FAILURE) {
						return;
					}
			}
		}
		ZEPHIR_INIT_NVAR(&value);
		ZEPHIR_INIT_NVAR(&key);
	}
	ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(&_10);
	if (zephir_require_zval_ret(&_10, &templatePath_zv) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(&_10);
}

PHP_METHOD(Stub_Requires, requireOnce)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval path_zv, _0;
	zend_string *path = NULL;

	ZVAL_UNDEF(&path_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(path)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&path_zv);
	ZVAL_STR_COPY(&path_zv, path);
	ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(&_0);
	if (zephir_require_once_zval_ret(&_0, &path_zv) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(&_0);
}

