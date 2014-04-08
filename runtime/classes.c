
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
#include "kernel/main.h"

#include "utils.h"
#include "blocks.h"
#include "symtable.h"
#include "builder.h"

#define ZEPHIR_INIT_OVERLOADED_CLASS_ENTRY_EX(class_container, class_name, class_name_len, functions) { \
	const char *cl_name = class_name;                               \
	int _len = class_name_len;                              \
	class_container.name = zend_strndup(cl_name, _len); \
	class_container.name_length = _len;                     \
	INIT_CLASS_ENTRY_INIT_METHODS(class_container, functions, NULL, NULL, NULL, NULL, NULL) \
}

PHP_METHOD(Zephir, emptyMethod) {
	RETURN_NULL();
}

static void zephir_process_parameters(zephir_context *context, zval *parameters TSRMLS_DC)
{
	HashTable       *ht;
	HashPosition    pos = {0};
	zval **parameter, *name;
	zephir_variable *symbol;
	LLVMValueRef    function, args[10];
	LLVMTypeRef     *arg_tys;
	int i;

	args[0] = LLVMConstInt(LLVMInt32Type(), zend_hash_num_elements(Z_ARRVAL_P(parameters)), 0);
	args[1] = LLVMConstInt(LLVMInt32Type(), zend_hash_num_elements(Z_ARRVAL_P(parameters)), 0);
	args[2] = LLVMConstInt(LLVMInt32Type(), 0, 0);

	i = 3;
	ht = Z_ARRVAL_P(parameters);
	zend_hash_internal_pointer_reset_ex(ht, &pos);
	for (
	 ; zend_hash_get_current_data_ex(ht, (void**) &parameter, &pos) == SUCCESS
	 ; zend_hash_move_forward_ex(ht, &pos)
	) {

		_zephir_array_fetch_string(&name, *parameter, "name", strlen("name") + 1 TSRMLS_CC);
		if (Z_TYPE_P(name) != IS_STRING) {
			continue;
		}

		symbol = zephir_symtable_add(ZEPHIR_T_TYPE_VAR, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
		symbol->value_ref = LLVMBuildAlloca(context->builder, context->types.zval_pointer_type, Z_STRVAL_P(name));
		symbol->initialized = 1;
		args[i] = symbol->value_ref;
		i++;
	}
	args[i] = NULL;

	/**
	 * Create or register zephir_fetch_parameters
	 */
	function = LLVMGetNamedFunction(context->module, "zephir_fetch_parameters");
	if (!function) {

		arg_tys    = emalloc(3 * sizeof(*arg_tys));
		arg_tys[0] = LLVMInt32Type();
		arg_tys[1] = LLVMInt32Type();
		arg_tys[2] = LLVMInt32Type();
		function = LLVMAddFunction(context->module, "zephir_fetch_parameters", LLVMFunctionType(LLVMInt32Type(), arg_tys, 3, 1));
		if (!function) {
			efree(arg_tys);
			zend_error(E_ERROR, "Cannot register zephir_fetch_parameters");
		}

		LLVMAddGlobalMapping(context->engine, function, zephirt_fetch_parameters);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);

		efree(arg_tys);
	}

	LLVMValueRef fetchStatus = LLVMBuildCall(context->builder, function, args, i, "");
	LLVMValueRef condition = LLVMBuildICmp(context->builder, LLVMIntEQ, fetchStatus, LLVMConstInt(LLVMInt32Type(), FAILURE, 0), "ifcond");

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(func, "then");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(func, "else");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-if");

	LLVMBuildCondBr(context->builder, condition, then_block, else_block);

	LLVMPositionBuilderAtEnd(context->builder, then_block);
	LLVMBuildRetVoid(context->builder);
	then_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, else_block);
	LLVMBuildBr(context->builder, merge_block);
	else_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, merge_block);
}

