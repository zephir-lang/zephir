
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php_zephir.h"
#include "zephir.h"
#include "utils.h"
#include "symtable.h"
#include "errors.h"
#include "fcall.h"

#include "kernel/main.h"

#include "operators/arithmetical.h"
#include "operators/comparison.h"

zephir_compiled_expr *zephir_expr(zephir_context *context, zval *expr TSRMLS_DC)
{
	zval *type, *value, *left_expr;
	zephir_compiled_expr *compiled_expr, *compiled_expr_left;
	zephir_variable *variable;

	_zephir_array_fetch_string(&type, expr, "type", strlen("type") + 1 TSRMLS_CC);
	if (Z_TYPE_P(type) != IS_STRING) {
		zephir_error(expr, "Expression is corrupt");
	}

	if (!memcmp(Z_STRVAL_P(type), SS("bool"))) {

		_zephir_array_fetch_string(&value, expr, SS("value") TSRMLS_CC);
		if (Z_TYPE_P(value) != IS_STRING) {
			return NULL;
		}

		compiled_expr = emalloc(sizeof(zephir_compiled_expr));
		compiled_expr->type  = ZEPHIR_T_TYPE_BOOL;
		if (!memcmp(Z_STRVAL_P(value), SS("true"))) {
			compiled_expr->value = LLVMConstInt(LLVMInt8Type(), 1, 0);
		} else {
			compiled_expr->value = LLVMConstInt(LLVMInt8Type(), 0, 0);
		}
		return compiled_expr;
	}

	if (!memcmp(Z_STRVAL_P(type), SS("int"))) {

		_zephir_array_fetch_string(&value, expr, SS("value") TSRMLS_CC);
		if (Z_TYPE_P(value) != IS_STRING) {
			return NULL;
		}

		compiled_expr = emalloc(sizeof(zephir_compiled_expr));
		compiled_expr->type  = ZEPHIR_T_TYPE_INTEGER;
#if ZEPHIR_32
		compiled_expr->value = LLVMConstInt(LLVMInt32Type(), strtol(Z_STRVAL_P(value), NULL, 10), 0);
#else
		compiled_expr->value = LLVMConstInt(LLVMInt64Type(), strtol(Z_STRVAL_P(value), NULL, 10), 0);
#endif

		return compiled_expr;
	}

	if (!memcmp(Z_STRVAL_P(type), SS("double"))) {

		_zephir_array_fetch_string(&value, expr, SS("value") TSRMLS_CC);
		if (Z_TYPE_P(value) != IS_STRING) {
			return NULL;
		}

		compiled_expr = emalloc(sizeof(zephir_compiled_expr));
		compiled_expr->type  = ZEPHIR_T_TYPE_DOUBLE;
		compiled_expr->value = LLVMConstReal(LLVMDoubleType(), zend_strtod(Z_STRVAL_P(value), NULL));

		return compiled_expr;
	}

	if (!memcmp(Z_STRVAL_P(type), SS("string"))) {

		_zephir_array_fetch_string(&value, expr, SS("value") TSRMLS_CC);
		if (Z_TYPE_P(value) != IS_STRING) {
			return NULL;
		}

		compiled_expr = emalloc(sizeof(zephir_compiled_expr));
		compiled_expr->type  = ZEPHIR_T_TYPE_STRING;
		compiled_expr->value = LLVMBuildGlobalStringPtr(context->builder, Z_STRVAL_P(value), "");

		return compiled_expr;
	}

	if (!memcmp(Z_STRVAL_P(type), SS("variable"))) {

		_zephir_array_fetch_string(&value, expr, SS("value") TSRMLS_CC);
		if (Z_TYPE_P(value) != IS_STRING) {
			return NULL;
		}

		variable = zephir_symtable_get_variable_for_read(context->symtable, Z_STRVAL_P(value), Z_STRLEN_P(value), context, expr);

		compiled_expr = emalloc(sizeof(zephir_compiled_expr));
		compiled_expr->type  = ZEPHIR_T_TYPE_VAR;
		compiled_expr->variable = variable;

		return compiled_expr;
	}

	if (!memcmp(Z_STRVAL_P(type), SS("add"))) {
		return zephir_operator_arithmetical_add(context, expr TSRMLS_CC);
	}

	if (!memcmp(Z_STRVAL_P(type), SS("mul"))) {
		return zephir_operator_arithmetical_mul(context, expr TSRMLS_CC);
	}

	if (!memcmp(Z_STRVAL_P(type), SS("div"))) {
		return zephir_operator_arithmetical_div(context, expr TSRMLS_CC);
	}

	if (!memcmp(Z_STRVAL_P(type), SS("greater"))) {
		return zephir_operator_comparison_greater(context, expr TSRMLS_CC);
	}

	if (!memcmp(Z_STRVAL_P(type), SS("less"))) {
		return zephir_operator_comparison_less(context, expr TSRMLS_CC);
	}

	if (!memcmp(Z_STRVAL_P(type), SS("fcall"))) {
		return zephir_fcall_compile(context, expr TSRMLS_CC);
	}

	if (!memcmp(Z_STRVAL_P(type), SS("list"))) {

		_zephir_array_fetch_string(&left_expr, expr, SS("left") TSRMLS_CC);
		if (Z_TYPE_P(left_expr) != IS_ARRAY) {
			return NULL;
		}

		return zephir_expr(context, left_expr TSRMLS_CC);
	}

	zephir_error(expr, "Unknown expression \"%s\"", Z_STRVAL_P(type));
	return NULL;
}
