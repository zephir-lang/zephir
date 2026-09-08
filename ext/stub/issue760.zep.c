
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
#include "kernel/array.h"
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/string.h"
#include "kernel/fcall.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/760
 *
 * PHP's `$str[$off]` is a byte-indexed string offset. Zephir only supported it
 * when the source variable was declared `string`, so the compile-time type
 * switch in `NativeArrayAccess` could route it to `ZEPHIR_STRING_OFFSET`. A
 * `var` holding a string fell through to the array kernel and produced NULL.
 *
 * Every method here has a plain-PHP counterpart in the test, and the two
 * transcripts (diagnostics + value) must be byte-identical.
 */
ZEPHIR_INIT_CLASS(Stub_Issue760)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue760, stub, issue760, stub_issue760_method_entry, 0);

	return SUCCESS;
}

/**
 * Dynamic source, native int offset. Emits `zephir_array_fetch_long()`.
 */
PHP_METHOD(Stub_Issue760, dynIntOffset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long i;
	zval *s, s_sub, *i_param = NULL, ch;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&ch);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(s)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &s, &i_param);
	zephir_memory_observe(&ch);
	zephir_array_fetch_long(&ch, s, i, PH_NOISY, "stub/issue760.zep", 22);
	RETURN_CCTOR(&ch);
}

/**
 * Dynamic source, dynamic offset. Emits `zephir_array_fetch()` and so
 * exercises PHP's offset normalisation (numeric strings, casts, TypeError).
 */
PHP_METHOD(Stub_Issue760, dynVarOffset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *s, s_sub, *i, i_sub, ch;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&ch);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(s)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &s, &i);
	zephir_memory_observe(&ch);
	zephir_array_fetch(&ch, s, i, PH_NOISY, "stub/issue760.zep", 33);
	RETURN_CCTOR(&ch);
}

/**
 * Declared `string` source: the path that already worked, kept as a
 * control so a regression there is visible.
 */
PHP_METHOD(Stub_Issue760, strIntOffset)
{
	unsigned char _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long i;
	zval s_zv, *i_param = NULL, ch, _1;
	zend_string *s = NULL;

	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&ch);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(s)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	i_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_INIT_VAR(&_1);
	zephir_string_offset_read(&_1, &s_zv, i, PH_NOISY);
	ZEPHIR_CPY_WRT(&ch, &_1);
	RETURN_CCTOR(&ch);
}

/**
 * A local `string` is a plain zval, unlike a `string` parameter which is a
 * `zend_string*` plus a companion `_zv`.
 */
PHP_METHOD(Stub_Issue760, localStringOffset)
{
	unsigned char _0;
	zval s;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *i_param = NULL, ch, _1;
	zend_long i;

	ZVAL_UNDEF(&ch);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&s);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &i_param);
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "abcdef");
	ZEPHIR_INIT_VAR(&_1);
	zephir_string_offset_read(&_1, &s, i, PH_NOISY);
	ZEPHIR_CPY_WRT(&ch, &_1);
	RETURN_CCTOR(&ch);
}

/**
 * A native `char` destination takes the raw byte, not a 1-char string:
 * `char` is int-like by construction (#2069). Out of range still yields
 * NUL, but must warn exactly as PHP does.
 */
PHP_METHOD(Stub_Issue760, charDest)
{
	char c = 0;
	zend_long i;
	zval s_zv, *i_param = NULL;
	zend_string *s = NULL;

	ZVAL_UNDEF(&s_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(s)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	i_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&s_zv, s);
	c = zephir_string_offset_byte(&s_zv, i, PH_NOISY);
	c = c;
	RETURN_LONG((unsigned char) c);
}

/**
 * Same as charDest, sourced from a `var`. Used to fail to compile with
 * "Cannot use variable: char to assign array index".
 */
PHP_METHOD(Stub_Issue760, dynCharDest)
{
	char c = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long i;
	zval *s, s_sub, *i_param = NULL, _0;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(s)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &s, &i_param);
	zephir_memory_observe(&_0);
	zephir_array_fetch_long(&_0, s, i, PH_NOISY, "stub/issue760.zep", 81);
	c = (unsigned char) zephir_get_charval(&_0);
	c = c;
	RETURN_MM_LONG((unsigned char) c);
}

/**
 * A `string` source with a dynamic offset. Used to fail to compile with
 * "Cannot use index type variable as offset"; the offset now runs PHP's
 * normalisation at runtime.
 */
