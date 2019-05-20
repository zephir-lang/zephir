
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


ZEPHIR_INIT_CLASS(Test_Requires) {

	ZEPHIR_REGISTER_CLASS(Test, Requires, test, requires, test_requires_method_entry, 0);

	zend_declare_property_null(test_requires_ce, SL("content"), ZEND_ACC_PROTECTED TSRMLS_CC);

	return SUCCESS;

}

PHP_METHOD(Test_Requires, requireExternal1) {

	zval *path, path_sub, _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params(1, 1, 0, &path);



	if (zephir_require_zval_ret(&_0, path TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	ZEPHIR_MM_ADD_ENTRY(&_0);
	RETURN_MM_CTOR(&_0);

}

PHP_METHOD(Test_Requires, requireExternal2) {

	zval *path, path_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&path_sub);

	zephir_fetch_params(1, 1, 0, &path);



	if (zephir_require_zval(path TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	RETURN_MM_BOOL(1);

}

PHP_METHOD(Test_Requires, requireExternal3) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *path, path_sub, external3, _0;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&path_sub);
	ZVAL_UNDEF(&external3);
	ZVAL_UNDEF(&_0);

	zephir_fetch_params(1, 1, 0, &path);



	zephir_create_symbol_table(&zephir_memory);
	
	object_init_ex(&external3, test_requires_external3_ce);
	if (zephir_has_constructor(&external3 TSRMLS_CC)) {
		ZEPHIR_CALL_METHOD(NULL, &external3, "__construct", NULL, 0);
		zephir_check_call_status();
	}
	ZEPHIR_CALL_METHOD(NULL, &external3, "req", NULL, 63, path, this_ptr);
	zephir_check_call_status();
	RETURN_MM_MEMBER(getThis(), "content");

}

PHP_METHOD(Test_Requires, setContent) {

	zval *content, content_sub;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&content_sub);

	zephir_fetch_params(1, 1, 0, &content);



	zephir_update_property_zval(this_ptr, SL("content"), content);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Requires, renderTemplate) {

	zend_string *_4$$3;
	zend_ulong _3$$3;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *templatePath_param = NULL, *params, params_sub, _0, key, value, _7, *_1$$3, _2$$3, _5$$4, _6$$5;
	zval templatePath;
	zval *this_ptr = getThis();

	ZEPHIR_MM_GROW();
	ZVAL_UNDEF(&templatePath);
	ZVAL_UNDEF(&params_sub);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&value);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_2$$3);
	ZVAL_UNDEF(&_5$$4);
	ZVAL_UNDEF(&_6$$5);

	zephir_fetch_params(1, 2, 0, &templatePath_param, &params);

	if (UNEXPECTED(Z_TYPE_P(templatePath_param) != IS_STRING && Z_TYPE_P(templatePath_param) != IS_NULL)) {
		zephir_throw_exception_string(spl_ce_InvalidArgumentException, SL("Parameter 'templatePath' must be of the type string") TSRMLS_CC);
		RETURN_MM_NULL();
	}
	if (EXPECTED(Z_TYPE_P(templatePath_param) == IS_STRING)) {
		zephir_get_strval(&templatePath, templatePath_param);
	} else {
		ZEPHIR_MM_ZVAL_EMPTY_STRING(&templatePath);
	}


	zephir_create_symbol_table(&zephir_memory);
	
	if (Z_TYPE_P(params) == IS_ARRAY) {
		zephir_is_iterable(params, 0, "test/requires.zep", 47);
		if (Z_TYPE_P(params) == IS_ARRAY) {
			ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(params), _3$$3, _4$$3, _1$$3)
			{
				if (_4$$3 != NULL) { 
					ZVAL_STR_COPY(&key, _4$$3);
				} else {
					ZVAL_LONG(&key, _3$$3);
				}
				ZEPHIR_CPY_WRT(&value, _1$$3);
				ZEPHIR_CPY_WRT(&_5$$4, &value);
				if (zephir_set_symbol(&key, &_5$$4 TSRMLS_CC) == FAILURE) {
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
					if (zephir_set_symbol(&key, &_6$$5 TSRMLS_CC) == FAILURE) {
						return;
					}
				ZEPHIR_CALL_METHOD(NULL, params, "next", NULL, 0);
				zephir_check_call_status();
			}
		}
	}
	if (zephir_require_zval_ret(&_7, &templatePath TSRMLS_CC) == FAILURE) {
		RETURN_MM_NULL();
	}
	ZEPHIR_MM_ADD_ENTRY(&_7);
	RETURN_MM_CTOR(&_7);

}

