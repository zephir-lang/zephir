/*
  +------------------------------------------------------------------------+
  | Zephir Language                                                        |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2015 Zephir Team (http://www.zephir-lang.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@zephir-lang.com so we can send you a copy immediately.      |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@zephir-lang.com>                     |
  |          Eduar Carvajal <eduar@zephir-lang.com>                        |
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  +------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>

#include <php.h>
#include "php_ext.h"
#include "php_main.h"

#include <ext/standard/php_smart_string.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_rand.h>
#include <ext/standard/php_lcg.h>
#include <ext/standard/php_http.h>
#include "ext/standard/base64.h"
#include "ext/standard/md5.h"
#include "ext/standard/crc32.h"
#include "ext/standard/url.h"
#include "ext/standard/html.h"
#include "ext/date/php_date.h"

#ifdef ZEPHIR_USE_PHP_PCRE
#include <ext/pcre/php_pcre.h>
#endif

#ifdef ZEPHIR_USE_PHP_JSON
#include <ext/json/php_json.h>
#endif

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"

#define PH_RANDOM_ALNUM 0
#define PH_RANDOM_ALPHA 1
#define PH_RANDOM_HEXDEC 2
#define PH_RANDOM_NUMERIC 3
#define PH_RANDOM_NOZERO 4

/**
 * This function is not external in the Zend API so we redeclare it here in the extension
 */
int zephir_spprintf(char **message, int max_len, char *format, ...)
{
	va_list arg;
	int len;

	va_start(arg, format);
	len = vspprintf(message, max_len, format, arg);
	va_end(arg);
	return len;
}

/**
 * Fast call to php strlen
 */
int zephir_fast_strlen_ev(zval *str)
{
	zval copy;
	int use_copy = 0, length;

	if (Z_TYPE_P(str) != IS_STRING) {
		use_copy = zend_make_printable_zval(str, &copy);
		if (use_copy) {
			str = &copy;
		}
	}

	length = Z_STRLEN_P(str);
	if (use_copy) {
		zval_dtor(str);
	}

	return length;
}

/**
 * Fast call to php strtolower
 */
void zephir_fast_strtolower(zval *return_value, zval *str)
{
	zval copy;
	int use_copy = 0;
	char *lower_str;
	unsigned int length;

	if (Z_TYPE_P(str) != IS_STRING) {
		use_copy = zend_make_printable_zval(str, &copy);
		if (use_copy) {
			str = &copy;
		}
	}

	length = Z_STRLEN_P(str);
	lower_str = estrndup(Z_STRVAL_P(str), length);
	php_strtolower(lower_str, length);

	if (use_copy) {
		zval_dtor(str);
	}

	ZVAL_STRINGL(return_value, lower_str, length);
}

/**
 * Fast call to PHP strtoupper() function
 */
void zephir_fast_strtoupper(zval *return_value, zval *str)
{
	zval copy;
	int use_copy = 0;
	char *lower_str;
	unsigned int length;

	if (Z_TYPE_P(str) != IS_STRING) {
		use_copy = zend_make_printable_zval(str, &copy);
		if (use_copy) {
			str = &copy;
		}
	}

	length = Z_STRLEN_P(str);
	lower_str = estrndup(Z_STRVAL_P(str), length);
	php_strtoupper(lower_str, length);

	if (use_copy) {
		zval_dtor(str);
	}

	ZVAL_STRINGL(return_value, lower_str, length);
}

/**
 * Fast join function
 * This function is an adaption of the php_implode function
 *
 */
