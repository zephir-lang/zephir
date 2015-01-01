
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

#include "kernel/main.h"

LLVMValueRef zephir_optimizers_evalexpr(zephir_context *context, zval *expr) {

	LLVMValueRef condition = NULL;
	zephir_compiled_expr *compiled_expr;

	compiled_expr = zephir_expr(context, expr TSRMLS_CC);
	switch (compiled_expr->type) {

		case ZEPHIR_T_TYPE_BOOL:
			condition = compiled_expr->value;
			break;

		case ZEPHIR_T_TYPE_LONG:
		case ZEPHIR_T_TYPE_INTEGER:
#if ZEPHIR_32
			condition = LLVMBuildICmp(context->builder, LLVMIntNE, compiled_expr->value, LLVMConstInt(LLVMInt32Type(), 0, 0), "");
#else
			condition = LLVMBuildICmp(context->builder, LLVMIntNE, compiled_expr->value, LLVMConstInt(LLVMInt64Type(), 0, 0), "");
#endif
			break;

		case ZEPHIR_T_TYPE_DOUBLE:
			condition = LLVMBuildFCmp(context->builder, LLVMRealUNE, compiled_expr->value, LLVMConstReal(LLVMDoubleType(), 0), "");
			break;

		case ZEPHIR_T_TYPE_VAR:

			switch (compiled_expr->variable->type) {

				case ZEPHIR_T_TYPE_BOOL:
					condition = LLVMBuildICmp(context->builder, LLVMIntNE, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), LLVMConstInt(LLVMInt8Type(), 0, 0), "");
					break;

				case ZEPHIR_T_TYPE_INTEGER:
				case ZEPHIR_T_TYPE_LONG:
#if ZEPHIR_32
					condition = LLVMBuildICmp(context->builder, LLVMIntNE, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), LLVMConstInt(LLVMInt32Type(), 0, 0), "");
#else
					condition = LLVMBuildICmp(context->builder, LLVMIntNE, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), LLVMConstInt(LLVMInt64Type(), 0, 0), "");
#endif
					break;

				case ZEPHIR_T_TYPE_DOUBLE:
					condition = LLVMBuildFCmp(context->builder, LLVMRealUNE, LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, ""), LLVMConstReal(LLVMDoubleType(), 0), "");
					break;

				case ZEPHIR_T_TYPE_VAR:
					condition = LLVMBuildICmp(context->builder, LLVMIntNE, zephir_build_zend_is_true(context, compiled_expr->variable->value_ref), LLVMConstInt(LLVMInt32Type(), 0, 0), "");
					break;

			}

	}

	efree(compiled_expr);

	return condition;
}
