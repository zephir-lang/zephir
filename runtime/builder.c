
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
#include "symtable.h"

#include "kernel/main.h"
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
		LLVMConstInt(LLVMInt32Type(), 1, 0),
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

void zephir_build_return_long(zephir_context *context, LLVMValueRef value_ref) {

	zephir_variable *symbol_variable;

	symbol_variable = zephir_symtable_get_variable_for_read(context->symtable, SL("return_value"));

	zephir_build_zval_long(context, symbol_variable->value_ref, value_ref);
}

void zephir_build_return_double(zephir_context *context, LLVMValueRef value_ref) {

	zephir_variable *symbol_variable;

	symbol_variable = zephir_symtable_get_variable_for_read(context->symtable, SL("return_value"));

	zephir_build_zval_double(context, symbol_variable->value_ref, value_ref);
}
