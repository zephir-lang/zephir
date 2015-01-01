
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
#include "symtable.h"
#include "builder.h"

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/operators.h"

int zephir_initialize_zval_struct(zephir_context *context)
{
	LLVMTypeRef zval_struct, zvalue_struct, zvalue_string_struct, zvalue_obj_struct;
	LLVMTypeRef zval_struct_types[4], zvalue_struct_types[1], zvalue_string_struct_types[2], zvalue_obj_struct_types[2];

	if (context->types.zval_type != NULL) {
		return 0;
	}

	zval_struct = LLVMStructCreateNamed(LLVMGetGlobalContext(), "%struct._zval_struct");

	zvalue_string_struct = LLVMStructCreateNamed(LLVMGetGlobalContext(), "%struct.zvalue_str"); // struct {
	zvalue_string_struct_types[0] = LLVMPointerType(LLVMInt8Type(), 0); // char *val;
	zvalue_string_struct_types[1] = LLVMInt32Type();                   // int len;
	LLVMStructSetBody(zvalue_string_struct, zvalue_string_struct_types, 2, 0);

	zvalue_obj_struct = LLVMStructCreateNamed(LLVMGetGlobalContext(), "%struct.zvalue_obj"); // struct {
	zvalue_obj_struct_types[0] = LLVMInt32Type();                   // int len;
	zvalue_obj_struct_types[1] = LLVMPointerType(LLVMInt8Type(), 0); // char *val;
	LLVMStructSetBody(zvalue_obj_struct, zvalue_obj_struct_types, 2, 0);

	zvalue_struct = LLVMStructCreateNamed(LLVMGetGlobalContext(), "%union._zvalue_value");
#if ZEPHIR_32
	zvalue_struct_types[0] = LLVMDoubleType(); // double dval;                /* double value */
#else
	zvalue_struct_types[0] = zvalue_string_struct; // { char *val, int len }
#endif
	//zvalue_struct_types[2] = zvalue_string_struct; // char *val;
	//zvalue_struct_types[3] = LLVMPointerType(LLVMInt8Type(), 0); // HashTable *ht;              /* hash table value */
	//zvalue_struct_types[4] = zvalue_obj_struct; // zend_object_value obj;
	LLVMStructSetBody(zvalue_struct, zvalue_struct_types, 1, 0);

	zval_struct_types[0] = zvalue_struct;
	zval_struct_types[1] = LLVMInt32Type();
	zval_struct_types[2] = LLVMInt8Type();
	zval_struct_types[3] = LLVMInt8Type();
	LLVMStructSetBody(zval_struct, zval_struct_types, 4, 0);

	context->types.zval_type = zval_struct;
	context->types.zval_pointer_type = LLVMPointerType(zval_struct, 0);
	context->types.zval_double_pointer_type = LLVMPointerType(context->types.zval_pointer_type, 0);
	context->types.zvalue_type = zvalue_struct;

	return 1;
}

/**
 * Builds a call to 'emalloc'
 */
