
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
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/object.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/1170
 */
ZEPHIR_INIT_CLASS(Stub_UnreachableLoop)
{
	ZEPHIR_REGISTER_CLASS(Stub, UnreachableLoop, stub, unreachableloop, stub_unreachableloop_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_UnreachableLoop, toPropertyString)
{
	zend_string *_2;
	zend_ulong _1;
	zend_bool x = 0, _8;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *properties_param = NULL, ret, k, v, *_0, _7, _3$$3, _4$$5, _5$$5, _6$$6, _9$$7, _10$$9, _11$$9, _12$$10;
	zval properties;

	ZVAL_UNDEF(&properties);
	ZVAL_UNDEF(&ret);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&v);
	ZVAL_UNDEF(&_7);
	ZVAL_UNDEF(&_3$$3);
	ZVAL_UNDEF(&_4$$5);
	ZVAL_UNDEF(&_5$$5);
	ZVAL_UNDEF(&_6$$6);
	ZVAL_UNDEF(&_9$$7);
	ZVAL_UNDEF(&_10$$9);
	ZVAL_UNDEF(&_11$$9);
	ZVAL_UNDEF(&_12$$10);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		ZEPHIR_Z_PARAM_ARRAY(properties, properties_param)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &properties_param);
	zephir_get_arrval(&properties, properties_param);
	x = 0;
	ZEPHIR_INIT_VAR(&ret);
	ZVAL_STRING(&ret, "{");
	zephir_is_iterable(&properties, 0, "stub/unreachableloop.zep", 26);
	if (Z_TYPE_P(&properties) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(&properties), _1, _2, _0)
		{
			ZEPHIR_INIT_NVAR(&k);
			if (_2 != NULL) { 
				ZVAL_STR_COPY(&k, _2);
			} else {
				ZVAL_LONG(&k, _1);
			}
			ZEPHIR_INIT_NVAR(&v);
			ZVAL_COPY(&v, _0);
			if (x) {
				zephir_concat_self_str(&ret, SL(", "));
			}
			x = 1;
			ZEPHIR_INIT_NVAR(&_3$$3);
			ZEPHIR_CONCAT_VS(&_3$$3, &k, ": ");
			zephir_concat_self(&ret, &_3$$3);
			if (Z_TYPE_P(&v) == IS_ARRAY) {
				ZEPHIR_INIT_NVAR(&_4$$5);
				zephir_fast_join_str(&_4$$5, SL("\", \""), &v);
				ZEPHIR_INIT_NVAR(&_5$$5);
				ZEPHIR_CONCAT_SVS(&_5$$5, "[\"", &_4$$5, "\"]");
				zephir_concat_self(&ret, &_5$$5);
			} else {
				ZEPHIR_INIT_NVAR(&_6$$6);
				ZEPHIR_CONCAT_SVS(&_6$$6, "\"", &v, "\"");
				zephir_concat_self(&ret, &_6$$6);
			}
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, &properties, "rewind", NULL, 0);
		zephir_check_call_status();
		_8 = 1;
		while (1) {
			if (_8) {
				_8 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, &properties, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_7, &properties, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_7)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&k, &properties, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&v, &properties, "current", NULL, 0);
			zephir_check_call_status();
				if (x) {
					zephir_concat_self_str(&ret, SL(", "));
				}
				x = 1;
				ZEPHIR_INIT_NVAR(&_9$$7);
				ZEPHIR_CONCAT_VS(&_9$$7, &k, ": ");
				zephir_concat_self(&ret, &_9$$7);
				if (Z_TYPE_P(&v) == IS_ARRAY) {
					ZEPHIR_INIT_NVAR(&_10$$9);
					zephir_fast_join_str(&_10$$9, SL("\", \""), &v);
					ZEPHIR_INIT_NVAR(&_11$$9);
					ZEPHIR_CONCAT_SVS(&_11$$9, "[\"", &_10$$9, "\"]");
					zephir_concat_self(&ret, &_11$$9);
				} else {
					ZEPHIR_INIT_NVAR(&_12$$10);
					ZEPHIR_CONCAT_SVS(&_12$$10, "\"", &v, "\"");
					zephir_concat_self(&ret, &_12$$10);
				}
		}
	}
	ZEPHIR_INIT_NVAR(&v);
	ZEPHIR_INIT_NVAR(&k);
	ZEPHIR_CONCAT_VS(return_value, &ret, "}");
	RETURN_MM();
}

