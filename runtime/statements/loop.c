
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
 * Builds an 'loop' statement
 */
int zephir_statement_loop(zephir_context *context, zval *statement TSRMLS_DC)
{

	zval *statements;
	LLVMValueRef condition, then_value, else_value;

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef start_block = LLVMAppendBasicBlock(func, "start-loop");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-while");
	LLVMBuildBr(context->builder, start_block);
	LLVMPositionBuilderAtEnd(context->builder, start_block);

	context->inside_cycle++;

	_zephir_array_fetch_string(&statements, statement, SS("statements") TSRMLS_CC);
	if (Z_TYPE_P(statements) == IS_ARRAY) {
		zephir_compile_block(context, statements);
		if (context->is_unrecheable == 0) {
			LLVMBuildBr(context->builder, start_block);
		}
	} else {
		LLVMBuildBr(context->builder, start_block);
	}

	LLVMPositionBuilderAtEnd(context->builder, merge_block);
	context->is_unrecheable = 0;

	context->inside_cycle--;

	return 0;
}