void zephir_build_emalloc(zephir_context *context, LLVMTypeRef type, size_t size, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[3], bitcast, alloc;
	LLVMTypeRef     arg_tys[3];

	function = LLVMGetNamedFunction(context->module, "emalloc");
	if (!function) {

#if ZEPHIR_32
		arg_tys[0] = LLVMInt32Type();
#else
		arg_tys[0] = LLVMInt64Type();
#endif
		arg_tys[1] = LLVMPointerType(LLVMInt8Type(), 0);
		arg_tys[2] = LLVMInt32Type();
		function = LLVMAddFunction(context->module, "emalloc", LLVMFunctionType(LLVMPointerType(LLVMInt8Type(), 0), arg_tys, 3, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register emalloc");
		}

		LLVMAddGlobalMapping(context->engine, function, _emalloc);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

#if ZEPHIR_32
	args[0] = LLVMConstInt(LLVMInt32Type(), size, 0);
#else
	args[0] = LLVMConstInt(LLVMInt64Type(), size, 0);
#endif
	args[1] = LLVMBuildGlobalStringPtr(context->builder, "hello", "");
	args[2] = LLVMConstInt(LLVMInt32Type(), 1, 0);

	alloc = LLVMBuildCall(context->builder, function, args, 3, ""); // call i8* @_emalloc(i64 24, i8* getelementptr inbounds ([1 x i8]* @.str2, i32 0, i32 0), i32 1)
	bitcast = LLVMBuildBitCast(context->builder, alloc, type, ""); // bitcast i8* %6 to %struct._zval_struct*
	LLVMBuildStore(context->builder, bitcast, value_ref); // store %struct._zval_struct* %7, %struct._zval_struct** %p, align 8
}

/**
 * Builds a call to 'zval_dtor'
 */
void zephir_build_zval_dtor(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[1], bitcast, alloc;
	LLVMTypeRef     arg_tys[1];

	function = LLVMGetNamedFunction(context->module, "zval_dtor");
	if (!function) {

		arg_tys[0] = context->types.zval_pointer_type;
		function = LLVMAddFunction(context->module, "zval_dtor", LLVMFunctionType(LLVMVoidType(), arg_tys, 1, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zval_dtor");
		}

		LLVMAddGlobalMapping(context->engine, function, _zval_dtor);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = LLVMBuildLoad(context->builder, value_ref, "");
	LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to 'zval_ptr_dtor'
 */
void zephir_build_zval_ptr_dtor(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[1], bitcast, alloc;
	LLVMTypeRef     arg_tys[1];

	function = LLVMGetNamedFunction(context->module, "zval_ptr_dtor");
	if (!function) {

		arg_tys[0] = context->types.zval_double_pointer_type;
		function = LLVMAddFunction(context->module, "zval_ptr_dtor", LLVMFunctionType(LLVMVoidType(), arg_tys, 1, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zval_ptr_dtor");
		}

		LLVMAddGlobalMapping(context->engine, function, _zval_ptr_dtor);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = value_ref;
	LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to Z_ADDREF_P()
 */
void zephir_build_zval_addref(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr, bitcast;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 1, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 1

	LLVMBuildStore(context->builder, // store i32 %7, i32* %a, align 4
		LLVMBuildAdd( // add i32 %8, 1
			context->builder,
			LLVMBuildLoad(context->builder, ref, ""), // load i32* %a, align 4
			LLVMConstInt(LLVMInt32Type(), 1, 0),
			""
		),
		ref
	);
}

/**
 * Builds a call to Z_DELREF_P()
 */
void zephir_build_zval_delref(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr, bitcast;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 1, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 1

	LLVMBuildStore(context->builder, // store i32 %7, i32* %a, align 4
		LLVMBuildAdd( // add i32 %8, -1
			context->builder,
			LLVMBuildLoad(context->builder, ref, ""), // load i32* %a, align 4
			LLVMConstInt(LLVMInt32Type(), -1, 0),
			""
		),
		ref
	);
}

/**
 * Builds a call to Z_SET_REFCOUNT_P()
 */
void zephir_build_zval_set_refcount(zephir_context *context, LLVMValueRef symbol_ref, LLVMValueRef value) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 1, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 1

	LLVMBuildStore(context->builder, value, ref);
}

/**
 * Builds a call to Z_UNSET_ISREF_P()
 */
void zephir_build_zval_unset_isref(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 3, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 1

	LLVMBuildStore(context->builder, LLVMConstInt(LLVMInt8Type(), 0, 0), ref);
}

/**
 * Builds a call to Z_REFCOUNT_P()
 */
LLVMValueRef zephir_build_zval_refcount(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr, bitcast;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 1, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 1

	return LLVMBuildLoad(context->builder, ref, ""); //load i32* %7, align 4
}

/**
 * Builds a call to 'zephir_grow_stack'
 */
void zephir_build_memory_grow_stack(zephir_context *context) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	//zephirt_memory_grow_stack(TSRMLS_C)
	function = LLVMGetNamedFunction(context->module, "zephirt_memory_grow_stack");
	if (!function) {

		function = LLVMAddFunction(context->module, "zephirt_memory_grow_stack", LLVMFunctionType(LLVMVoidType(), NULL, 0, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephirt_memory_grow_stack");
		}

		LLVMAddGlobalMapping(context->engine, function, zephirt_memory_grow_stack);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	LLVMBuildCall(context->builder, function, NULL, 0, "");
}

/**
 * Builds a call to 'zephir_grow_stack'
 */
void zephir_build_memory_restore_stack(zephir_context *context) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	//zephirt_memory_restore_stack(TSRMLS_C)
	function = LLVMGetNamedFunction(context->module, "zephirt_memory_restore_stack");
	if (!function) {

		function = LLVMAddFunction(context->module, "zephirt_memory_restore_stack", LLVMFunctionType(LLVMVoidType(), NULL, 0, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephirt_memory_restore_stack");
		}

		LLVMAddGlobalMapping(context->engine, function, zephirt_memory_restore_stack);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	LLVMBuildCall(context->builder, function, NULL, 0, "");
}

/**
 * Builds a call to 'zephir_memory_alloc'
 */
void zephir_build_memory_alloc(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "zephirt_memory_alloc");
	if (!function) {

		arg_tys[0] = context->types.zval_double_pointer_type;
		function = LLVMAddFunction(context->module, "zephirt_memory_alloc", LLVMFunctionType(LLVMVoidType(), arg_tys, 1, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephirt_memory_alloc");
		}

		LLVMAddGlobalMapping(context->engine, function, zephirt_memory_alloc);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = value_ref;
	LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to 'zephir_memory_observe'
 */
void zephir_build_memory_observe(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "zephirt_memory_observe");
	if (!function) {

		arg_tys[0] = context->types.zval_double_pointer_type;
		function = LLVMAddFunction(context->module, "zephirt_memory_observe", LLVMFunctionType(LLVMVoidType(), arg_tys, 1, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephirt_memory_observe");
		}

		LLVMAddGlobalMapping(context->engine, function, zephirt_memory_observe);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = value_ref;
	LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to 'zephir_memory_nalloc'
 */
void zephir_build_memory_nalloc(zephir_context *context, LLVMValueRef value_ref) {

	LLVMBasicBlockRef then_block1, else_block1, merge_block1, then_block2, else_block2, merge_block2;
	LLVMValueRef current_function;

	current_function = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	then_block1 = LLVMAppendBasicBlock(current_function, "then");
	else_block1 = LLVMAppendBasicBlock(current_function, "else");
	merge_block1 = LLVMAppendBasicBlock(current_function, "merge-if");

	/**
	 * Check if the pointer is not NULL
	 */
	LLVMBuildCondBr(context->builder, LLVMBuildIsNotNull(context->builder, LLVMBuildLoad(context->builder, value_ref, ""), ""), then_block1, else_block1);

	LLVMPositionBuilderAtEnd(context->builder, then_block1);

	{

		then_block2 = LLVMAppendBasicBlock(current_function, "then");
		else_block2 = LLVMAppendBasicBlock(current_function, "else");
		merge_block2 = LLVMAppendBasicBlock(current_function, "merge-if");

		/**
		 * Check if the refcount is greater than 1
		 */
		LLVMBuildCondBr(context->builder,
			LLVMBuildICmp(
				context->builder,
				LLVMIntUGT,
				zephir_build_zval_refcount(context, value_ref),
				LLVMConstInt(LLVMInt32Type(), 1, 0),
				""
			),
			then_block2,
			else_block2
		);

		LLVMPositionBuilderAtEnd(context->builder, then_block2);

		/**
		 * Reduce the refcount
		 */
		zephir_build_zval_delref(context, value_ref);

		/**
		 * Realloc the pointer
		 */
		zephir_build_emalloc(context, context->types.zval_pointer_type, sizeof(zval), value_ref);

		/**
		 * Reset refcount to 1
		 */
		zephir_build_zval_set_refcount(context, value_ref, LLVMConstInt(LLVMInt32Type(), 1, 0));

		/**
		 * Mark zval as not reference
		 */
		zephir_build_zval_unset_isref(context, value_ref);

		LLVMBuildBr(context->builder, merge_block2);
		then_block2 = LLVMGetInsertBlock(context->builder);

		LLVMPositionBuilderAtEnd(context->builder, else_block2);

		/**
		 * Destroy the body of the zval
		 */
		zephir_build_zval_dtor(context, value_ref);
		LLVMBuildBr(context->builder, merge_block2);
		else_block2 = LLVMGetInsertBlock(context->builder);

		LLVMPositionBuilderAtEnd(context->builder, merge_block2);

		/**
		 * Initialize variable to null
		 */
		zephir_build_zval_null(context, value_ref);
	}

	LLVMBuildBr(context->builder, merge_block1);
	then_block1 = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, else_block1);

	/**
	 * Variable does not have memory and it is not tracked
	 */
	zephir_build_memory_alloc(context, value_ref);

	LLVMBuildBr(context->builder, merge_block1);
	else_block1 = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, merge_block1);

}

/**
 * Builds a ZEPHIR_CPY_WRT call
 */
void zephir_build_copy_on_write(zephir_context *context, LLVMValueRef symbol_ref, LLVMValueRef value_ref)
{
	LLVMBasicBlockRef then_block1, else_block1, merge_block1, then_block2, else_block2, merge_block2;
	LLVMValueRef current_function;

	current_function = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	then_block1 = LLVMAppendBasicBlock(current_function, "then");
	else_block1 = LLVMAppendBasicBlock(current_function, "else");
	merge_block1 = LLVMAppendBasicBlock(current_function, "merge-if");

	/**
	 * Check if the pointer is not NULL
	 */
	LLVMBuildCondBr(context->builder, LLVMBuildIsNotNull(context->builder, LLVMBuildLoad(context->builder, symbol_ref, ""), ""), then_block1, else_block1);

	LLVMPositionBuilderAtEnd(context->builder, then_block1);

	{

		then_block2 = LLVMAppendBasicBlock(current_function, "then");
		else_block2 = LLVMAppendBasicBlock(current_function, "else");
		merge_block2 = LLVMAppendBasicBlock(current_function, "merge-if");

		/**
		 * Check if the refcount is greater than 1
		 */
		LLVMBuildCondBr(context->builder,
			LLVMBuildICmp(
				context->builder,
				LLVMIntUGT,
				zephir_build_zval_refcount(context, symbol_ref),
				LLVMConstInt(LLVMInt32Type(), 0, 0),
				""
			),
			then_block2,
			else_block2
		);

		LLVMPositionBuilderAtEnd(context->builder, then_block2);

		/**
		 * Reduce refcount or destroy zval
		 */
		zephir_build_zval_ptr_dtor(context, symbol_ref);

		LLVMBuildBr(context->builder, merge_block2);
		then_block2 = LLVMGetInsertBlock(context->builder);

		LLVMPositionBuilderAtEnd(context->builder, else_block2);
		LLVMBuildBr(context->builder, merge_block2);
		else_block2 = LLVMGetInsertBlock(context->builder);

		LLVMPositionBuilderAtEnd(context->builder, merge_block2);
	}

	LLVMBuildBr(context->builder, merge_block1);
	then_block1 = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, else_block1);

	/**
	 * Variable does not have memory and it is not tracked
	 */
	zephir_build_memory_observe(context, symbol_ref);

	LLVMBuildBr(context->builder, merge_block1);
	else_block1 = LLVMGetInsertBlock(context->builder);

	LLVMPositionBuilderAtEnd(context->builder, merge_block1);

	/**
	 * Increase refcount of value
	 */
	zephir_build_zval_addref(context, value_ref);

  	// load %struct._zval_struct** %2, align 8
	LLVMBuildStore(context->builder, LLVMBuildLoad(context->builder, value_ref, ""), symbol_ref); // store i64 5000, i64* %8, align 8
}

/**
 * Builds a call to zend_is_true()
 */
LLVMValueRef zephir_build_zend_is_true(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "zend_is_true");
	if (!function) {

		arg_tys[0] = context->types.zval_pointer_type;
		function = LLVMAddFunction(context->module, "zend_is_true", LLVMFunctionType(LLVMInt32Type(), arg_tys, 1, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zend_is_true");
		}

		LLVMAddGlobalMapping(context->engine, function, zend_is_true);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = LLVMBuildLoad(context->builder, value_ref, "");
	return LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to ZVAL_NULL()
 */
void zephir_build_zval_null(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef indicest[2];
	LLVMValueRef ref, ptr;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 2, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 0
	LLVMBuildStore(context->builder, LLVMConstInt(LLVMInt8Type(), IS_NULL, 0), ref); // store i8 1, i8* %7, align 1
}

/**
 * Builds a call to ZVAL_LONG()
 */
void zephir_build_zval_long(zephir_context *context, LLVMValueRef symbol_ref, LLVMValueRef value_ref) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr, bitcast;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 2, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 0
	LLVMBuildStore(context->builder, LLVMConstInt(LLVMInt8Type(), IS_LONG, 0), ref); // store i8 1, i8* %7, align 1

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indices[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indices[1] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indices, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 0
#if ZEPHIR_32
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt32Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i32*
	LLVMBuildStore(context->builder, value_ref, bitcast); // store i32 5000, i32* %8, align 8
#else
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt64Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i64*
	LLVMBuildStore(context->builder, value_ref, bitcast); // store i64 5000, i64* %8, align 8
#endif

}

/**
 * Builds a call to ZVAL_DOUBLE()
 */
void zephir_build_zval_double(zephir_context *context, LLVMValueRef symbol_ref, LLVMValueRef value_ref) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr, bitcast;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 2, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 0
	LLVMBuildStore(context->builder, LLVMConstInt(LLVMInt8Type(), IS_DOUBLE, 0), ref); // store i8 1, i8* %7, align 1

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indices[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indices[1] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indices, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 0

	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMDoubleType(), 0), ""); //bitcast %union._zvalue_value* %5 to double*
	LLVMBuildStore(context->builder, value_ref, bitcast); // store i64 5000, i64* %8, align 8

}

/**
 * Builds a call to ZVAL_BOOL()
 */
void zephir_build_zval_bool(zephir_context *context, LLVMValueRef symbol_ref, LLVMValueRef value_ref) {

	LLVMValueRef indicest[2], indices[2];
	LLVMValueRef ref, ptr, bitcast, cast;

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 2, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 0
	LLVMBuildStore(context->builder, LLVMConstInt(LLVMInt8Type(), IS_BOOL, 0), ref); // store i8 1, i8* %7, align 1

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indices[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indices[1] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indices, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 0

#if ZEPHIR_32
	cast = LLVMBuildZExt(context->builder, value_ref, LLVMInt32Type(), ""); // zext i8 %4 to i32 //zext i8 %8 to i64
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt32Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i32*
	LLVMBuildStore(context->builder, cast, bitcast); // store i32 5000, i32* %8, align 8
#else
	cast = LLVMBuildZExt(context->builder, value_ref, LLVMInt64Type(), ""); // zext i8 %4 to i32 //zext i8 %8 to i64
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt64Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i64*
	LLVMBuildStore(context->builder, cast, bitcast); // store i64 5000, i64* %8, align 8
#endif

}

/**
 * Builds a call to zephir_build_zephir_get_intval_ex
 */
LLVMValueRef zephir_build_zephir_get_intval_ex(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "zephir_get_intval_ex");
	if (!function) {

		arg_tys[0] = context->types.zval_pointer_type;
#if ZEPHIR_32
		function = LLVMAddFunction(context->module, "zephir_get_intval_ex", LLVMFunctionType(LLVMInt32Type(), arg_tys, 1, 0));
#else
		function = LLVMAddFunction(context->module, "zephir_get_intval_ex", LLVMFunctionType(LLVMInt64Type(), arg_tys, 1, 0));
#endif
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephir_get_intval_ex");
		}

		LLVMAddGlobalMapping(context->engine, function, zephir_get_intval_ex);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = LLVMBuildLoad(context->builder, value_ref, "");
	return LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to zephir_build_zephir_get_boolval_ex
 */
LLVMValueRef zephir_build_zephir_get_boolval_ex(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "zephir_get_boolval_ex");
	if (!function) {

		arg_tys[0] = context->types.zval_pointer_type;
		function = LLVMAddFunction(context->module, "zephir_get_boolval_ex", LLVMFunctionType(LLVMInt8Type(), arg_tys, 1, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephir_get_intval_ex");
		}

		LLVMAddGlobalMapping(context->engine, function, zephir_get_intval_ex);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = LLVMBuildLoad(context->builder, value_ref, "");
	return LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to zephir_build_zephir_get_doubleval_ex
 */
LLVMValueRef zephir_build_zephir_get_doubleval_ex(zephir_context *context, LLVMValueRef value_ref) {

	LLVMValueRef    function, args[2];
	LLVMTypeRef     arg_tys[2];

	function = LLVMGetNamedFunction(context->module, "zephir_get_doubleval_ex");
	if (!function) {

		arg_tys[0] = context->types.zval_pointer_type;
		function = LLVMAddFunction(context->module, "zephir_get_doubleval_ex", LLVMFunctionType(LLVMDoubleType(), arg_tys, 1, 0));
		if (!function) {
			zend_error(E_ERROR, "Cannot register zephir_get_doubleval_ex");
		}

		LLVMAddGlobalMapping(context->engine, function, zephir_get_doubleval_ex);
		LLVMSetFunctionCallConv(function, LLVMCCallConv);
		LLVMAddFunctionAttr(function, LLVMNoUnwindAttribute);
	}

	args[0] = LLVMBuildLoad(context->builder, value_ref, "");
	return LLVMBuildCall(context->builder, function, args, 1, "");
}

/**
 * Builds a call to zephir_get_intval
 */
LLVMValueRef zephir_build_get_intval(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef ptr, type, ref, cast, condition, then_value, else_value, bitcast, phi;
	LLVMValueRef indicest[2], indices[2];

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 2, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 2
	type = LLVMBuildLoad(context->builder, ref, ""); // load i8* %3, align 1
	cast = LLVMBuildZExt(context->builder, type, LLVMInt32Type(), ""); // zext i8 %4 to i32
	condition = LLVMBuildICmp(context->builder, LLVMIntEQ, // icmp eq i32 %5, 1
		cast,
		LLVMConstInt(LLVMInt32Type(), IS_LONG, 0),
		""
	);

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(func, "then");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(func, "else");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-if");

	LLVMBuildCondBr(context->builder, condition, then_block, else_block); // br i1 %6, label %7, label %12

	/**
	 * Return zvalue.lval member
	 */
	LLVMPositionBuilderAtEnd(context->builder, then_block);
	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %1, align 8
	indices[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indices[1] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indices, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 2

#if ZEPHIR_32
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt32Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i32*
#else
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt64Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i64*
#endif

	then_value = LLVMBuildLoad(context->builder, bitcast, ""); // load i64* %10, align 8

	LLVMBuildBr(context->builder, merge_block);
	then_block = LLVMGetInsertBlock(context->builder);

	/**
	 * Or, call zephir_get_intval_ex
	 */
	LLVMPositionBuilderAtEnd(context->builder, else_block);
	else_value = zephir_build_zephir_get_intval_ex(context, symbol_ref);
	LLVMBuildBr(context->builder, merge_block);
	else_block = LLVMGetInsertBlock(context->builder);

	/**
	 * Merge the resulting value using a phi
	 */
	LLVMPositionBuilderAtEnd(context->builder, merge_block);
#if ZEPHIR_32
	phi = LLVMBuildPhi(context->builder, LLVMInt32Type(), ""); //phi i64 [ %11, %7 ], [ %14, %12 ]
#else
	phi = LLVMBuildPhi(context->builder, LLVMInt64Type(), ""); //phi i64 [ %11, %7 ], [ %14, %12 ]
#endif
	LLVMAddIncoming(phi, &then_value, &then_block, 1);
	LLVMAddIncoming(phi, &else_value, &else_block, 1);

	return phi;
}

/**
 * Builds a call to zephir_get_boolval
 */
LLVMValueRef zephir_build_get_boolval(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef ptr, type, ref, cast, condition, then_value, else_value, bitcast, phi, boolval_ex;
	LLVMValueRef indicest[2], indices[2];

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 2, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 2
	type = LLVMBuildLoad(context->builder, ref, ""); // load i8* %3, align 1
	cast = LLVMBuildZExt(context->builder, type, LLVMInt32Type(), ""); // zext i8 %4 to i32
	condition = LLVMBuildICmp(context->builder, LLVMIntEQ, // icmp eq i32 %5, 1
		cast,
		LLVMConstInt(LLVMInt32Type(), IS_BOOL, 0),
		""
	);

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(func, "then");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(func, "else");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-if");

	LLVMBuildCondBr(context->builder, condition, then_block, else_block); // br i1 %6, label %7, label %12

	/**
	 * Return zvalue.lval member
	 */
	LLVMPositionBuilderAtEnd(context->builder, then_block);
	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %1, align 8
	indices[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indices[1] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indices, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 2

#if ZEPHIR_32
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt32Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i32*
#else
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMInt64Type(), 0), ""); //bitcast %union._zvalue_value* %7 to i64*
#endif

	then_value = LLVMBuildLoad(context->builder, bitcast, ""); // load i64* %10, align 8

	LLVMBuildBr(context->builder, merge_block);
	then_block = LLVMGetInsertBlock(context->builder);

	/**
	 * Or, call zephir_get_boolval_ex
	 */
	LLVMPositionBuilderAtEnd(context->builder, else_block);
	boolval_ex = zephir_build_zephir_get_boolval_ex(context, symbol_ref);
#if ZEPHIR_32
	else_value = LLVMBuildZExt(context->builder, boolval_ex, LLVMInt32Type(), ""); // zext i8 %18 to i64
#else
	else_value = LLVMBuildZExt(context->builder, boolval_ex, LLVMInt64Type(), ""); // zext i8 %18 to i64
#endif
	LLVMBuildBr(context->builder, merge_block);
	else_block = LLVMGetInsertBlock(context->builder);

	/**
	 * Merge the resulting value using a phi
	 */
	LLVMPositionBuilderAtEnd(context->builder, merge_block);
#if ZEPHIR_32
	phi = LLVMBuildPhi(context->builder, LLVMInt32Type(), ""); //phi i64 [ %11, %7 ], [ %14, %12 ]
#else
	phi = LLVMBuildPhi(context->builder, LLVMInt64Type(), ""); //phi i64 [ %11, %7 ], [ %14, %12 ]
#endif
	LLVMAddIncoming(phi, &then_value, &then_block, 1);
	LLVMAddIncoming(phi, &else_value, &else_block, 1);

	return LLVMBuildTrunc(context->builder, phi, LLVMInt8Type(), "");
}

/**
 * Builds a call to zephir_get_doubleval
 */
LLVMValueRef zephir_build_get_doubleval(zephir_context *context, LLVMValueRef symbol_ref) {

	LLVMValueRef ptr, type, ref, cast, condition, then_value, else_value, bitcast, phi;
	LLVMValueRef indicest[2], indices[2];

	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %2, align 8
	indicest[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indicest[1] = LLVMConstInt(LLVMInt32Type(), 2, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indicest, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 2
	type = LLVMBuildLoad(context->builder, ref, ""); // load i8* %3, align 1
	cast = LLVMBuildZExt(context->builder, type, LLVMInt32Type(), ""); // zext i8 %4 to i32
	condition = LLVMBuildICmp(context->builder, LLVMIntEQ, // icmp eq i32 %5, 1
		cast,
		LLVMConstInt(LLVMInt32Type(), IS_DOUBLE, 0),
		""
	);

	LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(context->builder));

	LLVMBasicBlockRef then_block = LLVMAppendBasicBlock(func, "then");
	LLVMBasicBlockRef else_block = LLVMAppendBasicBlock(func, "else");
	LLVMBasicBlockRef merge_block = LLVMAppendBasicBlock(func, "merge-if");

	LLVMBuildCondBr(context->builder, condition, then_block, else_block); // br i1 %6, label %7, label %12

	/**
	 * Return zvalue.dval member if type is IS_DOUBLE
	 */
	LLVMPositionBuilderAtEnd(context->builder, then_block);
	ptr = LLVMBuildLoad(context->builder, symbol_ref, ""); // load %struct._zval_struct** %1, align 8
	indices[0] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	indices[1] = LLVMConstInt(LLVMInt32Type(), 0, 0);
	ref = LLVMBuildInBoundsGEP(context->builder, ptr, indices, 2, ""); // getelementptr inbounds %struct._zval_struct* %6, i32 0, i32 2
	bitcast = LLVMBuildBitCast(context->builder, ref, LLVMPointerType(LLVMDoubleType(), 0), ""); //bitcast %union._zvalue_value* %7 to double*
	then_value = LLVMBuildLoad(context->builder, bitcast, ""); // load i64* %10, align 8

	LLVMBuildBr(context->builder, merge_block);
	then_block = LLVMGetInsertBlock(context->builder);

	/**
	 * Or, call zephir_get_doubleval_ex
	 */
	LLVMPositionBuilderAtEnd(context->builder, else_block);
	else_value = zephir_build_zephir_get_doubleval_ex(context, symbol_ref);
	LLVMBuildBr(context->builder, merge_block);
	else_block = LLVMGetInsertBlock(context->builder);

	/**
	 * Merge the resulting value using a phi
	 */
	LLVMPositionBuilderAtEnd(context->builder, merge_block);
	phi = LLVMBuildPhi(context->builder, LLVMDoubleType(), ""); //phi i64 [ %11, %7 ], [ %14, %12 ]
	LLVMAddIncoming(phi, &then_value, &then_block, 1);
	LLVMAddIncoming(phi, &else_value, &else_block, 1);

	return phi;
}

/**
 * Builds a RETURN_LONG
 */
void zephir_build_return_long(zephir_context *context, LLVMValueRef value_ref, zval *location) {

	zephir_variable *symbol_variable;

	symbol_variable = zephir_symtable_get_variable_for_read(context->symtable, SL("return_value"), context, location);

	zephir_build_zval_long(context, symbol_variable->value_ref, value_ref);
}

/**
 * Builds a RETURN_DOUBLE
 */
void zephir_build_return_double(zephir_context *context, LLVMValueRef value_ref, zval *location) {

	zephir_variable *symbol_variable;

	symbol_variable = zephir_symtable_get_variable_for_read(context->symtable, SL("return_value"), context, location);

	zephir_build_zval_double(context, symbol_variable->value_ref, value_ref);
}

/**
 * Builds a RETURN_BOOL
 */
void zephir_build_return_bool(zephir_context *context, LLVMValueRef value_ref, zval *location) {

	zephir_variable *symbol_variable;

	symbol_variable = zephir_symtable_get_variable_for_read(context->symtable, SL("return_value"), context, location);

	zephir_build_zval_bool(context, symbol_variable->value_ref, value_ref);
}
