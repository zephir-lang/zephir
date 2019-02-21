
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
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Test_Globals_Session_Base) {

	ZEPHIR_REGISTER_CLASS(Test\\Globals\\Session, Base, test, globals_session_base, test_globals_session_base_method_entry, 0);

	return SUCCESS;

}

PHP_METHOD(Test_Globals_Session_Base, set) {

	zval *index_param = NULL, *value, value_sub, _SESSION;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_UNDEF(&value_sub);
	ZVAL_UNDEF(&_SESSION);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_SESSION, SL("_SESSION"));
	zephir_fetch_params(1, 2, 0, &index_param, &value);

	zephir_get_strval(&index, index_param);


	zephir_array_update_zval(&_SESSION, &index, value, PH_COPY | PH_SEPARATE);
	RETVAL_ZVAL(&_SESSION, 1, 0);
	RETURN_MM();

}

PHP_METHOD(Test_Globals_Session_Base, remove) {

	zval *index_param = NULL, _SESSION;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_UNDEF(&_SESSION);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_SESSION, SL("_SESSION"));
	zephir_fetch_params(1, 1, 0, &index_param);

	zephir_get_strval(&index, index_param);


	zephir_array_unset(&_SESSION, &index, PH_SEPARATE);
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Globals_Session_Base, __set) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *index_param = NULL, *value, value_sub;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);
	ZVAL_UNDEF(&value_sub);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 2, 0, &index_param, &value);

	zephir_get_strval(&index, index_param);


	ZEPHIR_RETURN_CALL_METHOD(this_ptr, "set", NULL, 0, &index, value);
	zephir_check_call_status();
	RETURN_MM();

}

PHP_METHOD(Test_Globals_Session_Base, __unset) {

	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *index_param = NULL;
	zval index;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&index);

	ZEPHIR_MM_GROW();
	zephir_fetch_params(1, 1, 0, &index_param);

	zephir_get_strval(&index, index_param);


	ZEPHIR_CALL_METHOD(NULL, this_ptr, "remove", NULL, 0, &index);
	zephir_check_call_status();
	ZEPHIR_MM_RESTORE();

}

PHP_METHOD(Test_Globals_Session_Base, removeSessionData) {

	zend_string *_3;
	zend_ulong _2;
	zval _SESSION, key, _0, *_1;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&_SESSION);
	ZVAL_UNDEF(&key);
	ZVAL_UNDEF(&_0);

	ZEPHIR_MM_GROW();
	zephir_get_global(&_SESSION, SL("_SESSION"));

	ZEPHIR_INIT_VAR(&_0);
	zephir_is_iterable(&_SESSION, 1, "test/globals/session/base.zep", 36);
	ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&_SESSION), _2, _3, _1)
	{
		ZEPHIR_INIT_NVAR(&key);
		if (_3 != NULL) { 
			ZVAL_STR_COPY(&key, _3);
		} else {
			ZVAL_LONG(&key, _2);
		}
		ZEPHIR_INIT_NVAR(&_0);
		ZVAL_COPY(&_0, _1);
		zephir_array_unset(&_SESSION, &key, PH_SEPARATE);
	} ZEND_HASH_FOREACH_END();
	ZEPHIR_INIT_NVAR(&_0);
	ZEPHIR_INIT_NVAR(&key);
	RETVAL_ZVAL(&_SESSION, 1, 0);
	RETURN_MM();

}