PHP_METHOD(Stub_Issue760, strVarOffset)
{
	unsigned char _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval s_zv, *i, i_sub, ch, _1;
	zend_string *s = NULL;

	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&ch);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(s)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	i = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_INIT_VAR(&_1);
	zephir_string_offset_read_zval(&_1, &s_zv, i, PH_NOISY);
	ZEPHIR_CPY_WRT(&ch, &_1);
	RETURN_CCTOR(&ch);
}

/**
 * A string literal source, PHP's `"abcdef"[1]`. Used to fail to compile
 * with "Cannot use expression: string as an array".
 */
PHP_METHOD(Stub_Issue760, literalOffset)
{
	unsigned char _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *i, i_sub, ch, _0, _2;

	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&ch);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &i);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "abcdef");
	ZEPHIR_INIT_VAR(&_2);
	zephir_string_offset_read_zval(&_2, &_0, i, PH_NOISY);
	ZEPHIR_CPY_WRT(&ch, &_2);
	RETURN_CCTOR(&ch);
}

/**
 * Returning the offset straight out, with no intermediate variable: the
 * destination is `return_value`, which takes a different boxing decision.
 */
PHP_METHOD(Stub_Issue760, returnStrOffset)
{
	unsigned char _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long i;
	zval s_zv, *i_param = NULL, _1;
	zend_string *s = NULL;

	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(s)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	i_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_INIT_VAR(&_1);
	zephir_string_offset_read(&_1, &s_zv, i, PH_NOISY);
	RETURN_CCTOR(&_1);
}

PHP_METHOD(Stub_Issue760, returnDynOffset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long i;
	zval *s, s_sub, *i_param = NULL, _0;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(s)
		Z_PARAM_LONG(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &s, &i_param);
	zephir_memory_observe(&_0);
	zephir_array_fetch_long(&_0, s, i, PH_NOISY, "stub/issue760.zep", 119);
	RETURN_CCTOR(&_0);
}

/**
 * isset() on a string offset: PHP is silent for every illegal offset, and
 * folds a negative one from the end.
 */
PHP_METHOD(Stub_Issue760, dynIsset)
{
	zval *s, s_sub, *i, i_sub;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&i_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(s)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &s, &i);
	RETURN_BOOL(zephir_array_isset_value(s, i));
}

PHP_METHOD(Stub_Issue760, strIsset)
{
	zval s_zv, *i, i_sub;
	zend_string *s = NULL;

	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&i_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(s)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	i = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&s_zv, s);
	RETURN_BOOL(zephir_array_isset_value(&s_zv, i));
}

/**
 * empty() must not report anything either: PHP has a separate silent
 * handler for it, so `empty(s[99])` warns in neither language.
 */
PHP_METHOD(Stub_Issue760, dynEmpty)
{
	zval *s, s_sub, *i, i_sub;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&i_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(s)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(2, 0, &s, &i);
	RETURN_BOOL(zephir_isempty_dim(s, i));
}

PHP_METHOD(Stub_Issue760, strEmpty)
{
	zval s_zv, *i, i_sub;
	zend_string *s = NULL;

	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&i_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(s)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	i = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&s_zv, s);
	RETURN_BOOL(zephir_isempty_dim(&s_zv, i));
}

/**
 * `empty(arr[9])` used to emit "Undefined index", which PHP never does.
 */
PHP_METHOD(Stub_Issue760, arrayEmpty)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *a_param = NULL, *i, i_sub;
	zval a;

	ZVAL_UNDEF(&a);
	ZVAL_UNDEF(&i_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		ZEPHIR_Z_PARAM_ARRAY(a, a_param)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &a_param, &i);
	zephir_get_arrval(&a, a_param);
	RETURN_MM_BOOL(zephir_isempty_dim(&a, i));
}

/**
 * unset() and [] on a string are runtime Errors in PHP. Only reachable
 * through a `var`; on a declared `string` the compiler rejects them
 * outright, since the operation could never have succeeded.
 */
PHP_METHOD(Stub_Issue760, dynUnset)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *s, s_sub, *i, i_sub;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&i_sub);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(s)
		Z_PARAM_ZVAL(i)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &s, &i);
	ZEPHIR_SEPARATE_PARAM(s);
	zephir_array_unset(s, i, PH_SEPARATE);
	RETVAL_ZVAL(s, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue760, dynAppend)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *s, s_sub, _0;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &s);
	ZEPHIR_SEPARATE_PARAM(s);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRING(&_0, "x");
	zephir_array_append(s, &_0, PH_SEPARATE, "stub/issue760.zep", 172);
	RETVAL_ZVAL(s, 1, 0);
	RETURN_MM();
}