static void zephir_compile_methods(zephir_context *context, zval *methods, zend_function_entry *class_functions TSRMLS_DC)
{
	HashTable       *ht;
	HashPosition    pos = {0};
	zval **method, *name, *parameters, *statements;
	zend_function_entry *class_function;
	LLVMValueRef func, param, alloca[5];
	LLVMTypeRef ret, params[5];
	zephir_symtable *symtable;
	zephir_variable *symbols[5];
	int i;

	zephir_initialize_zval_struct(context);

	params[0] = LLVMInt32Type(); // int ht
	params[1] = context->types.zval_pointer_type; // zval *return_value
	params[2] = context->types.zval_double_pointer_type; // zval **return_value_ptr
	params[3] = context->types.zval_pointer_type; // zval *this_ptr
	params[4] = LLVMInt32Type(); // int ht

	ht = Z_ARRVAL_P(methods);
	class_function = class_functions;
	zend_hash_internal_pointer_reset_ex(ht, &pos);
	for (
	 ; zend_hash_get_current_data_ex(ht, (void**) &method, &pos) == SUCCESS
	 ; zend_hash_move_forward_ex(ht, &pos)
	) {

		_zephir_array_fetch_string(&name, *method, "name", strlen("name") + 1 TSRMLS_CC);
		if (Z_TYPE_P(name) != IS_STRING) {
			continue;
		}

		/**
		 * Create the function prototype
		 */
		func = LLVMAddFunction(context->module, Z_STRVAL_P(name), LLVMFunctionType(LLVMVoidType(), params, 5, 0));
		LLVMSetLinkage(func, LLVMExternalLinkage);

		LLVMBasicBlockRef block = LLVMAppendBasicBlock(func, "entry");
		LLVMPositionBuilderAtEnd(context->builder, block);

		symtable = zephir_symtable_new();
		context->symtable = symtable;

		//%1 = alloca i32, align 4
  		//%2 = alloca %struct._zval_struct*, align 8
  		//%3 = alloca %struct._zval_struct**, align 8
  		//%4 = alloca %struct._zval_struct*, align 8
  		//%5 = alloca i32, align 4

		/**
		 * Initialize internal parameters
		 */
		param = LLVMGetParam(func, 0);
		LLVMSetValueName(param, "ht");
		symbols[0] = zephir_symtable_add(ZEPHIR_T_TYPE_INTEGER, SL("ht"), context);
		symbols[0]->initialized = 1;

		param = LLVMGetParam(func, 1);
		LLVMSetValueName(param, "return_value");
		symbols[1] = zephir_symtable_add(ZEPHIR_T_TYPE_VAR, SL("return_value"), context);
		symbols[1]->initialized = 1;

		param = LLVMGetParam(func, 2);
		LLVMSetValueName(param, "return_value_ptr");
		symbols[2] = zephir_symtable_add(ZEPHIR_T_TYPE_VAR, SL("return_value_ptr"), context);
		symbols[2]->initialized = 1;

		param = LLVMGetParam(func, 3);
		LLVMSetValueName(param, "this_ptr");
		symbols[3] = zephir_symtable_add(ZEPHIR_T_TYPE_VAR, SL("this_ptr"), context);
		symbols[3]->initialized = 1;
		symbols[3]->value_ref = param;

		param = LLVMGetParam(func, 4);
		LLVMSetValueName(param, "return_value_used");
		symbols[4] = zephir_symtable_add(ZEPHIR_T_TYPE_INTEGER, SL("return_value_used"), context);
		symbols[4]->initialized = 1;
		symbols[4]->value_ref = param;

		alloca[0] = LLVMBuildAlloca(context->builder, LLVMInt32Type(), "");
		alloca[1] = LLVMBuildAlloca(context->builder, context->types.zval_pointer_type, "");
		alloca[2] = LLVMBuildAlloca(context->builder, context->types.zval_double_pointer_type, "");
		alloca[3] = LLVMBuildAlloca(context->builder, context->types.zval_pointer_type, "");
		alloca[4] = LLVMBuildAlloca(context->builder, LLVMInt32Type(), "");

  		LLVMBuildStore(context->builder, LLVMGetParam(func, 0), alloca[0]);
  		LLVMBuildStore(context->builder, LLVMGetParam(func, 1), alloca[1]);
  		LLVMBuildStore(context->builder, LLVMGetParam(func, 2), alloca[2]);
  		LLVMBuildStore(context->builder, LLVMGetParam(func, 3), alloca[3]);
  		LLVMBuildStore(context->builder, LLVMGetParam(func, 4), alloca[4]);

  		symbols[0]->value_ref = alloca[0];
  		symbols[1]->value_ref = alloca[1];
  		symbols[2]->value_ref = alloca[2];
  		symbols[3]->value_ref = alloca[3];
  		symbols[4]->value_ref = alloca[4];

		_zephir_array_fetch_string(&parameters, *method, SS("parameters") TSRMLS_CC);
		if (Z_TYPE_P(parameters) == IS_ARRAY) {
			zephir_process_parameters(context, parameters);
		}

		_zephir_array_fetch_string(&statements, *method, SS("statements") TSRMLS_CC);
		if (Z_TYPE_P(statements) == IS_ARRAY) {
			zephir_compile_block(context, statements);
		}

		LLVMBuildRetVoid(context->builder);

		LLVMDumpValue(func);

		if (LLVMVerifyFunction(func, LLVMPrintMessageAction) == 1) {
			LLVMDeleteFunction(func);
			continue;
		}

		class_function->fname    = zend_strndup(Z_STRVAL_P(name), Z_STRLEN_P(name));
		class_function->handler  = LLVMGetPointerToGlobal(context->engine, func);
		class_function->arg_info = NULL;
		class_function->num_args = 0;
		class_function->flags     = ZEND_ACC_PUBLIC;
		class_function++;

		efree(context->symtable);
	}

	class_function->fname = NULL;

}