void zephir_fast_join_str(zval *return_value, char *glue, unsigned int glue_length, zval *pieces)
{
	zval         *tmp;
	int           numelems;
	zend_string  *str;
	char         *cptr;
	size_t        len = 0;
	zend_string **strings, **strptr;
	zend_array *arr;

	if (Z_TYPE_P(pieces) != IS_ARRAY) {
		php_error_docref(NULL, E_WARNING, "Invalid arguments supplied for fast_join()");
		RETURN_EMPTY_STRING();
	}

	arr = Z_ARRVAL_P(pieces);
	numelems = zend_hash_num_elements(arr);

	if (numelems == 0) {
		RETURN_EMPTY_STRING();
	} else if (numelems == 1) {
		/* loop to search the first not undefined element... */
		ZEND_HASH_FOREACH_VAL(arr, tmp) {
			RETURN_STR(zval_get_string(tmp));
		} ZEND_HASH_FOREACH_END();
	}

	strings = emalloc((sizeof(zend_long) + sizeof(zend_string *)) * numelems);
	strptr = strings - 1;

	ZEND_HASH_FOREACH_VAL(arr, tmp) {
		if (Z_TYPE_P(tmp) == IS_LONG) {
			double val = Z_LVAL_P(tmp);
			*++strptr = NULL;
			((zend_long *) (strings + numelems))[strptr - strings] = Z_LVAL_P(tmp);
			if (val < 0) {
				val = -10 * val;
			}
			if (val < 10) {
				len++;
			} else {
				len += (int) log10(10 * (double) val);
			}
		} else {
			*++strptr = zval_get_string(tmp);
			len += ZSTR_LEN(*strptr);
		}
	} ZEND_HASH_FOREACH_END();

	str = zend_string_alloc(len + (numelems - 1) * glue_length, 0);
	cptr = ZSTR_VAL(str) + ZSTR_LEN(str);
	*cptr = 0;

	do {
		if (*strptr) {
			cptr -= ZSTR_LEN(*strptr);
			memcpy(cptr, ZSTR_VAL(*strptr), ZSTR_LEN(*strptr));
			zend_string_release(*strptr);
		} else {
			char *oldPtr = cptr;
			char oldVal = *cptr;
			zend_long val = ((zend_long *) (strings + numelems))[strptr - strings];
			cptr = zend_print_long_to_buf(cptr, val);
			*oldPtr = oldVal;
		}

		cptr -= glue_length;
		memcpy(cptr, glue, glue_length);
	} while (--strptr > strings);

	if (*strptr) {
		memcpy(ZSTR_VAL(str), ZSTR_VAL(*strptr), ZSTR_LEN(*strptr));
		zend_string_release(*strptr);
	} else {
		char *oldPtr = cptr;
		char oldVal = *cptr;
		zend_print_long_to_buf(cptr, ((zend_long *) (strings + numelems))[strptr - strings]);
		*oldPtr = oldVal;
	}

	efree(strings);
	RETURN_NEW_STR(str);
}

/**
 * Immediate function resolution for strpos function
 */
void zephir_fast_strpos(zval *return_value, const zval *haystack, const zval *needle, unsigned int offset)
{
	const char *found = NULL;

	if (unlikely(Z_TYPE_P(haystack) != IS_STRING || Z_TYPE_P(needle) != IS_STRING)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Invalid arguments supplied for strpos()");
		return;
	}

	if (offset > Z_STRLEN_P(haystack)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Offset not contained in string");
		return;
	}

	if (!Z_STRLEN_P(needle)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Empty delimiter");
		return;
	}

	found = php_memnstr(Z_STRVAL_P(haystack)+offset, Z_STRVAL_P(needle), Z_STRLEN_P(needle), Z_STRVAL_P(haystack) + Z_STRLEN_P(haystack));

	if (found) {
		ZVAL_LONG(return_value, found - Z_STRVAL_P(haystack));
	} else {
		ZVAL_BOOL(return_value, 0);
	}
}

/**
 * Immediate function resolution for strpos function
 */
void zephir_fast_strpos_str(zval *return_value, const zval *haystack, char *needle, unsigned int needle_length)
{
	const char *found = NULL;

	if (unlikely(Z_TYPE_P(haystack) != IS_STRING)) {
		ZVAL_NULL(return_value);
		zend_error(E_WARNING, "Invalid arguments supplied for strpos()");
		return;
	}

	found = php_memnstr(Z_STRVAL_P(haystack), needle, needle_length, Z_STRVAL_P(haystack) + Z_STRLEN_P(haystack));

	if (found) {
		ZVAL_LONG(return_value, found - Z_STRVAL_P(haystack));
	} else {
		ZVAL_BOOL(return_value, 0);
	}
}

