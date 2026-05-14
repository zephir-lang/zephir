
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
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_BuiltIn_StringMethods)
{
	ZEPHIR_REGISTER_CLASS(Stub\\BuiltIn, StringMethods, stub, builtin_stringmethods, stub_builtin_stringmethods_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_BuiltIn_StringMethods, camelize)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, *delimiter = NULL, delimiter_sub, __$null, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(delimiter)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 1) {
		delimiter = ZEND_CALL_ARG(execute_data, 2);
	}
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	if (!delimiter) {
		delimiter = &delimiter_sub;
		delimiter = &__$null;
	}
	ZEPHIR_INIT_VAR(&_0);
	zephir_camelize(&_0, &str_zv, delimiter);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, uncamelize)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, *delimiter = NULL, delimiter_sub, __$null, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&delimiter_sub);
	ZVAL_NULL(&__$null);
	ZVAL_UNDEF(&_0);
	bool is_null_true = 1;
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_ZVAL_OR_NULL(delimiter)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 1) {
		delimiter = ZEND_CALL_ARG(execute_data, 2);
	}
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	if (!delimiter) {
		delimiter = &delimiter_sub;
		delimiter = &__$null;
	}
	ZEPHIR_INIT_VAR(&_0);
	zephir_uncamelize(&_0, &str_zv, delimiter);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLength1)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	RETURN_MM_LONG(zephir_fast_strlen_ev(&_0));
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLength2)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello");
	RETURN_MM_LONG(zephir_fast_strlen_ev(&_0));
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLength3)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SS(&_0, "hello", "hello");
	RETURN_MM_LONG(zephir_fast_strlen_ev(&_0));
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLength4)
{
	zval a_zv;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(a)
	ZEND_PARSE_PARAMETERS_END();
	ZVAL_STR(&a_zv, a);
	RETURN_LONG(zephir_fast_strlen_ev(&a_zv));
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLength5)
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval a_zv;
	zend_string *a = NULL;

	ZVAL_UNDEF(&a_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&a_zv);
	ZVAL_STR_COPY(&a_zv, a);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SV(&_0, "hello", &a_zv);
	RETURN_MM_LONG(zephir_fast_strlen_ev(&_0));
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getIndex)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, needle_zv, _0;
	zend_string *str = NULL, *needle = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&needle_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_STR(needle)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	zephir_memory_observe(&needle_zv);
	ZVAL_STR_COPY(&needle_zv, needle);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_strpos(&_0, &str_zv, &needle_zv, 0 );
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getIndexWithPosition)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long position;
	zval str_zv, needle_zv, *position_param = NULL, _0, _1;
	zend_string *str = NULL, *needle = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&needle_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(str)
		Z_PARAM_STR(needle)
		Z_PARAM_LONG(position)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	position_param = ZEND_CALL_ARG(execute_data, 3);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	zephir_memory_observe(&needle_zv);
	ZVAL_STR_COPY(&needle_zv, needle);
	ZVAL_LONG(&_0, position);
	ZEPHIR_INIT_VAR(&_1);
	zephir_fast_strpos(&_1, &str_zv, &needle_zv, zephir_get_intval(&_0) );
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getTrimmed)
{
	zval _0, _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRING(&_1, " hello \t\n");
	zephir_fast_trim(&_0, &_1, NULL , ZEPHIR_TRIM_BOTH);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getTrimmed1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_trim(&_0, &str_zv, NULL , ZEPHIR_TRIM_BOTH);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLeftTrimmed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_trim(&_0, &str_zv, NULL , ZEPHIR_TRIM_LEFT);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getRightTrimmed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_trim(&_0, &str_zv, NULL , ZEPHIR_TRIM_RIGHT);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLower)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_strtolower(&_0, &str_zv);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getUpper)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_strtoupper(&_0, &str_zv);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLowerFirst)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_CALL_FUNCTION(&_0, "lcfirst", NULL, 17, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getUpperFirst)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	zephir_ucfirst(&_0, &str_zv);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getFormatted)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0, _1;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello %s!");
	ZEPHIR_CALL_FUNCTION(&_1, "sprintf", NULL, 18, &_0, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getMd5)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_INIT_VAR(&_0);
	zephir_md5(&_0, &str_zv);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getSha1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_CALL_FUNCTION(&_0, "sha1", NULL, 19, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getNl2br)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_CALL_FUNCTION(&_0, "nl2br", NULL, 20, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getParsedCsv)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_CALL_FUNCTION(&_0, "str_getcsv", NULL, 21, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getParsedJson)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool assoc;
	zval str_zv, *assoc_param = NULL, _0, _1;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(assoc)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	if (ZEND_NUM_ARGS() > 1) {
		assoc_param = ZEND_CALL_ARG(execute_data, 2);
	}
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	if (!assoc_param) {
		assoc = 1;
	} else {
		}
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_1, (assoc ? 1 : 0));
	zephir_json_decode(&_0, &str_zv, zephir_get_intval(&str_zv) );
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getRepeatted)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long count, ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, *count_param = NULL, _0, _1;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	count_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZVAL_LONG(&_0, count);
	ZEPHIR_CALL_FUNCTION(&_1, "str_repeat", NULL, 22, &str_zv, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getShuffled)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_CALL_FUNCTION(&_0, "str_shuffle", NULL, 23, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getSplited)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, del_zv, _0;
	zend_string *str = NULL, *del = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&del_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_STR(del)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	zephir_memory_observe(&del_zv);
	ZVAL_STR_COPY(&del_zv, del);
	ZEPHIR_CALL_FUNCTION(&_0, "str_split", NULL, 24, &str_zv, &del_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getCompare)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval left_zv, right_zv, _0;
	zend_string *left = NULL, *right = NULL;

	ZVAL_UNDEF(&left_zv);
	ZVAL_UNDEF(&right_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(left)
		Z_PARAM_STR(right)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&left_zv);
	ZVAL_STR_COPY(&left_zv, left);
	zephir_memory_observe(&right_zv);
	ZVAL_STR_COPY(&right_zv, right);
	ZEPHIR_CALL_FUNCTION(&_0, "strcmp", NULL, 25, &left_zv, &right_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getCompareLocale)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval left_zv, right_zv, _0;
	zend_string *left = NULL, *right = NULL;

	ZVAL_UNDEF(&left_zv);
	ZVAL_UNDEF(&right_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(left)
		Z_PARAM_STR(right)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&left_zv);
	ZVAL_STR_COPY(&left_zv, left);
	zephir_memory_observe(&right_zv);
	ZVAL_STR_COPY(&right_zv, right);
	ZEPHIR_CALL_FUNCTION(&_0, "strcoll", NULL, 26, &left_zv, &right_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getReversed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_CALL_FUNCTION(&_0, "strrev", NULL, 27, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getHtmlSpecialChars)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval str_zv, _0;
	zend_string *str = NULL;

	ZVAL_UNDEF(&str_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&str_zv);
	ZVAL_STR_COPY(&str_zv, str);
	ZEPHIR_CALL_FUNCTION(&_0, "htmlspecialchars", NULL, 28, &str_zv);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