void zephir_compile_properties(zval *properties, zend_class_entry *class_ce)
{
	HashTable       *ht;
	HashPosition    pos = {0};
	zval            **property, *name;

	ht = Z_ARRVAL_P(properties);
	zend_hash_internal_pointer_reset_ex(ht, &pos);
	for (
		 ; zend_hash_get_current_data_ex(ht, (void**) &property, &pos) == SUCCESS
		 ; zend_hash_move_forward_ex(ht, &pos)
	) {

		_zephir_array_fetch_string(&name, *property, "name", strlen("name") + 1 TSRMLS_CC);
		if (Z_TYPE_P(name) != IS_STRING) {
			continue;
		}

		zend_declare_property_null(class_ce, Z_STRVAL_P(name), Z_STRLEN_P(name), ZEND_ACC_PUBLIC TSRMLS_CC);
	}
}

int zephir_compile_class(zephir_context *context, zval *class_definition TSRMLS_DC) {

	zval *name, *properties, **method, **property, *definition, *methods, *class_name, *statements;
	zend_class_entry ce, *class_ce;
	HashTable       *ht;
	HashPosition    pos = {0};
	zend_function_entry class_functions[32];
	int number_methods = 0, i;

	_zephir_array_fetch_string(&class_name, class_definition, SS("name") TSRMLS_CC);
	if (Z_TYPE_P(class_name) != IS_STRING) {
		return 0;
	}

	_zephir_array_fetch_string(&definition, class_definition, SS("definition") TSRMLS_CC);
	if (Z_TYPE_P(definition) != IS_ARRAY) {
		ZEPHIR_INIT_OVERLOADED_CLASS_ENTRY_EX(ce, Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), NULL);
		zend_register_internal_class(&ce TSRMLS_CC);
		return 1;
	}

	/**
	 * Process methods
	 */
	_zephir_array_fetch_string(&methods, definition, SS("methods") TSRMLS_CC);
	if (Z_TYPE_P(methods) == IS_ARRAY) {
		number_methods = zend_hash_num_elements(Z_ARRVAL_P(methods));
		if (number_methods > 0) {
			zephir_compile_methods(context, methods, &class_functions[0]);
		}
	}

	if (number_methods > 0) {
		ZEPHIR_INIT_OVERLOADED_CLASS_ENTRY_EX(ce, Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), &class_functions[0]);
	} else{
		ZEPHIR_INIT_OVERLOADED_CLASS_ENTRY_EX(ce, Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), NULL);
	}
	class_ce = zend_register_internal_class(&ce TSRMLS_CC);

	_zephir_array_fetch_string(&properties, definition, SS("properties") TSRMLS_CC);
	if (Z_TYPE_P(properties) == IS_ARRAY) {
		zephir_compile_properties(properties, class_ce);
	}

	return 1;
}
