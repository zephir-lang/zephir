
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
#include "blocks.h"

#include "kernel/main.h"
#include "optimizers/evalexpr.h"

/**
 * Builds an 'if' statement
 */
int zephir_statement_if(zephir_context *context, zval *statement TSRMLS_DC)
{

	zval *expr, *statements;
	LLVMValueRef condition, then_value, else_value;

	_zephir_array_fetch_string(&expr, statement, SS("expr") TSRMLS_CC);
	if (Z_TYPE_P(expr) != IS_ARRAY) {
		return 0;
	}

	condition = zephir_optimizers_evalexpr(context, expr);
	if (!condition) {
		zend_error(E_ERROR, "Unknown eval expression");
	}

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(func, "then");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(func, "else");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-if");

	LLVMBuildCondBr(context->builder, condition, then_block, else_block);

	LLVMPositionBuilderAtEnd(context->builder, then_block);
	_zephir_array_fetch_string(&statements, statement, SS("statements") TSRMLS_CC);
	if (Z_TYPE_P(statements) == IS_ARRAY) {
		zephir_compile_block(context, statements);
		if (context->is_unrecheable == 0) {
			LLVMBuildBr(context->builder, merge_block);
		}
	} else {
		LLVMBuildBr(context->builder, merge_block);
	}
	then_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, else_block);
	_zephir_array_fetch_string(&statements, statement, SS("else_statements") TSRMLS_CC);
	if (Z_TYPE_P(statements) == IS_ARRAY) {
		zephir_compile_block(context, statements);
		if (context->is_unrecheable == 0) {
			LLVMBuildBr(context->builder, merge_block);
		}
	} else {
		LLVMBuildBr(context->builder, merge_block);
	}
	else_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, merge_block);
	context->is_unrecheable = 0;

	return 0;
}
