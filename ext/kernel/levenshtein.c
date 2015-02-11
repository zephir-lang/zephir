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

#include <ext/standard/php_smart_str.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_rand.h>
#include <ext/standard/php_lcg.h>
#include <ext/standard/php_http.h>
#include "ext/standard/base64.h"
#include "ext/standard/md5.h"
#include "ext/standard/url.h"
#include "ext/standard/html.h"
#include "ext/date/php_date.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/operators.h"
#include "kernel/fcall.h"

#define LEVENSHTEIN_MAX_LENGTH 255

/* {{{ reference_levdist
 * reference implementation, only optimized for memory usage, not speed */
static zend_long reference_levdist(const char *s1, size_t l1, const char *s2, size_t l2, zend_long cost_ins, zend_long cost_rep, zend_long cost_del )
{
	zend_long *p1, *p2, *tmp;
	zend_long c0, c1, c2;
	size_t i1, i2;

	if (l1 == 0) {
		return l2 * cost_ins;
	}
	if (l2 == 0) {
		return l1 * cost_del;
	}

	if ((l1 > LEVENSHTEIN_MAX_LENGTH) || (l2 > LEVENSHTEIN_MAX_LENGTH)) {
		return -1;
	}
	p1 = safe_emalloc((l2 + 1), sizeof(zend_long), 0);
	p2 = safe_emalloc((l2 + 1), sizeof(zend_long), 0);

	for (i2 = 0; i2 <= l2; i2++) {
		p1[i2] = i2 * cost_ins;
	}
	for (i1 = 0; i1 < l1 ; i1++) {
		p2[0] = p1[0] + cost_del;

		for (i2 = 0; i2 < l2; i2++) {
			c0 = p1[i2] + ((s1[i1] == s2[i2]) ? 0 : cost_rep);
			c1 = p1[i2 + 1] + cost_del;
			if (c1 < c0) {
				c0 = c1;
			}
			c2 = p2[i2] + cost_ins;
			if (c2 < c0) {
				c0 = c2;
			}
			p2[i2 + 1] = c0;
		}
		tmp = p1;
		p1 = p2;
		p2 = tmp;
	}
	c0 = p1[l2];

	efree(p1);
	efree(p2);

	return c0;
}
/* }}} */

/* {{{ custom_levdist
 */
static int custom_levdist(char *str1, char *str2, char *callback_name)
{
	php_error_docref(NULL, E_WARNING, "The general Levenshtein support is not there yet");
	/* not there yet */

	return -1;
}
/* }}} */

void zephir_levenshtein(zval *str1, zval *str2 ZEPHIR_DEBUG_PARAMS) {

    char *str1, *str2;
    size_t str1_len, str2_len;

    str1_len = sizeof(str1);
    str2_len = sizeof(str2);

    zend_long cost_ins, cost_rep, cost_del;
    zend_long distance = -1;

    distance = reference_levdist(str1, str1_len, str2, str2_len, 1, 1, 1);

	if (distance < 0 && /* TODO */ ZEND_NUM_ARGS() != 3) {
		php_error_docref(NULL, E_WARNING, "Argument string(s) too long");
	}

    RETURN_LONG(distance);
}