/**
 * Writing through a string offset. Extends the string with spaces when the
 * offset is past the end, stores only the first byte, and refuses an empty
 * value.
 */
PHP_METHOD(Stub_Issue760, dynWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *s, s_sub, *i, i_sub, *v, v_sub;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(s)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &s, &i, &v);
	ZEPHIR_SEPARATE_PARAM(s);
	zephir_array_update_zval(s, i, v, PH_COPY | PH_SEPARATE);
	RETVAL_ZVAL(s, 1, 0);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue760, dynWriteInt)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long i;
	zval *s, s_sub, *i_param = NULL, *v, v_sub;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(s)
		Z_PARAM_LONG(i)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &s, &i_param, &v);
	ZEPHIR_SEPARATE_PARAM(s);
	zephir_array_update_long(s, i, v, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETVAL_ZVAL(s, 1, 0);
	RETURN_MM();
}

/**
 * Writing through a declared `string`. A `string` parameter is passed by
 * value in PHP, so the caller's string must be left alone.
 */
PHP_METHOD(Stub_Issue760, strWrite)
{
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *s_param = NULL, *i, i_sub, *v, v_sub;
	zval s;

	ZVAL_UNDEF(&s);
	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&v_sub);
	ZEND_PARSE_PARAMETERS_START(3, 3)
		Z_PARAM_ZVAL(s_param)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 3, 0, &s_param, &i, &v);
	zephir_get_strval(&s, s_param);
	zephir_string_offset_write_zval(&s, i, v);
	RETURN_CTOR(&s);
}

PHP_METHOD(Stub_Issue760, localStringWrite)
{
	zval s;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval *i, i_sub, *v, v_sub;

	ZVAL_UNDEF(&i_sub);
	ZVAL_UNDEF(&v_sub);
	ZVAL_UNDEF(&s);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_ZVAL(i)
		Z_PARAM_ZVAL(v)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 2, 0, &i, &v);
	ZEPHIR_INIT_VAR(&s);
	ZVAL_STRING(&s, "abcdef");
	zephir_string_offset_write_zval(&s, i, v);
	RETURN_CTOR(&s);
}

/**
 * `for c in s` over a dynamically typed source. A declared `string`
 * traverses its bytes, so a `var` holding one has to do the same.
 */
PHP_METHOD(Stub_Issue760, dynTraverse)
{
	zend_bool _4;
	zval out;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *s, s_sub, c, *_0, _1, *_2, _3;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&out);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &s);
	ZEPHIR_INIT_VAR(&out);
	array_init(&out);
	if (Z_TYPE_P(s) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, s);
		_0 = &_1;
	} else {
		_0 = s;
	}
	zephir_is_iterable(_0, 0, "stub/issue760.zep", 229);
	if (Z_TYPE_P(_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(_0), _2)
		{
			ZEPHIR_INIT_NVAR(&c);
			ZVAL_COPY(&c, _2);
			zephir_array_append(&out, &c, PH_SEPARATE, "stub/issue760.zep", 226);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _0, "rewind", NULL, 0);
		zephir_check_call_status();
		_4 = 1;
		while (1) {
			if (_4) {
				_4 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_3, _0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&c, _0, "current", NULL, 0);
			zephir_check_call_status();
				zephir_array_append(&out, &c, PH_SEPARATE, "stub/issue760.zep", 226);
		}
	}
	ZEPHIR_INIT_NVAR(&c);
	RETURN_CTOR(&out);
}

