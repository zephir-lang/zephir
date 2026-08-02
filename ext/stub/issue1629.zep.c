
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
#include "kernel/operators.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/string.h"


/**
 * @issue https://github.com/zephir-lang/zephir/issues/1629
 *
 * A string offset read (`s[i]`) is PHP's `$s[$i]`: a 1-char string. It boxes as
 * one whenever it crosses into a zval. A declared `char`/`uchar` variable is a
 * native numeric byte instead (it does arithmetic), so boxing that yields its
 * integer value -- see `Chars::arrayOfChars()` and #1988.
 */
ZEPHIR_INIT_CLASS(Stub_Issue1629)
{
	ZEPHIR_REGISTER_CLASS(Stub, Issue1629, stub, issue1629, stub_issue1629_method_entry, 0);

	zend_declare_property_null(stub_issue1629_ce, SL("parts"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(stub_issue1629_ce, SL("staticParts"), ZEND_ACC_PROTECTED|ZEND_ACC_STATIC);
	stub_issue1629_ce->create_object = zephir_init_properties_Stub_Issue1629;

	return SUCCESS;
}

/**
 * Case 1: a `string` target. Used to emit dead PHP-5 C that gcc rejected.
 */
PHP_METHOD(Stub_Issue1629, stringTarget)
{
	unsigned char _0;
	zval ch;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _1;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&ch);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	_0 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRINGL(&_1, &_0, 1);
	zephir_get_strval(&ch, &_1);
	RETURN_CTOR(&ch);
}

/**
 * Case 2: a declared `char` boxed into an array keeps its byte value.
 * Used to fail with "Unknown type: char".
 */
PHP_METHOD(Stub_Issue1629, charTarget)
{
	zval pieces;
	char ch = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _0;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&pieces);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ZEPHIR_INIT_VAR(&pieces);
	array_init(&pieces);
	ch = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ch = ch;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, ch);
	zephir_array_append(&pieces, &_0, PH_SEPARATE, "stub/issue1629.zep", 37);
	RETURN_CTOR(&pieces);
}

/**
 * Case 3: append the offset read straight into an array, then implode.
 * Used to fail with "Unknown type: uchar".
 */
PHP_METHOD(Stub_Issue1629, rebuild)
{
	unsigned char _3$$3;
	zend_bool _0;
	zval pieces;
	zend_long i = 0, max = 0, _1, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zval keyspace_zv, _4$$3;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_4$$3);
	ZVAL_UNDEF(&pieces);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(keyspace)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ZEPHIR_INIT_VAR(&pieces);
	array_init(&pieces);
	max = (zephir_fast_strlen_ev(&keyspace_zv) - 1);
	_2 = max;
	_1 = 0;
	_0 = 0;
	if (_1 <= _2) {
		while (1) {
			if (_0) {
				_1++;
				if (!(_1 <= _2)) {
					break;
				}
			} else {
				_0 = 1;
			}
			i = _1;
			_3$$3 = ZEPHIR_STRING_OFFSET(&keyspace_zv, i);
			ZEPHIR_INIT_NVAR(&_4$$3);
			ZVAL_STRINGL(&_4$$3, &_3$$3, 1);
			zephir_array_append(&pieces, &_4$$3, PH_SEPARATE, "stub/issue1629.zep", 54);
		}
	}
	zephir_fast_join_str(return_value, SL(""), &pieces);
	RETURN_MM();
}

/**
 * The workaround the reporter settled on -- must match PHP's `$s[$i]`.
 */
PHP_METHOD(Stub_Issue1629, varTarget)
{
	unsigned char _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, ch, _1;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&ch);
	ZVAL_UNDEF(&_1);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	_0 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRINGL(&_1, &_0, 1);
	ZEPHIR_CPY_WRT(&ch, &_1);
	RETURN_CCTOR(&ch);
}

/**
 * A native-typed target still reads the raw byte (unchanged).
 */
PHP_METHOD(Stub_Issue1629, intTarget)
{
	unsigned char _0;
	zend_long nb, c = 0;
	zval keyspace_zv, *nb_param = NULL;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&keyspace_zv, keyspace);
	_0 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	c = _0;
	RETURN_LONG(c);
}

PHP_METHOD(Stub_Issue1629, arrayIndexTarget)
{
	unsigned char _0;
	zval pieces;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _1;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&pieces);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ZEPHIR_INIT_VAR(&pieces);
	array_init(&pieces);
	_0 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRINGL(&_1, &_0, 1);
	zephir_array_update_long(&pieces, 3, &_1, PH_COPY | PH_SEPARATE ZEPHIR_DEBUG_PARAMS_DUMMY);
	RETURN_CTOR(&pieces);
}

