
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

#include "kernel/main.h"

int zephir_statement_declare(zephir_context *context, zval *statement TSRMLS_DC)
{
	zval *data_type, *variables, **variable, *name, *default_value;
	HashTable       *ht;
	HashPosition    pos = {0};
	zephir_variable *symbol;
	zephir_compiled_expr *compiled_expr;

	_zephir_array_fetch_string(&data_type, statement, SS("data-type") TSRMLS_CC);
	if (Z_TYPE_P(data_type) != IS_STRING) {
		return 0;
	}

	_zephir_array_fetch_string(&variables, statement, SS("variables") TSRMLS_CC);
	if (Z_TYPE_P(variables) != IS_ARRAY) {
		return 0;
	}

	ht = Z_ARRVAL_P(variables);
	zend_hash_internal_pointer_reset_ex(ht, &pos);
	for (
	 ; zend_hash_get_current_data_ex(ht, (void**) &variable, &pos) == SUCCESS
	 ; zend_hash_move_forward_ex(ht, &pos)
	) {

		_zephir_array_fetch_string(&name, *variable, SS("variable") TSRMLS_CC);
		if (Z_TYPE_P(name) != IS_STRING) {
			return 0;
		}

		if (zephir_symtable_has(Z_STRVAL_P(name), Z_STRLEN_P(name), context)) {
			zend_error(E_ERROR, "Variable \"%s\" is already defined", Z_STRVAL_P(name));
		}

		if (!memcmp(Z_STRVAL_P(data_type), SS("variable"))) {

			symbol = zephir_symtable_add(ZEPHIR_T_TYPE_VAR, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
			symbol->value_ref = LLVMBuildAlloca(context->builder, context->types.zval_pointer_type, Z_STRVAL_P(name));

			// store %struct._zval_struct* null, %struct._zval_struct** %x, align 8
			LLVMBuildStore(context->builder, LLVMConstPointerNull(context->types.zval_pointer_type), symbol->value_ref);

			/**
			 * Assign default value if any
			 */
			_zephir_array_fetch_string(&default_value, *variable, SS("expr") TSRMLS_CC);
			if (Z_TYPE_P(default_value) != IS_ARRAY) {
				continue;
			}

			compiled_expr = zephir_expr(context, default_value TSRMLS_CC);
			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:
					//LLVMBuildStore(context->builder, LLVMBuildSIToFP(context->builder, compiled_expr->value, LLVMDoubleType(), ""), symbol->value_ref); // sitofp i64 %2 to double
					break;

				case ZEPHIR_T_TYPE_DOUBLE:
					//LLVMBuildStore(context->builder, compiled_expr->value, symbol->value_ref);
					break;

				default:
					zend_error(E_ERROR, "Wrong default type for variant type");
					break;
			}

			continue;
		}

		if (!memcmp(Z_STRVAL_P(data_type), SS("int"))) {
			symbol = zephir_symtable_add(ZEPHIR_T_TYPE_INTEGER, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
#if ZEPHIR_32
			symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt32Type(), Z_STRVAL_P(name));
#else
			symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt64Type(), Z_STRVAL_P(name));
#endif

			/**
			 * Assign default value if any
			 */
			_zephir_array_fetch_string(&default_value, *variable, SS("expr") TSRMLS_CC);
			if (Z_TYPE_P(default_value) != IS_ARRAY) {
				continue;
			}

			compiled_expr = zephir_expr(context, default_value TSRMLS_CC);
			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:
					LLVMBuildStore(context->builder, compiled_expr->value, symbol->value_ref);
					break;

				default:
					zend_error(E_ERROR, "Wrong default type for double type");
					break;
			}

			continue;
		}

		if (!memcmp(Z_STRVAL_P(data_type), SS("long"))) {

			symbol = zephir_symtable_add(ZEPHIR_T_TYPE_LONG, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
#if ZEPHIR_32
			symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt32Type(), Z_STRVAL_P(name));
#else
			symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt64Type(), Z_STRVAL_P(name));
#endif

			/**
			 * Assign default value if any
			 */
			_zephir_array_fetch_string(&default_value, *variable, SS("expr") TSRMLS_CC);
			if (Z_TYPE_P(default_value) != IS_ARRAY) {
				continue;
			}

			compiled_expr = zephir_expr(context, default_value TSRMLS_CC);
			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:
					LLVMBuildStore(context->builder, compiled_expr->value, symbol->value_ref);
					break;

				default:
					zend_error(E_ERROR, "Wrong default type for double type");
					break;
			}

			continue;
		}

		if (!memcmp(Z_STRVAL_P(data_type), SS("double"))) {

			symbol = zephir_symtable_add(ZEPHIR_T_TYPE_DOUBLE, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
			symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMDoubleType(), Z_STRVAL_P(name));

			/**
			 * Assign default value if any
			 */
			_zephir_array_fetch_string(&default_value, *variable, SS("expr") TSRMLS_CC);
			if (Z_TYPE_P(default_value) != IS_ARRAY) {
				continue;
			}

			compiled_expr = zephir_expr(context, default_value TSRMLS_CC);
			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:
					LLVMBuildStore(context->builder, LLVMBuildSIToFP(context->builder, compiled_expr->value, LLVMDoubleType(), ""), symbol->value_ref); // sitofp i64 %2 to double
					break;

				case ZEPHIR_T_TYPE_DOUBLE:
					LLVMBuildStore(context->builder, compiled_expr->value, symbol->value_ref);
					break;

				default:
					zend_error(E_ERROR, "Wrong default type for double type");
					break;
			}

			continue;
		}

		if (!memcmp(Z_STRVAL_P(data_type), SS("bool"))) {

			symbol = zephir_symtable_add(ZEPHIR_T_TYPE_BOOL, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
			symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt8Type(), Z_STRVAL_P(name));

			/**
			 * Assign default value if any
			 */
			_zephir_array_fetch_string(&default_value, *variable, SS("expr") TSRMLS_CC);
			if (Z_TYPE_P(default_value) != IS_ARRAY) {
				continue;
			}

			compiled_expr = zephir_expr(context, default_value TSRMLS_CC);
			switch (compiled_expr->type) {

				case ZEPHIR_T_TYPE_BOOL:
					LLVMBuildStore(context->builder, compiled_expr->value, symbol->value_ref);
					break;

				default:
					zend_error(E_ERROR, "Wrong default type for boolean type");
					break;
			}

			continue;
		}

		zend_print_zval_r(*variable, 0 TSRMLS_CC);
	}

	return 0;
}
