
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
#include "expr.h"
#include "builder.h"
#include "symtable.h"
#include "variable.h"
#include "kernel/main.h"

int zephir_statement_let_variable(zephir_context *context, zval *assignment, zval *statement TSRMLS_DC) {

	zval *expr, *variable;
	zephir_compiled_expr *compiled_expr;
	zephir_variable *symbol_variable;

	_zephir_array_fetch_string(&expr, assignment, SS("expr") TSRMLS_CC);
	if (Z_TYPE_P(expr) != IS_ARRAY) {
		return 0;
	}

	_zephir_array_fetch_string(&variable, assignment, SS("variable") TSRMLS_CC);
	if (Z_TYPE_P(variable) != IS_STRING) {
		return 0;
	}

	compiled_expr = zephir_expr(context, expr TSRMLS_CC);

	symbol_variable = zephir_symtable_get_variable_for_write(context->symtable, Z_STRVAL_P(variable), Z_STRLEN_P(variable));
	symbol_variable->initialized = 1;

	switch (symbol_variable->type) {

		case ZEPHIR_T_TYPE_BOOL:

			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_BOOL:
					LLVMBuildStore(context->builder, compiled_expr->value, symbol_variable->value_ref);
					break;

			}
			break;

		case ZEPHIR_T_TYPE_LONG:
		case ZEPHIR_T_TYPE_INTEGER:

			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:
					LLVMBuildStore(context->builder, compiled_expr->value, symbol_variable->value_ref);
					break;

				case ZEPHIR_T_TYPE_VAR:

					switch (compiled_expr->variable->type) {

						case ZEPHIR_T_TYPE_LONG:
						case ZEPHIR_T_TYPE_INTEGER:
							LLVMBuildStore(context->builder, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), symbol_variable->value_ref);
							break;
					}
					break;
			}
			break;

		case ZEPHIR_T_TYPE_DOUBLE:

			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_DOUBLE:
					LLVMBuildStore(context->builder, compiled_expr->value, symbol_variable->value_ref);
					break;

				case ZEPHIR_T_TYPE_VAR:

					switch (compiled_expr->variable->type) {

						case ZEPHIR_T_TYPE_DOUBLE:
							LLVMBuildStore(context->builder, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), symbol_variable->value_ref);
							break;
					}
					break;
			}

			break;

		case ZEPHIR_T_TYPE_VAR:

			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_NULL:
					zephir_variable_init_variant(symbol_variable, context);
					break;

				case ZEPHIR_T_TYPE_BOOL:
					zephir_variable_init_variant(symbol_variable, context);
					zephir_build_zval_bool(context, symbol_variable->value_ref, compiled_expr->value);
					break;

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:
					zephir_variable_init_variant(symbol_variable, context);
					zephir_build_zval_long(context, symbol_variable->value_ref, compiled_expr->value);
					break;

				case ZEPHIR_T_TYPE_DOUBLE:
					zephir_variable_init_variant(symbol_variable, context);
					zephir_build_zval_double(context, symbol_variable->value_ref, compiled_expr->value);
					break;

				case ZEPHIR_T_TYPE_VAR:

					switch (compiled_expr->variable->type) {

						case ZEPHIR_T_TYPE_BOOL:
							zephir_variable_init_variant(symbol_variable, context);
							zephir_build_zval_bool(context, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), compiled_expr->value);
							break;

						case ZEPHIR_T_TYPE_LONG:
						case ZEPHIR_T_TYPE_INTEGER:
							zephir_variable_init_variant(symbol_variable, context);
							zephir_build_zval_long(context, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), compiled_expr->value);
							break;

						case ZEPHIR_T_TYPE_DOUBLE:
							zephir_variable_init_variant(symbol_variable, context);
							zephir_build_zval_double(context, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), compiled_expr->value);
							break;

						case ZEPHIR_T_TYPE_VAR:
							zephir_build_copy_on_write(context, symbol_variable->value_ref, compiled_expr->variable->value_ref);
							break;

						default:
							zend_error(E_ERROR, "Unknown let variable %d", compiled_expr->variable->type);
					}
					break;
			}

			break;

		default:
			zend_error(E_ERROR, "Unknown let variable %d", symbol_variable->type);

	}

	efree(compiled_expr);
	return 0;
}

