
#ifdef HAVE_CONFIG_H
#include "../../../ext_config.h"
#endif

#include <php.h>
#include "../../../php_ext.h"
#include "../../../ext.h"

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/array.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"
#include "kernel/string.h"


ZEPHIR_INIT_CLASS(Stub_Globals_Session_Base)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Globals\\Session, Base, stub, globals_session_base, stub_globals_session_base_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Globals_Session_Base, set)
{
	zval index_zv, *value, value_sub, _SESSION;
	zend_string *index = NULL;

	ZVAL_UNDEF(&index_zv);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_SESSION);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_get_global(&_SESSION, SL("_SESSION"));
	value = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&index_zv, index);
	zephir_array_update_zval(&_SESSION, &index_zv, value, PH_COPY | PH_SEPARATE);
	RETVAL_ZVAL(&_SESSION, 1, 0);
	return;
}

PHP_METHOD(Stub_Globals_Session_Base, remove)
{
	zval index_zv, _SESSION;
	zend_string *index = NULL;

	ZVAL_UNDEF(&index_zv);
	ZVAL_UNDEF(&_SESSION);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(index)
	ZEND_PARSE_PARAMETERS_END();
	zephir_get_global(&_SESSION, SL("_SESSION"));
	ZVAL_STR(&index_zv, index);
	zephir_array_unset(&_SESSION, &index_zv, PH_SEPARATE);
}

PHP_METHOD(Stub_Globals_Session_Base, __set)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval index_zv, *value, value_sub;
	zend_string *index = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index_zv);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	value = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&index_zv);
	ZVAL_STR_COPY(&index_zv, index);
	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "set", NULL, 0, &index_zv, value);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Globals_Session_Base, __unset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval index_zv;
	zend_string *index = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(index)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&index_zv);
	ZVAL_STR_COPY(&index_zv, index);
	ZEPHIR_CALL_METHOD(NULL, this_ptr, "remove", NULL, 0, &index_zv);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Globals_Session_Base, removeSessionData)
{
	zend_string *_5;
	zend_ulong _4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval _SESSION, key, *_0, _1, _2, *_3;

	ZVAL_UNDEF(&_SESSION);
	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_2);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_get_global(&_SESSION, SL("_SESSION"));

	if (Z_TYPE_P(&_SESSION) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, &_SESSION);
		_0 = &_1;
	} else {
		_0 = &_SESSION;
	}
	ZEPHIR_INIT_VAR(&_2);
	zephir_is_iterable(_0, 0, "stub/globals/session/base.zep", 36);
	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_0), _4, _5, _3)
	{
		ZEPHIR_INIT_NVAR(&key);
		if (_5 != NULL) { 
			ZVAL_STR_COPY(&key, _5);
		} else {
			ZVAL_LONG(&key, _4);
		}
		ZEPHIR_INIT_NVAR(&_2);
		ZVAL_COPY(&_2, _3);
		zephir_array_unset(&_SESSION, &key, PH_SEPARATE);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&_2);
	ZEPHIR_INIT_NVAR(&key);
	RETVAL_ZVAL(&_SESSION, 1, 0);
	RETURN_MM();
}

