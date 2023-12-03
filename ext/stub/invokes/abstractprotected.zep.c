
#ifdef HAVE_CONFIG_H
#include "../../ext_config.h"
#endif

#include <php.h>
#include "../../php_ext.h"
#include "../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/concat.h"
#include "kernel/memory.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Invokes_AbstractProtected)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Invokes, AbstractProtected, stub, invokes_abstractprotected, stub_invokes_abstractprotected_method_entry, ZEND_ACC_EXPLICIT_ABSTRACT_CLASS);

	zend_declare_property_null(stub_invokes_abstractprotected_ce, SL("text"), ZEND_ACC_PROTECTED);
	return SUCCESS;
}

PHP_METHOD(Stub_Invokes_AbstractProtected, protectedMethod1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *text_param = NULL, ret;
	zval text, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&ret);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(text)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &text_param);
	zephir_get_strval(&text, text_param);


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_VS(&_0, &text, "1");
	ZEPHIR_CPY_WRT(&ret, &_0);
	RETURN_CCTOR(&ret);
}

PHP_METHOD(Stub_Invokes_AbstractProtected, protectedMethod2)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *text_param = NULL, ret;
	zval text, _0;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&ret);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(text)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &text_param);
	zephir_get_strval(&text, text_param);


	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_VS(&_0, &text, "2");
	ZEPHIR_CPY_WRT(&ret, &_0);
	RETURN_CCTOR(&ret);
}

PHP_METHOD(Stub_Invokes_AbstractProtected, renderArrayElements)
{
	zval _0, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *text_param = NULL, _1;
	zval text;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(text)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &text_param);
	zephir_get_strval(&text, text_param);


	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	zephir_array_fast_append(&_0, this_ptr);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "protectedMethod2");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	zephir_array_fast_append(&_2, &text);
	ZEPHIR_CALL_USER_FUNC_ARRAY(return_value, &_0, &_2);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Invokes_AbstractProtected, renderArrayElementsComplex)
{
	zval elements, _0, _2, _6$$3, _10$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *text_param = NULL, item, _1, *_3, _4, _5$$3, _7$$3, _8$$3, _9$$4, _11$$4, _12$$4;
	zval text, result;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&text);
	ZVAL_UNDEF(&result);
	ZVAL_UNDEF(&item);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_4);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_7$$3);
	ZVAL_UNDEF(&_8$$3);
	ZVAL_UNDEF(&_9$$4);
	ZVAL_UNDEF(&_11$$4);
	ZVAL_UNDEF(&_12$$4);
	ZVAL_UNDEF(&elements);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_6$$3);
	ZVAL_UNDEF(&_10$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(text)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &text_param);
	zephir_get_strval(&text, text_param);


	ZEPHIR_INIT_VAR(&result);
	ZEPHIR_INIT_VAR(&elements);
	zephir_create_array(&elements, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, "protectedMethod1");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_VAR(&_2);
	zephir_create_array(&_2, 1, 0);
	zephir_array_fast_append(&_2, &text);
	zephir_array_fast_append(&_0, &_2);
	zephir_array_fast_append(&elements, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	zephir_create_array(&_0, 2, 0);
	ZEPHIR_INIT_NVAR(&_1);
	ZVAL_STRING(&_1, "protectedMethod2");
	zephir_array_fast_append(&_0, &_1);
	ZEPHIR_INIT_NVAR(&_2);
	zephir_create_array(&_2, 1, 0);
	zephir_array_fast_append(&_2, &text);
	zephir_array_fast_append(&_0, &_2);
	zephir_array_fast_append(&elements, &_0);
	zephir_is_iterable(&elements, 0, "stub/invokes/abstractprotected.zep", 49);
	if (Z_TYPE_P(&elements) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(&elements), _3)
		{
			ZEPHIR_INIT_NVAR(&item);
			ZVAL_COPY(&item, _3);
			ZEPHIR_INIT_NVAR(&_5$$3);
			ZEPHIR_INIT_NVAR(&_6$$3);
			zephir_create_array(&_6$$3, 2, 0);
			zephir_array_fast_append(&_6$$3, this_ptr);
			ZEPHIR_OBS_NVAR(&_7$$3);
			zephir_array_fetch_long(&_7$$3, &item, 0, PH_NOISY, "stub/invokes/abstractprotected.zep", 46);
			zephir_array_fast_append(&_6$$3, &_7$$3);
			zephir_array_fetch_long(&_8$$3, &item, 1, PH_NOISY | PH_READONLY, "stub/invokes/abstractprotected.zep", 46);
			ZEPHIR_CALL_USER_FUNC_ARRAY(&_5$$3, &_6$$3, &_8$$3);
			zephir_check_call_status();
			zephir_concat_self(&result, &_5$$3);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &elements, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_4, &elements, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_4)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&item, &elements, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_INIT_NVAR(&_9$$4);
				ZEPHIR_INIT_NVAR(&_10$$4);
				zephir_create_array(&_10$$4, 2, 0);
				zephir_array_fast_append(&_10$$4, this_ptr);
				ZEPHIR_OBS_NVAR(&_11$$4);
				zephir_array_fetch_long(&_11$$4, &item, 0, PH_NOISY, "stub/invokes/abstractprotected.zep", 46);
				zephir_array_fast_append(&_10$$4, &_11$$4);
				zephir_array_fetch_long(&_12$$4, &item, 1, PH_NOISY | PH_READONLY, "stub/invokes/abstractprotected.zep", 46);
				ZEPHIR_CALL_USER_FUNC_ARRAY(&_9$$4, &_10$$4, &_12$$4);
				zephir_check_call_status();
				zephir_concat_self(&result, &_9$$4);
			ZEPHIR_CALL_METHOD(NULL, &elements, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&item);
	RETURN_CTOR(&result);
}

