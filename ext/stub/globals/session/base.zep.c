
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
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_Globals_Session_Base)
{
	ZEPHIR_REGISTER_CLASS(Stub\\Globals\\Session, Base, stub, globals_session_base, stub_globals_session_base_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Globals_Session_Base, set)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index_param = NULL, *value, value_sub, _SESSION;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_SESSION);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_get_global(&_SESSION, SL("_SESSION"));
	zephir_fetch_params(1, 2, 0, &index_param, &value);
	zephir_get_strval(&index, index_param);


	zephir_array_update_zval(&_SESSION, &index, value, PH_COPY | PH_SEPARATE);
	RETVAL_ZVAL(&_SESSION, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Globals_Session_Base, remove)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *index_param = NULL, _SESSION;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_UNDEF(&_SESSION);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(index)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_get_global(&_SESSION, SL("_SESSION"));
	zephir_fetch_params(1, 1, 0, &index_param);
	zephir_get_strval(&index, index_param);


	zephir_array_unset(&_SESSION, &index, PH_SEPARATE);
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Globals_Session_Base, __set)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *index_param = NULL, *value, value_sub;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(index)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &index_param, &value);
	zephir_get_strval(&index, index_param);


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "set", NULL, 0, &index, value);
	zephir_check_call_status();
	RETURN_MM();
}

PHP_METHOD(Stub_Globals_Session_Base, __unset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *index_param = NULL;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(index)
	ZEND_PARSE_PARAMETERS_END();


	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);
	zephir_get_strval(&index, index_param);


	ZEPHIR_CALL_METHOD(NULL, this_ptr, "remove", NULL, 0, &index);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();
}

PHP_METHOD(Stub_Globals_Session_Base, removeSessionData)
{
	zend_string *_4;
	zend_ulong _3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval _SESSION, key, _0, *_1, _2;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_SESSION);
	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);


	ZEPHIR_MM_GROW();
	zephir_get_global(&_SESSION, SL("_SESSION"));

	ZEPHIR_INIT_VAR(&_0);
	zephir_is_iterable(&_SESSION, 1, "stub/globals/session/base.zep", 36);
	if (Z_TYPE_P(&_SESSION) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&_SESSION), _3, _4, _1)
		{
			ZEPHIR_INIT_NVAR(&key);
			if (_4 != NULL) { 
				ZVAL_STR_COPY(&key, _4);
			} else {
				ZVAL_LONG(&key, _3);
			}
			ZEPHIR_INIT_NVAR(&_0);
			ZVAL_COPY(&_0, _1);
			zephir_array_unset(&_SESSION, &key, PH_SEPARATE);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &_SESSION, "rewind", NULL, 0);
		zephir_check_call_status();
		while (1) {
			ZEPHIR_CALL_METHOD(&_2, &_SESSION, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_2)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&key, &_SESSION, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&_0, &_SESSION, "current", NULL, 0);
			zephir_check_call_status();
				zephir_array_unset(&_SESSION, &key, PH_SEPARATE);
			ZEPHIR_CALL_METHOD(NULL, &_SESSION, "next", NULL, 0);
			zephir_check_call_status();
		}
	}
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_INIT_NVAR(&key);
	RETVAL_ZVAL(&_SESSION, 1, 0);
	RETURN_MM();
}

