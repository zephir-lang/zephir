
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
#include "ext/spl/spl_exceptions.h"
#include "kernel/exception.h"
#include "kernel/operators.h"


ZEPHIR_INIT_CLASS(Stub_Requires) {

	ZEPHIR_REGISTER_CLASS(Stub, Requires, stub, requires, stub_requires_method_entry, 0);

	zend_declare_property_null(stub_requires_ce, SL("content"), ZEND_ACC_PROTECTED);

	return SUCCESS;

}

PHP_METHOD(Stub_Requires, requireExternal1) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *path, path_sub, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(&_0);
	if (zephir_require_zval_ret(&_0, path) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(&_0);

}

PHP_METHOD(Stub_Requires, requireExternal2) {

	zval *path, path_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &path);



	if (zephir_require_zval(path) == FAILURE) {
		RETURN_NULL();
	}
	RETURN_BOOL(1);

}

PHP_METHOD(Stub_Requires, requireExternal3) {

	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *path, path_sub, external3, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&external3);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &path);



	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CREATE_SYMBOL_TABLE();
	
	ZEPHIR_INIT_VAR(&external3);
	object_init_ex(&external3, stub_requires_external3_ce);
	if (zephir_has_constructor(&external3)) {
		ZEPHIR_CALL_METHOD(NULL, &external3, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZEPHIR_CALL_METHOD(NULL, &external3, "req", NULL, 79, path, this_ptr);
	zephir_check_call_status();
	RETURN_MM_MEMBER(getThis(), "content");

}

PHP_METHOD(Stub_Requires, setContent) {

	zval *content, content_sub;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&content_sub);

	zephir_fetch_params_without_memory_grow(1, 0, &content);



	zephir_update_property_zval(this_ptr, ZEND_STRL("content"), content);

}

PHP_METHOD(Stub_Requires, renderTemplate) {

	zend_string *_4$$3;
	zend_ulong _3$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *templatePath_param = NULL, *params, params_sub, _0, key, value, _7, *_1$$3, _2$$3, _5$$4, _6$$5;
	zval templatePath;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&templatePath);
	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_5$$4);
	ZVAL_UNDEF(&_6$$5);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &templatePath_param, &params);

	if (UNEXPECTED(Z_TYPE_P(templatePath_param) != IS_STRING && Z_TYPE_P(templatePath_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'templatePath' must be of the type string"));
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(templatePath_param) == IS_STRING)) {
		zephir_get_strval(&templatePath, templatePath_param);
	} else {
		ZEPHIR_INIT_VAR(&templatePath);
		ZVAL_EMPTY_STRING(&templatePath);
	}


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CREATE_SYMBOL_TABLE();
	
	if (Z_TYPE_P(params) == IS_ARRAY) {
		zephir_is_iterable(params, 0, "stub/requires.zep", 47);
		if (Z_TYPE_P(params) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(params), _3$$3, _4$$3, _1$$3)
			{
				ZEPHIR_INIT_NVAR(&key);
				if (_4$$3 != NULL) { 
					ZVAL_STR_COPY(&key, _4$$3);
				} else {
					ZVAL_LONG(&key, _3$$3);
				}
				ZEPHIR_INIT_NVAR(&value);
				ZVAL_COPY(&value, _1$$3);
				ZEPHIR_CPY_WRT(&_5$$4, &value);
				if (zephir_set_symbol(&key, &_5$$4) == FAILURE) {
					return;
				}
			} ZEND_HASH_FOREACH_END();
		} else {
			ZEPHIR_CALL_METHOD(NULL, params, "rewind", NULL, 0);
			zephir_check_call_status();
			while (1) {
				ZEPHIR_CALL_METHOD(&_2$$3, params, "valid", NULL, 0);
				zephir_check_call_status();
				if (!zend_is_true(&_2$$3)) {
					break;
				}
				ZEPHIR_CALL_METHOD(&key, params, "key", NULL, 0);
				zephir_check_call_status();
				ZEPHIR_CALL_METHOD(&value, params, "current", NULL, 0);
				zephir_check_call_status();
					ZEPHIR_CPY_WRT(&_6$$5, &value);
					if (zephir_set_symbol(&key, &_6$$5) == FAILURE) {
						return;
					}
				ZEPHIR_CALL_METHOD(NULL, params, "next", NULL, 0);
				zephir_check_call_status();
			}
		}
		ZEPHIR_INIT_NVAR(&value);
		ZEPHIR_INIT_NVAR(&key);
	}
	ZEPHIR_OBSERVE_OR_NULLIFY_PPZV(&_7);
	if (zephir_require_zval_ret(&_7, &templatePath) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_CCTOR(&_7);

}

