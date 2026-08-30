
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
#include "kernel/object.h"
#include "kernel/operators.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/array.h"


/**
 * Chars specific tests
 */
ZEPHIR_INIT_CLASS(Stub_Chars)
{
	ZEPHIR_REGISTER_CLASS(Stub, Chars, stub, chars, stub_chars_method_entry, 0);

	return SUCCESS;
}

PHP_METHOD(Stub_Chars, sumChars1)
{
	char ch, chlower = 0;
	ch = 'A';
	chlower = (ch + 32);
	RETURN_LONG((unsigned char) chlower);
}

PHP_METHOD(Stub_Chars, sumChars2)
{
	zval *ch_param = NULL;
	char ch, chlower = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ch_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &ch_param);
	ch = zephir_get_charval(ch_param);
	chlower = (ch + 32);
	RETURN_LONG((unsigned char) chlower);
}

PHP_METHOD(Stub_Chars, diffChars1)
{
	char ch, chlower = 0;
	ch = 'a';
	chlower = (ch - 32);
	RETURN_LONG((unsigned char) chlower);
}

PHP_METHOD(Stub_Chars, diffChars2)
{
	zval *ch_param = NULL;
	char ch, chlower = 0;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ch_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &ch_param);
	ch = zephir_get_charval(ch_param);
	chlower = (ch - 32);
	RETURN_LONG((unsigned char) chlower);
}

/**
 * Issue #1291: char arithmetic must behave as a byte (uchar),
 * without sign extension when the value exceeds 127.
 */
PHP_METHOD(Stub_Chars, sumOverflowHex)
{
	zval _0, _1;
	char ch;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&_0);
	ZVAL_UNDEF(&_1);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ch = ' ';
	ch += 64;
	ch += 42;
	ZEPHIR_INIT_VAR(&_0);
	ZEPHIR_INIT_VAR(&_1);
	ZVAL_STRINGL(&_1, &ch, 1);
	zephir_string_to_hex(&_0, &_1);
	RETURN_CCTOR(&_0);
}

PHP_METHOD(Stub_Chars, sumOverflowValue)
{
	char ch;
	ch = ' ';
	ch += 64;
	ch += 42;
	RETURN_LONG((unsigned char) ch);
}

PHP_METHOD(Stub_Chars, sumCharParamOverflow)
{
	zval *ch_param = NULL;
	char ch;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_ZVAL(ch_param)
	ZEND_PARSE_PARAMETERS_END();
	zephir_fetch_params_without_memory_grow(1, 0, &ch_param);
	ch = zephir_get_charval(ch_param);
	ch += 100;
	RETURN_LONG((unsigned char) ch);
}

PHP_METHOD(Stub_Chars, charUnderflow)
{
	char ch;
	ch = 0;
	ch -= 1;
	RETURN_LONG((unsigned char) ch);
}

PHP_METHOD(Stub_Chars, maxByteValue)
{
	char ch;
	ch = 0;
	ch += 255;
	RETURN_LONG((unsigned char) ch);
}

PHP_METHOD(Stub_Chars, sumUchar)
{
	unsigned char ch;
	ch = 'd';
	ch += 100;
	RETURN_LONG((unsigned char) ch);
}

/**
 * Issue #1988: char-typed variables used as array literal items
 * must be boxed as their byte (integer) value instead of raising
 * "Unknown char" at compile time.
 */
PHP_METHOD(Stub_Chars, arrayOfChars)
{
	zval _0;
	zval name;
	char ch1 = 0, ch2 = 0;
	zephir_method_globals *ZEPHIR_METHOD_GLOBALS_PTR = NULL;

	ZVAL_UNDEF(&name);
	ZVAL_UNDEF(&_0);
	ZEPHIR_METHOD_GLOBALS_PTR = pecalloc(1, sizeof(zephir_method_globals), 0);
	zephir_memory_grow_stack(ZEPHIR_METHOD_GLOBALS_PTR, __func__);

	ZEPHIR_INIT_VAR(&name);
	ZVAL_STRING(&name, "peter");
	ch1 = zephir_string_offset_byte(&name, 0, PH_NOISY);
	ch1 = ch1;
	ch2 = zephir_string_offset_byte(&name, 1, PH_NOISY);
	ch2 = ch2;
	zephir_create_array(return_value, 3, 0);
	ZEPHIR_INIT_VAR(&_0);
	ZVAL_LONG(&_0, ch1);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, ch2);
	zephir_array_fast_append(return_value, &_0);
	ZEPHIR_INIT_NVAR(&_0);
	ZVAL_LONG(&_0, ch1);
	zephir_array_fast_append(return_value, &_0);
	RETURN_MM();
}

