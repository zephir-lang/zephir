
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

#ifndef PHP_ZEPHIR_RUNTIME_MAIN_H
#define PHP_ZEPHIR_RUNTIME_MAIN_H 1

#include "scanner.h"

typedef struct _zephir_parser_token {
	int opcode;
	char *token;
	int token_len;
	int free_flag;
} zephir_parser_token;

typedef struct _zephir_parser_status {
	int status;
	zval *ret;
	zephir_scanner_state *scanner_state;
	zephir_scanner_token *token;
	char *syntax_error;
	unsigned int syntax_error_len;
	unsigned int number_brackets;
} zephir_parser_status;

typedef struct _zephir_symtable {
	HashTable *variables;
	unsigned int temp_variables;
} zephir_symtable;

typedef struct _zephir_variable {
	int type;
	const char *name;
	unsigned int name_length;
	unsigned char initialized;
	LLVMValueRef value_ref;
	unsigned int variant_inits;
} zephir_variable;

typedef struct _zephir_context {
	LLVMModuleRef module;
	LLVMBuilderRef builder;
	LLVMExecutionEngineRef engine;
	zephir_symtable *symtable;
	LLVMBasicBlockRef declarations_block;
	unsigned int inside_cycle;
	unsigned int inside_try_catch;
	unsigned int is_unrecheable;
	struct {
		LLVMTypeRef zval_type;
		LLVMTypeRef zval_pointer_type;
		LLVMTypeRef zval_double_pointer_type;
		LLVMTypeRef zvalue_type;
	} types;
} zephir_context;

typedef struct _zephir_compiled_expr {
	int type;
	LLVMValueRef value;
	zephir_variable *variable;
} zephir_compiled_expr;

#define ZEPHIR_PARSING_OK 1
#define ZEPHIR_PARSING_FAILED 0

int zephir_parse_program(zval **return_value, char *program, unsigned int program_length, const char *file_path, zval **error_message TSRMLS_DC);

#endif