PHP_METHOD(Stub_Issue1629, propertyAppendTarget)
{
	unsigned char _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _0, _2;
	zend_string *keyspace = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("parts", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 45, &_0);
	_1 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRINGL(&_2, &_1, 1);
	zephir_update_property_array_append(this_ptr, SL("parts"), &_2);
	RETURN_MM_MEMBER_TYPED(getThis(), "parts", IS_ARRAY);
}

PHP_METHOD(Stub_Issue1629, propertyIndexTarget)
{
	unsigned char _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _0, _2, _3;
	zend_string *keyspace = NULL;
	zval *this_ptr = getThis();

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	static zend_string *_zephir_prop_0 = NULL;
	if (UNEXPECTED(!_zephir_prop_0)) {
		_zephir_prop_0 = zend_string_init("parts", 5, 1);
	}

	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_property_zval_cached(this_ptr, _zephir_prop_0, 45, &_0);
	_1 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRINGL(&_2, &_1, 1);
	ZEPHIR_INIT_VAR(&_3);
	ZVAL_STRING(&_3, "k");
	zephir_update_property_array(this_ptr, SL("parts"), &_3, &_2);
	RETURN_MM_MEMBER_TYPED(getThis(), "parts", IS_ARRAY);
}

PHP_METHOD(Stub_Issue1629, staticIndexTarget)
{
	unsigned char _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _0, _2, _3;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue1629_ce, ZEND_STRL("staticParts"), &_0);
	_1 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRINGL(&_2, &_1, 1);
	zephir_update_static_property_array_multi_ce(stub_issue1629_ce, SL("staticParts"), &_2, SL("l"), 1, 0);
	zephir_read_static_property_ce(&_3, stub_issue1629_ce, SL("staticParts"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_3);
}

PHP_METHOD(Stub_Issue1629, staticAppendTarget)
{
	unsigned char _1;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _0, _2, _3;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_2);
	ZVAL_UNDEF(&_3);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue1629_ce, ZEND_STRL("staticParts"), &_0);
	_1 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_2);
	ZVAL_STRINGL(&_2, &_1, 1);
	zephir_update_static_property_array_multi_ce(stub_issue1629_ce, SL("staticParts"), &_2, SL("a"), 1);
	zephir_read_static_property_ce(&_3, stub_issue1629_ce, SL("staticParts"), PH_NOISY_CC | PH_READONLY);
	RETURN_CTOR(&_3);
}

/**
 * A declared `char` assigned to a `string` target: the string-typed left
 * hand side asks for the character, not the byte.
 */
PHP_METHOD(Stub_Issue1629, charToStringTarget)
{
	zval out;
	char ch = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&out);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ch = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ch = ch;
	ZEPHIR_INIT_VAR(&out);
	ZVAL_STRINGL(&out, &ch, 1);
	RETURN_CTOR(&out);
}

/**
 * Sibling of the same dead PHP-5 codegen: an `int` assigned to a `string`.
 */
PHP_METHOD(Stub_Issue1629, intToStringTarget)
{
	zval out;
	zend_long n;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&out);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	n = 42;
	ZEPHIR_INIT_VAR(&out);
	ZVAL_STR(&out, zend_long_to_str(n));
	RETURN_CTOR(&out);
}

PHP_METHOD(Stub_Issue1629, castStringOfChar)
{
	zval _0;
	char ch = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ch = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ch = ch;
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_STRINGL(&_0, &ch, 1);
	RETURN_CTOR(&_0);
}

PHP_METHOD(Stub_Issue1629, castStringOfOffset)
{
	unsigned char _0;
	zval out, _2;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _1;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_1);
	ZVAL_UNDEF(&out);
	ZVAL_UNDEF(&_2);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	_0 = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRINGL(&_1, &_0, 1);
	zephir_cast_to_string(&_2, &_1);
	ZEPHIR_CPY_WRT(&out, &_2);
	RETURN_CTOR(&out);
}

PHP_METHOD(Stub_Issue1629, castCharOfVar)
{
	char _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = (char) zephir_get_intval(value);
	RETURN_LONG((unsigned char) _0);
}

