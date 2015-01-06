
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

#include <string.h>

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
	zval **parameter, *name, *data_type;
	zephir_variable *symbol, *symbol_param, *convert_params_from[16], *convert_params_to[16];
	LLVMValueRef    function, args[16];
	LLVMTypeRef     arg_tys[3];
	char            *buffer;
	int i, number_convert = 0;

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

		_zephir_array_fetch_string(&name, *parameter, SS("name") TSRMLS_CC);
		if (Z_TYPE_P(name) != IS_STRING) {
			continue;
		}

		symbol = zephir_symtable_add(ZEPHIR_T_TYPE_VAR, Z_STRVAL_P(name), Z_STRLEN_P(name), context);

		_zephir_array_fetch_string(&data_type, *parameter, SS("data-type") TSRMLS_CC);
		if (Z_TYPE_P(data_type) != IS_STRING || !memcmp(Z_STRVAL_P(data_type), SS("variable"))) {

			symbol->value_ref = LLVMBuildAlloca(context->builder, context->types.zval_pointer_type, Z_STRVAL_P(name));
			args[i] = symbol->value_ref;
			symbol->initialized = 1;
			i++;
			continue;

		} else {

			buffer = emalloc(sizeof(char) * Z_STRLEN_P(name) + 7);
			snprintf(buffer, Z_STRLEN_P(name) + 7, "%s_param", Z_STRVAL_P(name));
			buffer[Z_STRLEN_P(name) + 7] = '\0';

			symbol_param = zephir_symtable_add(ZEPHIR_T_TYPE_VAR, buffer, Z_STRLEN_P(name) + 6, context);
			symbol_param->value_ref = LLVMBuildAlloca(context->builder, context->types.zval_pointer_type, buffer);
			symbol_param->initialized = 1;
			args[i] = symbol_param->value_ref;
			i++;

			efree(buffer);

			if (!memcmp(Z_STRVAL_P(data_type), SS("long"))) {

				symbol = zephir_symtable_add(ZEPHIR_T_TYPE_LONG, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
#if ZEPHIR_32
				symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt32Type(), Z_STRVAL_P(name));
#else
				symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt64Type(), Z_STRVAL_P(name));
#endif
				symbol->initialized = 1;

				convert_params_to[number_convert] = symbol;
				convert_params_from[number_convert] = symbol_param;
				number_convert++;

				continue;
			}

			if (!memcmp(Z_STRVAL_P(data_type), SS("int"))) {

				symbol = zephir_symtable_add(ZEPHIR_T_TYPE_INTEGER, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
#if ZEPHIR_32
				symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt32Type(), Z_STRVAL_P(name));
#else
				symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt64Type(), Z_STRVAL_P(name));
