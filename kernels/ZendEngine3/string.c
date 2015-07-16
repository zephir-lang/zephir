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