/**
 * Increments a variable
 */
int zephir_statement_let_incr(zephir_context *context, zval *assignment, zval *statement TSRMLS_DC) {

	zval *variable;
	zephir_variable *symbol_variable;

	_zephir_array_fetch_string(&variable, assignment, SS("variable") TSRMLS_CC);
	if (Z_TYPE_P(variable) != IS_STRING) {
		return 0;
	}

	symbol_variable = zephir_symtable_get_variable_for_write(context->symtable, Z_STRVAL_P(variable), Z_STRLEN_P(variable));

	switch (symbol_variable->type) {

		case ZEPHIR_T_TYPE_LONG:
		case ZEPHIR_T_TYPE_INTEGER:

			LLVMBuildStore(context->builder, // store i32 %7, i32* %a, align 4
				LLVMBuildNSWAdd(
					context->builder,
					LLVMBuildLoad(context->builder, symbol_variable->value_ref, ""), // %6 = load i32* %a, align 4
#if ZEPHIR_32
					LLVMConstInt(LLVMInt32Type(), 1, 0), // add nsw i32 %6, 1
#else
					LLVMConstInt(LLVMInt64Type(), 1, 0), // add nsw i64 %6, 1
#endif
					""
				),
				symbol_variable->value_ref
			);

			break;

	}

	return 0;
}

/**
 * Decrements a variable
 */
int zephir_statement_let_decr(zephir_context *context, zval *assignment, zval *statement TSRMLS_DC) {

	zval *variable;
	zephir_variable *symbol_variable;

	_zephir_array_fetch_string(&variable, assignment, SS("variable") TSRMLS_CC);
	if (Z_TYPE_P(variable) != IS_STRING) {
		return 0;
	}

	symbol_variable = zephir_symtable_get_variable_for_write(context->symtable, Z_STRVAL_P(variable), Z_STRLEN_P(variable));

	switch (symbol_variable->type) {

		case ZEPHIR_T_TYPE_LONG:
		case ZEPHIR_T_TYPE_INTEGER:

			LLVMBuildStore(context->builder, // store i32 %7, i32* %a, align 4
				LLVMBuildNSWAdd( // // %7 = add nsw i32 %6, -1
					context->builder,
					LLVMBuildLoad(context->builder, symbol_variable->value_ref, ""), // %6 = load i32* %a, align 4
#if ZEPHIR_32
					LLVMConstInt(LLVMInt32Type(), -1, 0),
#else
					LLVMConstInt(LLVMInt64Type(), -1, 0),
#endif
					""
				),
				symbol_variable->value_ref
			);

			break;

	}

	return 0;
}

/**
 * Assigns values to variables, array indices, object properties, etc.
 *
 */
int zephir_statement_let(zephir_context *context, zval *statement TSRMLS_DC)
{
	HashTable       *ht;
	HashPosition    pos = {0};
	zval *assignment, **assignment, *assign_type;

	_zephir_array_fetch_string(&assignment, statement, SS("assignments") TSRMLS_CC);
	if (Z_TYPE_P(assignment) != IS_ARRAY) {
		return 0;
	}

	ht = Z_ARRVAL_P(assignment);
	zend_hash_internal_pointer_reset_ex(ht, &pos);
	for (
	 ; zend_hash_get_current_data_ex(ht, (void**) &assignment, &pos) == SUCCESS
	 ; zend_hash_move_forward_ex(ht, &pos)
	) {

		_zephir_array_fetch_string(&assign_type, *assignment, SS("assign-type") TSRMLS_CC);
		if (Z_TYPE_P(assign_type) != IS_STRING) {
			return 0;
		}

		if (!memcmp(Z_STRVAL_P(assign_type), SS("variable"))) {
			zephir_statement_let_variable(context, *assignment, statement);
			continue;
		}

		if (!memcmp(Z_STRVAL_P(assign_type), SS("incr"))) {
			zephir_statement_let_incr(context, *assignment, statement);
			continue;
		}

		if (!memcmp(Z_STRVAL_P(assign_type), SS("decr"))) {
			zephir_statement_let_decr(context, *assignment, statement);
			continue;
		}

		zend_print_zval_r(*assignment, 0 TSRMLS_CC);
	}

	return 0;
}