#endif
				symbol->initialized = 1;

				convert_params_to[number_convert] = symbol;
				convert_params_from[number_convert] = symbol_param;
				number_convert++;

				continue;
			}

			if (!memcmp(Z_STRVAL_P(data_type), SS("bool"))) {

				symbol = zephir_symtable_add(ZEPHIR_T_TYPE_BOOL, Z_STRVAL_P(name), Z_STRLEN_P(name), context);
				symbol->value_ref = LLVMBuildAlloca(context->builder, LLVMInt8Type(), Z_STRVAL_P(name));
				symbol->initialized = 1;

				convert_params_to[number_convert] = symbol;
				convert_params_from[number_convert] = symbol_param;
				number_convert++;

				continue;
			}

		}

		zend_print_zval_r(*parameter, 0 TSRMLS_CC);
	}
	args[i] = NULL;

	/**
	 * Create or register zephir_fetch_parameters
	 */
	function = LLVMGetNamedFunction(context->module, "zephir_fetch_parameters");
	if (!function) {

		arg_tys[0] = LLVMInt32Type();
		arg_tys[1] = LLVMInt32Type();
		arg_tys[2] = LLVMInt32Type();
		function = LLVMAddFunction(context->module, "zephir_fetch_parameters", LLVMFunctionType(LLVMInt32Type(), arg_tys, 3, 1));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephir_fetch_parameters");
		}

		LLVMAddGlobalMapping(context->engine, function, zephirt_fetch_parameters);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	LLVMValueRef fetchStatus = LLVMBuildCall(context->builder, function, args, i, "");
	LLVMValueRef condition = LLVMBuildICmp(context->builder, LLVMIntEQ, fetchStatus, LLVMConstInt(LLVMInt32Type(), FAILURE, 0), "");

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(func, "then");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(func, "else");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-if");

	LLVMBuildCondBr(context->builder, condition, then_block, else_block);

	LLVMPositionBuilderAtEnd(context->builder, then_block);
	zephir_build_memory_restore_stack(context);
	LLVMBuildRetVoid(context->builder);
	then_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, else_block);
	LLVMBuildBr(context->builder, merge_block);
	else_block = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, merge_block);

	for (i = 0; i < number_convert; i++) {

		switch (convert_params_to[i]->type) {

			case ZEPHIR_T_TYPE_BOOL:
				LLVMBuildStore(context->builder, zephir_build_get_boolval(context, convert_params_from[i]->value_ref), convert_params_to[i]->value_ref); // store i64 %16, i64* %a, align 8
				break;

			case ZEPHIR_T_TYPE_LONG:
			case ZEPHIR_T_TYPE_INTEGER:
				LLVMBuildStore(context->builder, zephir_build_get_intval(context, convert_params_from[i]->value_ref), convert_params_to[i]->value_ref); // store i64 %16, i64* %a, align 8
				break;

		}

	}
}

/**
 * This compiles every method into machine-code based methods
 */
