
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

#include <php.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_math.h>

#include "php_ext.h"
#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/string.h"
#include "kernel/operators.h"

#include "Zend/zend_operators.h"

/**
 * Appends the content of the right operator to the left operator
 */
void zephir_concat_self_str(zval *left, const char *right, int right_length)
{
	zval left_copy;
	uint length, left_length;
	int use_copy = 0;
	zend_string *target;

	if (Z_TYPE_P(left) == IS_NULL) {
		ZVAL_STRINGL(left, right, right_length);
		return;
	}

	if (Z_TYPE_P(left) != IS_STRING) {
		use_copy = zephir_make_printable_zval(left, &left_copy);
		if (use_copy) {
			ZEPHIR_CPY_WRT_CTOR(left, (&left_copy));
		}
	}

	SEPARATE_ZVAL_IF_NOT_REF(left);
	left_length = Z_STRLEN_P(left);
	length = left_length + right_length;
	target = zend_string_extend(Z_STR_P(left), length, 0);
	ZVAL_NEW_STR(left, target);
	memcpy(ZSTR_VAL(target) + left_length, right, right_length);
	ZSTR_VAL(target)[length] = '\0';

	if (use_copy) {
		zval_dtor(&left_copy);
	}
}
