
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

/**
 * Natural compare with long operandus on right
 */
int zephir_compare_strict_long(zval *op1, long op2)
{
	switch (Z_TYPE_P(op1)) {
		case IS_LONG:
			return Z_LVAL_P(op1) == op2;
		case IS_DOUBLE:
			return Z_DVAL_P(op1) == (double) op2;
		case IS_NULL:
			return 0 == op2;
		case IS_TRUE:
		case IS_FALSE:
			if (Z_TYPE_P(op1) == IS_TRUE) {
				return 1 == op2;
			} else {
				return 0 == op2;
			}
		default:
			{
				zval result, op2_tmp;
				ZVAL_LONG(&op2_tmp, op2);
				is_equal_function(&result, op1, &op2_tmp);
				return Z_TYPE(result) == IS_TRUE ? 1 : 0;
			}
	}

	return 0;
}

/**
 * Check if two zvals are equal
 */
int zephir_is_equal(zval *op1, zval *op2)
{
	zval result;

	is_equal_function(&result, op1, op2);
	return Z_TYPE(result) == IS_TRUE;
}