PHP_METHOD(Stub_Issue760, dynTraverseKeyed)
{
	zend_bool _7;
	zend_string *_4;
	zend_ulong _3;
	zval out, _5$$3, _8$$4;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *s, s_sub, k, c, *_0, _1, *_2, _6;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&k);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_6);
	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&_5$$3);
	ZVAL_UNDEF(&_8$$4);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &s);
	ZEPHIR_INIT_VAR(&out);
	array_init(&out);
	if (Z_TYPE_P(s) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, s);
		_0 = &_1;
	} else {
		_0 = s;
	}
	zephir_is_iterable(_0, 0, "stub/issue760.zep", 241);
	if (Z_TYPE_P(_0) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(_0), _3, _4, _2)
		{
			ZEPHIR_INIT_NVAR(&k);
			if (_4 != NULL) { 
				ZVAL_STR_COPY(&k, _4);
			} else {
				ZVAL_LONG(&k, _3);
			}
			ZEPHIR_INIT_NVAR(&c);
			ZVAL_COPY(&c, _2);
			ZEPHIR_INIT_NVAR(&_5$$3);
			zephir_create_array(&_5$$3, 2, 0);
			zephir_array_fast_append(&_5$$3, &k);
			zephir_array_fast_append(&_5$$3, &c);
			zephir_array_append(&out, &_5$$3, PH_SEPARATE, "stub/issue760.zep", 238);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _0, "rewind", NULL, 0);
		zephir_check_call_status();
		_7 = 1;
		while (1) {
			if (_7) {
				_7 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_6, _0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_6)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&k, _0, "key", NULL, 0);
			zephir_check_call_status();
			ZEPHIR_CALL_METHOD(&c, _0, "current", NULL, 0);
			zephir_check_call_status();
				ZEPHIR_INIT_NVAR(&_8$$4);
				zephir_create_array(&_8$$4, 2, 0);
				zephir_array_fast_append(&_8$$4, &k);
				zephir_array_fast_append(&_8$$4, &c);
				zephir_array_append(&out, &_8$$4, PH_SEPARATE, "stub/issue760.zep", 238);
		}
	}
	ZEPHIR_INIT_NVAR(&c);
	ZEPHIR_INIT_NVAR(&k);
	RETURN_CTOR(&out);
}

PHP_METHOD(Stub_Issue760, dynTraverseReverse)
{
	zend_bool _4;
	zval out;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long ZEPHIR_LAST_CALL_STATUS;
	zval *s, s_sub, c, *_0, _1, *_2, _3;

	ZVAL_UNDEF(&s_sub);
	ZVAL_UNDEF(&c);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&_3);
	ZVAL_UNDEF(&out);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_fetch_params(1, 1, 0, &s);
	ZEPHIR_INIT_VAR(&out);
	array_init(&out);
	if (Z_TYPE_P(s) == IS_STRING) {
		ZEPHIR_INIT_VAR(&_1);
		zephir_string_to_char_array(&_1, s);
		_0 = &_1;
	} else {
		_0 = s;
	}
	zephir_is_iterable(_0, 0, "stub/issue760.zep", 253);
	if (Z_TYPE_P(_0) == IS_ARRAY) {
		ZEND_HASH_REVERSE_FOREACH_VAL(Z_ARRVAL_P(_0), _2)
		{
			ZEPHIR_INIT_NVAR(&c);
			ZVAL_COPY(&c, _2);
			zephir_array_append(&out, &c, PH_SEPARATE, "stub/issue760.zep", 250);
		} ZEND_HASH_FOREACH_END();
	} else {
		ZEPHIR_CALL_METHOD(NULL, _0, "rewind", NULL, 0);
		zephir_check_call_status();
		_4 = 1;
		while (1) {
			if (_4) {
				_4 = 0;
			} else {
				ZEPHIR_CALL_METHOD(NULL, _0, "next", NULL, 0);
				zephir_check_call_status();
			}
			ZEPHIR_CALL_METHOD(&_3, _0, "valid", NULL, 0);
			zephir_check_call_status();
			if (!zend_is_true(&_3)) {
				break;
			}
			ZEPHIR_CALL_METHOD(&c, _0, "current", NULL, 0);
			zephir_check_call_status();
				zephir_array_append(&out, &c, PH_SEPARATE, "stub/issue760.zep", 250);
		}
	}
	ZEPHIR_INIT_NVAR(&c);
	RETURN_CTOR(&out);
}

PHP_METHOD(Stub_Issue760, strTraverse)
{
	zend_long _0;
	zval out;
	char c = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval s_zv, _1$$3;
	zend_string *s = NULL;

	ZVAL_UNDEF(&s_zv);
	ZVAL_UNDEF(&_1$$3);
	ZVAL_UNDEF(&out);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(s)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&s_zv);
	ZVAL_STR_COPY(&s_zv, s);
	ZEPHIR_INIT_VAR(&out);
	array_init(&out);
	for (_0 = 0; _0 < Z_STRLEN_P(&s_zv); _0++) {
		c = ZEPHIR_STRING_OFFSET(&s_zv, _0);
		ZEPHIR_INIT_NVAR(&_1$$3);
		ZVAL_LONG(&_1$$3, c);
		zephir_array_append(&out, &_1$$3, PH_SEPARATE, "stub/issue760.zep", 262);
	}
	RETURN_CTOR(&out);
}

