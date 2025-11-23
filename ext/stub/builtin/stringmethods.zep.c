
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
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/concat.h"
#include "kernel/fcall.h"


ZEPHIR_INIT_CLASS(Stub_BuiltIn_StringMethods)
{
	ZEPHIR_REGISTER_CLASS(Stub\\BuiltIn, StringMethods, stub, builtin_stringmethods, stub_builtin_stringmethods_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_BuiltIn_StringMethods, camelize)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, *delimiter = NULL, delimiter_sub, __$null, _0;
	zval str;

	ZVAL_UNDEF(&str);
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
	zephir_fetch_params(1, 1, 1, &str_param, &delimiter);
	zephir_get_strval(&str, str_param);
	if (!delimiter) {
		delimiter = &delimiter_sub;
		delimiter = &__$null;
	}
	ZEPHIR_INIT_VAR(&_0);
	zephir_camelize(&_0, &str, delimiter);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, uncamelize)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, *delimiter = NULL, delimiter_sub, __$null, _0;
	zval str;

	ZVAL_UNDEF(&str);
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
	zephir_fetch_params(1, 1, 1, &str_param, &delimiter);
	zephir_get_strval(&str, str_param);
	if (!delimiter) {
		delimiter = &delimiter_sub;
		delimiter = &__$null;
	}
	ZEPHIR_INIT_VAR(&_0);
	zephir_uncamelize(&_0, &str, delimiter);
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
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL;
	zval a;

	ZVAL_UNDEF(&a);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_strval(&a, a_param);
	RETURN_MM_LONG(zephir_fast_strlen_ev(&a));
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLength5)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL;
	zval a, _0;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(a)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &a_param);
	zephir_get_strval(&a, a_param);
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_CONCAT_SV(&_0, "hello", &a);
	RETURN_MM_LONG(zephir_fast_strlen_ev(&_0));
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getIndex)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, *needle_param = NULL, _0;
	zval str, needle;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&needle);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_STR(needle)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &str_param, &needle_param);
	zephir_get_strval(&str, str_param);
	zephir_get_strval(&needle, needle_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_strpos(&_0, &str, &needle, 0 );
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getIndexWithPosition)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long position;
	zval *str_param = NULL, *needle_param = NULL, *position_param = NULL, _0, _1;
	zval str, needle;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&needle);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_STR(str)
		Z_PARAM_STR(needle)
		Z_PARAM_LONG(position)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &str_param, &needle_param, &position_param);
	zephir_get_strval(&str, str_param);
	zephir_get_strval(&needle, needle_param);
	ZVAL_LONG(&_0, position);
	ZEPHIR_INIT_VAR(&_1);
	zephir_fast_strpos(&_1, &str, &needle, zephir_get_intval(&_0) );
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
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_trim(&_0, &str, NULL , ZEPHIR_TRIM_BOTH);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLeftTrimmed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_trim(&_0, &str, NULL , ZEPHIR_TRIM_LEFT);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getRightTrimmed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_trim(&_0, &str, NULL , ZEPHIR_TRIM_RIGHT);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLower)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_strtolower(&_0, &str);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getUpper)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_fast_strtoupper(&_0, &str);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getLowerFirst)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_CALL_FUNCTION(&_0, "lcfirst", NULL, 18, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getUpperFirst)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_ucfirst(&_0, &str);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getFormatted)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0, _1;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "hello %s!");
	ZEPHIR_CALL_FUNCTION(&_1, "sprintf", NULL, 19, &_0, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getMd5)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_INIT_VAR(&_0);
	zephir_md5(&_0, &str);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getSha1)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_CALL_FUNCTION(&_0, "sha1", NULL, 20, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getNl2br)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_CALL_FUNCTION(&_0, "nl2br", NULL, 21, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getParsedCsv)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_CALL_FUNCTION(&_0, "str_getcsv", NULL, 22, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getParsedJson)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_bool assoc;
	zval *str_param = NULL, *assoc_param = NULL, _0, _1;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(1, 2)
		Z_PARAM_STR(str)
		Z_PARAM_OPTIONAL
		Z_PARAM_BOOL(assoc)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 1, &str_param, &assoc_param);
	zephir_get_strval(&str, str_param);
	if (!assoc_param) {
		assoc = 1;
	} else {
		}
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_BOOL(&_1, (assoc ? 1 : 0));
	zephir_json_decode(&_0, &str, zephir_get_intval(&str) );
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getRepeatted)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long count, ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, *count_param = NULL, _0, _1;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_LONG(count)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &str_param, &count_param);
	zephir_get_strval(&str, str_param);
	ZVAL_LONG(&_0, count);
	ZEPHIR_CALL_FUNCTION(&_1, "str_repeat", NULL, 23, &str, &_0);
	zephir_check_call_status();
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getShuffled)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_CALL_FUNCTION(&_0, "str_shuffle", NULL, 24, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getSplited)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, *del_param = NULL, _0;
	zval str, del;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&del);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(str)
		Z_PARAM_STR(del)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &str_param, &del_param);
	zephir_get_strval(&str, str_param);
	zephir_get_strval(&del, del_param);
	ZEPHIR_CALL_FUNCTION(&_0, "str_split", NULL, 25, &str, &del);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getCompare)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *left_param = NULL, *right_param = NULL, _0;
	zval left, right;

	ZVAL_UNDEF(&left);
	ZVAL_UNDEF(&right);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(left)
		Z_PARAM_STR(right)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &left_param, &right_param);
	zephir_get_strval(&left, left_param);
	zephir_get_strval(&right, right_param);
	ZEPHIR_CALL_FUNCTION(&_0, "strcmp", NULL, 26, &left, &right);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getCompareLocale)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *left_param = NULL, *right_param = NULL, _0;
	zval left, right;

	ZVAL_UNDEF(&left);
	ZVAL_UNDEF(&right);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(left)
		Z_PARAM_STR(right)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &left_param, &right_param);
	zephir_get_strval(&left, left_param);
	zephir_get_strval(&right, right_param);
	ZEPHIR_CALL_FUNCTION(&_0, "strcoll", NULL, 27, &left, &right);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getReversed)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_CALL_FUNCTION(&_0, "strrev", NULL, 28, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_BuiltIn_StringMethods, getHtmlSpecialChars)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *str_param = NULL, _0;
	zval str;

	ZVAL_UNDEF(&str);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &str_param);
	zephir_get_strval(&str, str_param);
	ZEPHIR_CALL_FUNCTION(&_0, "htmlspecialchars", NULL, 29, &str);
	zephir_check_call_status();
	RETURN_CCTOR(&_0);
}

