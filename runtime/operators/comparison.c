
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
#include "symtable.h"

#include "kernel/main.h"

zephir_compiled_expr *zephir_operator_comparison_greater(zephir_context *context, zval *expr TSRMLS_DC)
{
	zval *left_expr, *right_expr;
	zephir_compiled_expr *compiled_expr_left, *compiled_expr_right, *compiled_expr;

	_zephir_array_fetch_string(&left_expr, expr, SS("left") TSRMLS_CC);
	if (Z_TYPE_P(left_expr) != IS_ARRAY) {
		return NULL;
	}

	_zephir_array_fetch_string(&right_expr, expr, SS("right") TSRMLS_CC);
	if (Z_TYPE_P(right_expr) != IS_ARRAY) {
		return NULL;
	}

	compiled_expr_left = zephir_expr(context, left_expr TSRMLS_CC);
	compiled_expr_right = zephir_expr(context, right_expr TSRMLS_CC);

	switch (compiled_expr_left->type) {

		case ZEPHIR_T_TYPE_LONG:
		case ZEPHIR_T_TYPE_INTEGER:

			switch (compiled_expr_right->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:

					compiled_expr = emalloc(sizeof(zephir_compiled_expr));
					compiled_expr->type  = ZEPHIR_T_TYPE_BOOL;
					compiled_expr->value = LLVMBuildNSWAdd(context->builder, compiled_expr_left->value, compiled_expr_right->value, "");
					return compiled_expr;

			}
			break;

		case ZEPHIR_T_TYPE_VAR:

			switch (compiled_expr_left->variable->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:

					switch (compiled_expr_right->type) {

						case ZEPHIR_T_TYPE_LONG:
						case ZEPHIR_T_TYPE_INTEGER:

							compiled_expr = emalloc(sizeof(zephir_compiled_expr));
							compiled_expr->type  = ZEPHIR_T_TYPE_BOOL;
							compiled_expr->value = LLVMBuildICmp(
								context->builder,
								LLVMIntSGT,
								LLVMBuildLoad(context->builder, compiled_expr_left->variable->value_ref, ""),
								compiled_expr_right->value,
								""
							);
							return compiled_expr;

						case ZEPHIR_T_TYPE_VAR:

							switch (compiled_expr_right->variable->type) {

								case ZEPHIR_T_TYPE_LONG:
								case ZEPHIR_T_TYPE_INTEGER:

									compiled_expr = emalloc(sizeof(zephir_compiled_expr));
									compiled_expr->type  = ZEPHIR_T_INTEGER;
									compiled_expr->value = LLVMBuildNSWAdd(
										context->builder,
										LLVMBuildLoad(context->builder, compiled_expr_left->variable->value_ref, ""),
										LLVMBuildLoad(context->builder, compiled_expr_right->variable->value_ref, ""),
										""
									);
									return compiled_expr;

									break;

							}
							break;

					}
					break;

			}
			break;

	}

	zend_error(E_ERROR, "Unknown operands in 'greater' operation");
	return NULL;
}

zephir_compiled_expr *zephir_operator_comparison_less(zephir_context *context, zval *expr TSRMLS_DC)
{
	zval *left_expr, *right_expr;
	zephir_compiled_expr *compiled_expr_left, *compiled_expr_right, *compiled_expr;

	_zephir_array_fetch_string(&left_expr, expr, SS("left") TSRMLS_CC);
	if (Z_TYPE_P(left_expr) != IS_ARRAY) {
		return NULL;
	}

	_zephir_array_fetch_string(&right_expr, expr, SS("right") TSRMLS_CC);
	if (Z_TYPE_P(right_expr) != IS_ARRAY) {
		return NULL;
	}

	compiled_expr_left = zephir_expr(context, left_expr TSRMLS_CC);
	compiled_expr_right = zephir_expr(context, right_expr TSRMLS_CC);

	switch (compiled_expr_left->type) {

		case ZEPHIR_T_TYPE_LONG:
		case ZEPHIR_T_TYPE_INTEGER:

			switch (compiled_expr_right->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:

					compiled_expr = emalloc(sizeof(zephir_compiled_expr));
					compiled_expr->type  = ZEPHIR_T_TYPE_BOOL;
					compiled_expr->value = LLVMBuildNSWAdd(context->builder, compiled_expr_left->value, compiled_expr_right->value, "");
					return compiled_expr;

			}
			break;

		case ZEPHIR_T_TYPE_VAR:

			switch (compiled_expr_left->variable->type) {

				case ZEPHIR_T_TYPE_LONG:
				case ZEPHIR_T_TYPE_INTEGER:

					switch (compiled_expr_right->type) {

						case ZEPHIR_T_TYPE_LONG:
						case ZEPHIR_T_TYPE_INTEGER:

							compiled_expr = emalloc(sizeof(zephir_compiled_expr));
							compiled_expr->type  = ZEPHIR_T_TYPE_BOOL;
							compiled_expr->value = LLVMBuildICmp(
								context->builder,
								LLVMIntSLT,
								LLVMBuildLoad(context->builder, compiled_expr_left->variable->value_ref, ""),
								compiled_expr_right->value,
								""
							);
							return compiled_expr;

						case ZEPHIR_T_TYPE_VAR:

							switch (compiled_expr_right->variable->type) {

								case ZEPHIR_T_TYPE_LONG:
								case ZEPHIR_T_TYPE_INTEGER:

									compiled_expr = emalloc(sizeof(zephir_compiled_expr));
									compiled_expr->type  = ZEPHIR_T_TYPE_BOOL;
									compiled_expr->value = LLVMBuildICmp(
										context->builder,
										LLVMIntSLT,
										LLVMBuildLoad(context->builder, compiled_expr_left->variable->value_ref, ""),
										LLVMBuildLoad(context->builder, compiled_expr_right->variable->value_ref, ""),
										""
									);
									return compiled_expr;

								default:

									zend_error(E_ERROR, "Unknown operands in 'less' operation %d %d", compiled_expr_left->type, compiled_expr_right->variable->type);

							}
							break;

					}
					break;

			}
			break;

	}

	zend_error(E_ERROR, "Unknown operands in 'less' operation %d %d", compiled_expr_left->type, compiled_expr_right->type);
	return NULL;
}
