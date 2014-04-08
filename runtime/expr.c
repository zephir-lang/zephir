
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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

zephir_compiled_expr *zephir_expr(zephir_context *context, zval *expr TSRMLS_DC)
{
	zval *type, *value;
	zephir_compiled_expr *compiled_expr;
	zephir_variable *variable;

	_zephir_array_fetch_string(&type, expr, "type", strlen("type") + 1 TSRMLS_CC);
	if (Z_TYPE_P(type) != IS_STRING) {
		return NULL;
	}

	if (!memcmp(Z_STRVAL_P(type), "int", strlen("int") + 1)) {

		_zephir_array_fetch_string(&value, expr, "value", strlen("value") + 1 TSRMLS_CC);
		if (Z_TYPE_P(value) != IS_STRING) {
			return NULL;
		}

		compiled_expr = emalloc(sizeof(zephir_compiled_expr));
		compiled_expr->type  = ZEPHIR_T_INTEGER;
#if ZEPHIR_32
		compiled_expr->value = LLVMConstInt(LLVMInt32Type(), strtol(Z_STRVAL_P(value), NULL, 10), 0);
#else
		compiled_expr->value = LLVMConstInt(LLVMInt64Type(), strtol(Z_STRVAL_P(value), NULL, 10), 0);
#endif

		return compiled_expr;
	}

	if (!memcmp(Z_STRVAL_P(type), "variable", strlen("variable") + 1)) {

		_zephir_array_fetch_string(&value, expr, "value", strlen("value") + 1 TSRMLS_CC);
		if (Z_TYPE_P(value) != IS_STRING) {
			return NULL;
		}

		variable = zephir_symtable_get_variable_for_read(context->symtable, Z_STRVAL_P(value), Z_STRLEN_P(value));

		compiled_expr = emalloc(sizeof(zephir_compiled_expr));
		compiled_expr->type  = ZEPHIR_T_VARIABLE;
		compiled_expr->variable = variable;

		return compiled_expr;
	}

	zend_error(E_ERROR, "Unknown expression %s", Z_STRVAL_P(type));
	return NULL;
}