/**
 * Fast call to PHP trim() function
 */
void zephir_fast_trim(zval *return_value, zval *str, zval *charlist, int where)
{
	zval copy;
	int use_copy = 0;
	zend_string *trimmed;

	if (Z_TYPE_P(str) != IS_STRING) {
		use_copy = zend_make_printable_zval(str, &copy);
		if (use_copy) {
			str = &copy;
		}
	}

	if (charlist && Z_TYPE_P(charlist) == IS_STRING) {
		trimmed = php_trim(Z_STR_P(str), Z_STRVAL_P(charlist), Z_STRLEN_P(charlist), where);
	} else {
		trimmed = php_trim(Z_STR_P(str), NULL, 0, where);
	}
	ZVAL_STR(return_value, trimmed);

	if (use_copy) {
		zval_dtor(&copy);
	}
}

#ifdef ZEPHIR_USE_PHP_JSON

int zephir_json_encode(zval *return_value, zval *v, int opts)
{
	smart_str buf = { 0 };

	php_json_encode(&buf, v, opts);
	smart_str_0(&buf);
	ZVAL_STR(return_value, buf.s);

	return SUCCESS;
}

int zephir_json_decode(zval *return_value, zval *v, zend_bool assoc)
{
	zval copy;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(v) != IS_STRING)) {
		use_copy = zend_make_printable_zval(v, &copy);
		if (use_copy) {
			v = &copy;
		}
	}

	php_json_decode(return_value, Z_STRVAL_P(v), Z_STRLEN_P(v), assoc, 512 /* JSON_PARSER_DEFAULT_DEPTH */);

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}

	return SUCCESS;
}

#else

int zephir_json_encode(zval *return_value, zval *v, int opts)
{
	zval zopts;
	zval *params[2];

	ZEPHIR_SINIT_VAR(zopts);
	ZVAL_LONG(&zopts, opts);

	params[0] = v;
	params[1] = &zopts;

	return zephir_return_call_function(return_value, SL("json_encode"), NULL, 0, 2, params);
}

int zephir_json_decode(zval *return_value, zval *v, zend_bool assoc)
{
	zval zassoc;
	zval *params[2];

	ZEPHIR_SINIT_VAR(zassoc);
	ZVAL_BOOL(&zassoc, assoc);

	params[0] = v;
	params[1] = &zassoc;

	return zephir_return_call_function(return_value, SL("json_decode"), NULL, 0, 2, params);
}

#endif /* ZEPHIR_USE_PHP_JSON */

void zephir_md5(zval *return_value, zval *str)
{
	PHP_MD5_CTX ctx;
	unsigned char digest[16];
	char hexdigest[33];
	zval copy;
	int use_copy = 0;

	if (Z_TYPE_P(str) != IS_STRING) {
		use_copy = zend_make_printable_zval(str, &copy);
		if (use_copy) {
			str = &copy;
		}
	}

	PHP_MD5Init(&ctx);
	PHP_MD5Update(&ctx, Z_STRVAL_P(str), Z_STRLEN_P(str));
	PHP_MD5Final(digest, &ctx);

	make_digest(hexdigest, digest);

	ZVAL_STRINGL(return_value, hexdigest, 32);
}

void zephir_ucfirst(zval *return_value, zval *s)
{
	zval copy;
	char *c;
	int use_copy = 0;

	if (unlikely(Z_TYPE_P(s) != IS_STRING)) {
		use_copy = zend_make_printable_zval(s, &copy);
		if (use_copy) {
			s = &copy;
		}
	}

	if (!Z_STRLEN_P(s)) {
		ZVAL_EMPTY_STRING(return_value);
	}
	else {
		ZVAL_STRINGL(return_value, Z_STRVAL_P(s), Z_STRLEN_P(s));
		c = Z_STRVAL_P(return_value);
		*c = toupper((unsigned char)*c);
	}

	if (unlikely(use_copy)) {
		zval_dtor(&copy);
	}
}