static void zephir_compile_methods(zephir_context *context, const zval *class_name, zval *methods, zend_function_entry *class_functions TSRMLS_DC)
{
	HashTable       *ht, *ht_visibility;
	HashPosition    pos = {0}, pos_visibility = {0};
	zval **method, *name, *parameters, *statements, *visibility, **visibility_item;
	zend_function_entry *class_function;
	LLVMValueRef func, param, alloca[5];
	LLVMTypeRef ret, params[5];
	LLVMBasicBlockRef block;
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
		int flags;
		char *function_name, function_length;

		_zephir_array_fetch_string(&name, *method, SS("name") TSRMLS_CC);
		if (Z_TYPE_P(name) != IS_STRING) {
			continue;
		}

		function_length = Z_STRLEN_P(class_name) + Z_STRLEN_P(name) + 2;
		function_name = emalloc(function_length);

		memcpy(function_name, Z_STRVAL_P(class_name), Z_STRLEN_P(class_name));
		function_name[Z_STRLEN_P(class_name)] = '+';
		memcpy(function_name + Z_STRLEN_P(class_name) + 1, Z_STRVAL_P(name), Z_STRLEN_P(name));
		function_name[function_length - 1] = '\0';

		/**
		 * Create the function prototype
		 */
		func = LLVMAddFunction(context->module, function_name, LLVMFunctionType(LLVMVoidType(), params, 5, 0));
		LLVMSetLinkage(func, LLVMExternalLinkage);

		context->declarations_block = LLVMAppendBasicBlock(func, "declarations");
		LLVMPositionBuilderAtEnd(context->builder, context->declarations_block);

		/**
		 * Initialize context
		 */
		context->inside_cycle = 0;
		context->is_unrecheable = 0;

		/**
		 * Create a new symbol table
		 */
		symtable = zephir_symtable_new();
		context->symtable = symtable;

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

		block = LLVMAppendBasicBlock(func, "entry");
		LLVMPositionBuilderAtEnd(context->builder, block);

		/**
		 * Always grow the stack conservatively
		 */
		zephir_build_memory_grow_stack(context);

		_zephir_array_fetch_string(&parameters, *method, SS("parameters") TSRMLS_CC);
		if (Z_TYPE_P(parameters) == IS_ARRAY) {
			zephir_process_parameters(context, parameters);
		}

		_zephir_array_fetch_string(&statements, *method, SS("statements") TSRMLS_CC);
		if (Z_TYPE_P(statements) == IS_ARRAY) {
			zephir_compile_block(context, statements);
		} else {
			context->is_unrecheable = 0;
		}

		if (context->is_unrecheable == 0) {
			zephir_build_memory_restore_stack(context);
			LLVMBuildRetVoid(context->builder);
		}

		/**
		 * Join "declarations" block with "entry" block
		 */
		LLVMPositionBuilderAtEnd(context->builder, context->declarations_block);
		LLVMBuildBr(context->builder, block);

		/**
		 * Shows the generated LLVM IR for every method if enviroment variable is defined
		 */
		if (getenv("ZEPHIR_RT_DEBUG")) {
			LLVMDumpValue(func);
			if (LLVMVerifyFunction(func, LLVMPrintMessageAction) == 1) {
				LLVMDeleteFunction(func);
				continue;
			}
		}

		/**
		 * Check and calculate visibility
		 */
		flags = 0;
		_zephir_array_fetch_string(&visibility, *method, SS("visibility") TSRMLS_CC);
		if (Z_TYPE_P(visibility) == IS_ARRAY) {

			ht_visibility = Z_ARRVAL_P(visibility);
			zend_hash_internal_pointer_reset_ex(ht_visibility, &pos_visibility);
			for (
			 ; zend_hash_get_current_data_ex(ht_visibility, (void**) &visibility_item, &pos_visibility) == SUCCESS
			 ; zend_hash_move_forward_ex(ht_visibility, &pos_visibility)
			) {

				if (Z_TYPE_PP(visibility_item) != IS_STRING) {
					continue;
				}

				if (!memcmp(Z_STRVAL_PP(visibility_item), SS("public"))) {
					flags |= ZEND_ACC_PUBLIC;
					continue;
				} else {
					if (!memcmp(Z_STRVAL_PP(visibility_item), SS("protected"))) {
						flags |= ZEND_ACC_PROTECTED;
						continue;
					} else {
						if (!memcmp(Z_STRVAL_PP(visibility_item), SS("private"))) {
							flags |= ZEND_ACC_PRIVATE;
							continue;
						}
					}
				}

				if (!memcmp(Z_STRVAL_PP(visibility_item), SS("static"))) {
					flags |= ZEND_ACC_STATIC;
					continue;
				}

				if (!memcmp(Z_STRVAL_PP(visibility_item), SS("final"))) {
					flags |= ZEND_ACC_FINAL;
					continue;
				}
			}

		}

		class_function->fname    = zend_strndup(Z_STRVAL_P(name), Z_STRLEN_P(name));
		class_function->handler  = LLVMGetPointerToGlobal(context->engine, func);
		class_function->arg_info = NULL;
		class_function->num_args = 0;
		class_function->flags    = flags ? flags : ZEND_ACC_PUBLIC;
		class_function++;

		efree(context->symtable);
		efree(function_name);
	}

	class_function->fname = NULL;

}

/**
 * Declares variables
 */
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

		_zephir_array_fetch_string(&name, *property, SS("name") TSRMLS_CC);
		if (Z_TYPE_P(name) != IS_STRING) {
			continue;
		}

		zend_declare_property_null(class_ce, Z_STRVAL_P(name), Z_STRLEN_P(name), ZEND_ACC_PUBLIC TSRMLS_CC);
	}
}

/**
 *
 */
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

	/**
	 * Load the class definition
	 */
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
			zephir_compile_methods(context, class_name, methods, &class_functions[0]);
		}
	}

	/**
	 * Register the class
	 */
	if (number_methods > 0) {
		ZEPHIR_INIT_OVERLOADED_CLASS_ENTRY_EX(ce, Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), &class_functions[0]);
	} else{
		ZEPHIR_INIT_OVERLOADED_CLASS_ENTRY_EX(ce, Z_STRVAL_P(class_name), Z_STRLEN_P(class_name), NULL);
	}
	class_ce = zend_register_internal_class(&ce TSRMLS_CC);

	/**
	 * Process properties
	 */
	_zephir_array_fetch_string(&properties, definition, SS("properties") TSRMLS_CC);
	if (Z_TYPE_P(properties) == IS_ARRAY) {
		zephir_compile_properties(properties, class_ce);
	}

	return 1;
}