PHP_METHOD(Stub_Issue1629, castUcharOfVar)
{
	unsigned char _0;
	zval *value, value_sub;

	ZVAL_UNDEF(&value_sub);
	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value);
	_0 = (unsigned char) zephir_get_intval(value);
	RETURN_LONG((unsigned char) _0);
}

PHP_METHOD(Stub_Issue1629, ucharParam)
{
	zend_long out = 0;
	zval *value_param = NULL;
	unsigned char value;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(value_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &value_param);
	value = zephir_get_charval(value_param);
	out = value;
	RETURN_LONG(out);
}

PHP_METHOD(Stub_Issue1629, keyedCharArray)
{
	char ch = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL, _0;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZVAL_UNDEF(&_0);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	zephir_memory_observe(&keyspace_zv);
	ZVAL_STR_COPY(&keyspace_zv, keyspace);
	ch = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ch = ch;
	zephir_create_array(return_value, 2, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, ch);
	zephir_array_update_long(return_value, 1, &_0, PH_COPY ZEPHIR_DEBUG_PARAMS_DUMMY);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, ch);
	zephir_array_update_string(return_value, SL("k"), &_0, PH_COPY | PH_SEPARATE);
	RETURN_MM();
}

PHP_METHOD(Stub_Issue1629, doubleFromChar)
{
	double d = 0;
	char ch = 0;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&keyspace_zv, keyspace);
	ch = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ch = ch;
	d = (double) ch;
	RETURN_DOUBLE(d);
}

PHP_METHOD(Stub_Issue1629, boolFromChar)
{
	zend_bool b = 0;
	char ch = 0;
	zend_long nb;
	zval keyspace_zv, *nb_param = NULL;
	zend_string *keyspace = NULL;

	ZVAL_UNDEF(&keyspace_zv);
	ZEND_PARSE_PARAMETERS_START(2, 2)
		Z_PARAM_STR(keyspace)
		Z_PARAM_LONG(nb)
	ZEND_PARSE_PARAMETERS_END();
	nb_param = ZEND_CALL_ARG(execute_data, 2);
	ZVAL_STR(&keyspace_zv, keyspace);
	ch = ZEPHIR_STRING_OFFSET(&keyspace_zv, nb);
	ch = ch;
	b = ((ch) ? 1 : 0);
	RETURN_BOOL(b);
}

/**
 * A char cast assigned to a dynamic variable used to emit `ZVAL_LONG(&x, ''a'')`.
 */
PHP_METHOD(Stub_Issue1629, dynamicFromCharCast)
{
	zval dyn;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&dyn);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&dyn);
	ZVAL_STRING(&dyn, "seed");
	ZEPHIR_INIT_NVAR(&dyn);
	ZVAL_LONG(&dyn, 'a');
	RETURN_CCTOR(&dyn);
}

/**
 * A bare char literal returned directly used to emit `(unsigned char) (a)`.
 */
PHP_METHOD(Stub_Issue1629, returnCharLiteral)
{

	RETURN_LONG((unsigned char) ('a'));
}

PHP_METHOD(Stub_Issue1629, appendCharLiteral)
{
	zval _0;
	zval pieces;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&pieces);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&pieces);
	array_init(&pieces);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, 'a');
	zephir_array_append(&pieces, &_0, PH_SEPARATE, "stub/issue1629.zep", 229);
	RETURN_CTOR(&pieces);
}

zend_object *zephir_init_properties_Stub_Issue1629(zend_class_entry *class_type)
{
		zval _0, _1$$3;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1$$3);
	

		ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
		zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);
	
	{
		zval local_this_ptr, *this_ptr = &local_this_ptr;
		ZEPHIR_CREATE_OBJECT(this_ptr, class_type);
		zephir_read_property_ex(&_0, this_ptr, ZEND_STRL("parts"), PH_NOISY_CC | PH_READONLY);
		if (Z_TYPE_P(&_0) == IS_NULL) {
			ZEPHIR_INIT_VAR(&_1$$3);
			array_init(&_1$$3);
			zephir_update_property_zval_ex(this_ptr, ZEND_STRL("parts"), &_1$$3);
		}
		ZEPHIR_MM_RESTORE();
		return Z_OBJ_P(this_ptr);
	}
}

void zephir_init_static_properties_Stub_Issue1629()
{
	zval _0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;
		ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&_0);
	array_init(&_0);
	zephir_update_static_property_ce(stub_issue1629_ce, ZEND_STRL("staticParts"), &_0);
	ZEPHIR_MM_RESTORE();
}

