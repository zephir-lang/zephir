
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
#include "errors.h"
#include "expr.h"

#include "kernel/main.h"
#include "kernel/string.h"

LLVMValueRef *zephir_resolve_parameters(zephir_context *context, zval *parameters TSRMLS_DC)
{

	HashTable            *ht = Z_ARRVAL_P(parameters);
	HashPosition         pos = {0};
	zval                 **expr, *parameter;
	LLVMValueRef         *args;
	zephir_compiled_expr *compiled_expr;
	int i = 0;

	args = emalloc(sizeof(LLVMValueRef) * zend_hash_num_elements(Z_ARRVAL_P(parameters)));
	zend_hash_internal_pointer_reset_ex(ht, &pos);
	for (
	 ; zend_hash_get_current_data_ex(ht, (void**) &expr, &pos) == SUCCESS
	 ; zend_hash_move_forward_ex(ht, &pos)
	) {

		_zephir_array_fetch_string(&parameter, *expr, SS("parameter") TSRMLS_CC);
		if (Z_TYPE_P(parameter) != IS_ARRAY) {
			zephir_error(parameter, "Corrupt parameter");
		}

		compiled_expr = zephir_expr(context, parameter);
		switch (compiled_expr->type) {

			case ZEPHIR_T_TYPE_VAR:
				args[i] = LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, "");
				break;

			default:
				fprintf(stderr, "%d\n", compiled_expr->type);

		}

		efree(compiled_expr);
		i++;
	}

	return args;
}

static LLVMValueRef zephir_get_strlen(zephir_context *context)
{
	LLVMValueRef    function;
	LLVMTypeRef     arg_tys[1];

	function = LLVMGetNamedFunction(context->module, "zephir_strlen");
	if (!function) {

		arg_tys[0] = context->types.zval_pointer_type;
#if ZEPHIR_32
		function = LLVMAddFunction(context->module, "zephir_strlen", LLVMFunctionType(LLVMInt32Type(), arg_tys, 1, 0));
#else
		function = LLVMAddFunction(context->module, "zephir_strlen", LLVMFunctionType(LLVMInt64Type(), arg_tys, 1, 0));
#endif
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephir_strlen");
		}

		LLVMAddGlobalMapping(context->engine, function, zephir_fast_strlen_ev);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	return function;
}

/**
 *
 */
zephir_compiled_expr *zephir_strlen_optimizer(zephir_context *context, zval *expr TSRMLS_DC) {

	zval *parameters;
	zephir_compiled_expr *compiled_expr;
	LLVMValueRef *args;

	_zephir_array_fetch_string(&parameters, expr, SS("parameters") TSRMLS_CC);
	if (Z_TYPE_P(parameters) != IS_ARRAY || zend_hash_num_elements(Z_ARRVAL_P(parameters)) != 1) {
		zephir_error(expr, "'strlen' optimizer requires 1 parameter");
	}

	compiled_expr = emalloc(sizeof(zephir_compiled_expr));
	compiled_expr->type  = ZEPHIR_T_TYPE_LONG;

	args = zephir_resolve_parameters(context, parameters);
	compiled_expr->value = LLVMBuildCall(context->builder, zephir_get_strlen(context), args, 1, "");

	efree(args);

	return compiled_expr;
}

zephir_compiled_expr *zephir_fcall_compile(zephir_context *context, zval *expr TSRMLS_DC) {

	zval *name;

	_zephir_array_fetch_string(&name, expr, SS("name") TSRMLS_CC);
	if (Z_TYPE_P(name) != IS_STRING) {
		return NULL;
	}

	if (!memcmp(Z_STRVAL_P(name), SS("strlen"))) {
		return zephir_strlen_optimizer(context, expr);
	}

	return NULL;
}

