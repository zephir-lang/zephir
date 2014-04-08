
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
#include "expr.h"
#include "builder.h"
#include "blocks.h"

#include "kernel/main.h"

int zephir_statement_while(zephir_context *context, zval *statement TSRMLS_DC)
{

	zval *expr, *statements;
	zephir_compiled_expr *compiled_expr;
	LLVMValueRef condition = NULL, then_value, else_value;

	_zephir_array_fetch_string(&expr, statement, SS("expr") TSRMLS_CC);
	if (Z_TYPE_P(expr) != IS_ARRAY) {
		return 0;
	}

	compiled_expr = zephir_expr(context, expr TSRMLS_CC);
	switch (compiled_expr->type) {

		case ZEPHIR_T_TYPE_BOOL:
			condition = compiled_expr->value;
			break;

		case ZEPHIR_T_TYPE_INTEGER:
#if ZEPHIR_32
			condition = LLVMBuildICmp(context->builder, LLVMIntNE, compiled_expr->value, LLVMConstInt(LLVMInt32Type(), 0, 0), "");
#else
			condition = LLVMBuildICmp(context->builder, LLVMIntNE, compiled_expr->value, LLVMConstInt(LLVMInt64Type(), 0, 0), "");
#endif
			break;

		case ZEPHIR_T_VARIABLE:

			switch (compiled_expr->variable->type) {

				case ZEPHIR_T_TYPE_VAR:
					condition = LLVMBuildICmp(context->builder, LLVMIntNE, zephir_build_zend_is_true(context, compiled_expr->variable->value_ref), LLVMConstInt(LLVMInt32Type(), 0, 0), "");
					break;

			}

	}

	if (!condition) {
		zend_error(E_ERROR, "Unknown expression");
		return 0;
	}

	/*LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(func, "then");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(func, "else");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-if");

	LLVMBuildCondBr(context->builder, condition, then_block, else_block);

	LLVMPositionBuilderAtEnd(context->builder, then_block);
	_zephir_array_fetch_string(&statements, statement, "statements", strlen("statements") + 1 TSRMLS_CC);
	if (Z_TYPE_P(statements) == IS_ARRAY) {
		zephir_compile_block(context, statements);
	}
	LLVMBuildBr(context->builder, merge_block);
	then_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, else_block);
	_zephir_array_fetch_string(&statements, statement, "else_statements", strlen("else_statements") + 1 TSRMLS_CC);
	if (Z_TYPE_P(statements) == IS_ARRAY) {
		zephir_compile_block(context, statements);
	}
	LLVMBuildBr(context->builder, merge_block);
	else_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, merge_block);
	efree(compiled_expr);

	return 0;*/
}
