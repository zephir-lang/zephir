
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
 * Builds an 'while' statement
 */
int zephir_statement_while(zephir_context *context, zval *statement TSRMLS_DC)
{

	zval *expr, *statements;
	LLVMValueRef condition, then_value, else_value;

	_zephir_array_fetch_string(&expr, statement, SS("expr") TSRMLS_CC);
	if (Z_TYPE_P(expr) != IS_ARRAY) {
		return 0;
	}

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef start_block = LLVMAppendBasicBlock(func, "start-while");
	LLVMBuildBr(context->builder, start_block);
	LLVMPositionBuilderAtEnd(context->builder, start_block);

	context->inside_cycle++;

	condition = zephir_optimizers_evalexpr(context, expr);
	if (!condition) {
		zend_error(E_ERROR, "Unknown eval expression");
	}

	LLVMBasicBlockRef while_block = LLVMAppendBasicBlock(func, "block-while");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-while");
	LLVMBuildCondBr(context->builder, condition, while_block, merge_block);

	LLVMPositionBuilderAtEnd(context->builder, while_block);
	_zephir_array_fetch_string(&statements, statement, SS("statements") TSRMLS_CC);
	if (Z_TYPE_P(statements) == IS_ARRAY) {
		zephir_compile_block(context, statements);
		if (context->is_unrecheable == 0) {
			LLVMBuildBr(context->builder, start_block);
		}
	} else {
		LLVMBuildBr(context->builder, start_block);
	}
	while_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, merge_block);
	context->is_unrecheable = 0;

	context->inside_cycle--;

	return 0;
}
