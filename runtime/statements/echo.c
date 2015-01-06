
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

int php_printf_long(const char *format, long i) {
	php_printf(format, i);
	return 0;
}

static LLVMValueRef zephir_get_php_printf_long(zephir_context *context)
{
	LLVMValueRef    function;
	LLVMTypeRef arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "php_printf_long");
	if (!function) {

		arg_tys[0] = LLVMPointerType(LLVMInt8Type(), 0);
#if ZEPHIR_32
		arg_tys[1] = LLVMInt32Type();
#else
		arg_tys[1] = LLVMInt64Type();
#endif
		function = LLVMAddFunction(context->module, "php_printf_long", LLVMFunctionType(LLVMVoidType(), arg_tys, 2, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register php_printf_long");
		}

		LLVMAddGlobalMapping(context->engine, function, php_printf);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	return function;
}

static LLVMValueRef zephir_get_php_printf_double(zephir_context *context)
{
	LLVMValueRef    function;
	LLVMTypeRef arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "php_printf_double");
	if (!function) {

		arg_tys[0] = LLVMPointerType(LLVMInt8Type(), 0);
		arg_tys[1] = LLVMDoubleType();
		function = LLVMAddFunction(context->module, "php_printf_double", LLVMFunctionType(LLVMVoidType(), arg_tys, 2, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register php_printf_double");
		}

		LLVMAddGlobalMapping(context->engine, function, php_printf);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	return function;
}

static LLVMValueRef zephir_get_php_printf_string(zephir_context *context)
{
	LLVMValueRef    function;
	LLVMTypeRef arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "php_printf_string");
	if (!function) {

		arg_tys[0] = LLVMPointerType(LLVMInt8Type(), 0);
		arg_tys[1] = LLVMPointerType(LLVMInt8Type(), 0);
		function = LLVMAddFunction(context->module, "php_printf_string", LLVMFunctionType(LLVMVoidType(), arg_tys, 2, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register php_printf_long");
		}

		LLVMAddGlobalMapping(context->engine, function, php_printf);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	return function;
}

static LLVMValueRef zephir_get_zend_print_zval(zephir_context *context)
{
	LLVMValueRef    function;
	LLVMTypeRef arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "zend_print_zval");
	if (!function) {

		arg_tys[0] = context->types.zval_pointer_type;
		arg_tys[1] = LLVMInt32Type();
		function = LLVMAddFunction(context->module, "zend_print_zval", LLVMFunctionType(LLVMInt32Type(), arg_tys, 2, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zend_print_zval");
		}

		LLVMAddGlobalMapping(context->engine, function, zend_print_zval);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);

	}

	return function;
}

int zephir_statement_echo(zephir_context *context, zval *statement TSRMLS_DC)
{

	HashTable       *ht;
	HashPosition    pos = {0};
	zval            **expr, *expressions;
	LLVMValueRef    function, args[5];
	zephir_compiled_expr *compiled_expr;
	LLVMTypeRef *arg_tys;

	_zephir_array_fetch_string(&expressions, statement, "expressions", strlen("expressions") + 1 TSRMLS_CC);
	if (Z_TYPE_P(expressions) != IS_ARRAY) {
		return 0;
	}

	ht = Z_ARRVAL_P(expressions);
	zend_hash_internal_pointer_reset_ex(ht, &pos);
	for (
	 ; zend_hash_get_current_data_ex(ht, (void**) &expr, &pos) == SUCCESS
	 ; zend_hash_move_forward_ex(ht, &pos)
	) {

		compiled_expr = zephir_expr(context, *expr TSRMLS_CC);
		switch (compiled_expr->type) {

			case ZEPHIR_T_TYPE_LONG:
			case ZEPHIR_T_TYPE_INTEGER:
				args[0] = LLVMBuildGlobalStringPtr(context->builder, "%ld", "");
				args[1] = compiled_expr->value;
				LLVMBuildCall(context->builder, zephir_get_php_printf_long(context), args, 2, "");
				break;

			case ZEPHIR_T_TYPE_DOUBLE:
				args[0] = LLVMBuildGlobalStringPtr(context->builder, "%f", "");
				args[1] = compiled_expr->value;
				LLVMBuildCall(context->builder, zephir_get_php_printf_double(context), args, 2, "");
				break;

			case ZEPHIR_T_TYPE_STRING:
				args[0] = LLVMBuildGlobalStringPtr(context->builder, "%s", "");
				args[1] = compiled_expr->value;
				LLVMBuildCall(context->builder, zephir_get_php_printf_string(context), args, 2, "");
				break;

			case ZEPHIR_T_TYPE_VAR:

				switch (compiled_expr->variable->type) {

					case ZEPHIR_T_TYPE_LONG:
					case ZEPHIR_T_TYPE_INTEGER:
						args[0] = LLVMBuildGlobalStringPtr(context->builder, "%ld", "");
						args[1] = LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, "");
						LLVMBuildCall(context->builder, zephir_get_php_printf_long(context), args, 2, "");
						break;

					case ZEPHIR_T_TYPE_VAR:
						args[0] = LLVMBuildLoad(context->builder, compiled_expr->variable->value_ref, "");
						args[1] = LLVMConstInt(LLVMInt32Type(), 0, 0);
						LLVMBuildCall(context->builder, zephir_get_zend_print_zval(context), args, 2, "");
						break;
				}

				break;

			default:
				zend_error(E_ERROR, "Unknown compiled expression %d", compiled_expr->type);
				break;
		}

		efree(compiled_expr);
	}

	return 0;
}
