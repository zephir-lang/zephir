
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
#include "variable.h"
#include "errors.h"

/**
 * Creates a new symbol table
 */
zephir_symtable *zephir_symtable_new(TSRMLS_D)
{
	zephir_symtable *symtable;

	symtable = emalloc(sizeof(zephir_symtable));
	symtable->variables = NULL;
	symtable->temp_variables = 0;

	return symtable;
}

int zephir_symtable_dtor(void *pDest TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
	zephir_variable **variable = (zephir_variable**)pDest;

	return ZEND_HASH_APPLY_REMOVE;
}

int _zephir_symtable_fetch_string(zephir_variable **return_value, HashTable *arr, const char *index, uint index_length TSRMLS_DC) {

	zephir_variable **zv;

	if (zend_hash_find(arr, index, index_length, (void**) &zv) == SUCCESS) {
		*return_value = *zv;
		return SUCCESS;
	}

	*return_value = NULL;
	return FAILURE;
}

/**
 * Adds a symbol to the symbol table
 */
zephir_variable *zephir_symtable_add(int type, const char *name, unsigned int name_length, zephir_context *context TSRMLS_DC)
{
	zephir_variable *variable;
	zephir_symtable *symtable;

	variable = emalloc(sizeof(zephir_variable));
	variable->type = type;
	variable->name = zend_strndup(name, name_length);
	variable->name_length = name_length;
	variable->value_ref = NULL;
	variable->initialized = 0;
	variable->variant_inits = 0;

	symtable = context->symtable;
	if (symtable && !symtable->variables) {
		symtable->variables = emalloc(sizeof(HashTable));
		zend_hash_init(symtable->variables, 8, NULL, (dtor_func_t) zephir_symtable_dtor, 0);
	}

	zend_symtable_update(symtable->variables, name, name_length+1, &variable, sizeof(zephir_variable*), NULL);

	return variable;
}

/**
 * Check if a variable was added to the active symbol table
 */
int zephir_symtable_has(const char *name, unsigned int name_length, zephir_context *context TSRMLS_DC)
{
	zephir_variable *variable;

	if (!context->symtable || !context->symtable->variables) {
		return 0;
	}

	if (_zephir_symtable_fetch_string(&variable, context->symtable->variables, name, name_length + 1 TSRMLS_CC) == FAILURE) {
		return 0;
	}

	return 1;
}

/**
 * Obtains a variable for mutating
 */
zephir_variable *zephir_symtable_get_variable_for_write(zephir_symtable *symtable, const char *name, unsigned int name_length TSRMLS_DC)
{
	zephir_variable *variable;

	if (_zephir_symtable_fetch_string(&variable, symtable->variables, name, name_length + 1 TSRMLS_CC) == FAILURE) {
		zend_error(E_ERROR, "Cannot mutate variable \"%s\" because it wasn't defined", name);
	}

	zephir_variable_incr_uses(variable);
	zephir_variable_incr_mutations(variable);

	return variable;
}

/**
 * Obtains a variable for reading
 */
zephir_variable *zephir_symtable_get_variable_for_read(zephir_symtable *symtable, const char *name, unsigned int name_length, zephir_context *context, zval *location TSRMLS_DC)
{
	zephir_variable *variable;

	if (_zephir_symtable_fetch_string(&variable, symtable->variables, name, name_length + 1 TSRMLS_CC) == FAILURE) {
		zephir_error(location, "Cannot read variable \"%s\" because it wasn't declared", name);
	}

	if (!variable->initialized) {
		zephir_error(location, "Variable \"%s\" cannot be read because it's not initialized", name);
	}

	return variable;
}

/**
 * Creates a temporary variable
 */
zephir_variable *zephir_symtable_get_temp_variable_for_write(zephir_symtable *symtable, int type, zephir_context *context TSRMLS_DC)
{
	zephir_variable *symbol;
	char *buffer;
	unsigned int length;
	LLVMBasicBlockRef current_block;

	symtable->temp_variables++;

	buffer = emalloc(sizeof(char) * 7);
	length = snprintf(buffer, 7, "_%u", symtable->temp_variables);

	symbol = zephir_symtable_add(type, buffer, length, context);
	symbol->initialized = 1;

	switch (type) {

		case ZEPHIR_T_TYPE_VAR:
		case ZEPHIR_T_TYPE_STRING:
		case ZEPHIR_T_TYPE_ARRAY:

			current_block = LLVMGetInsertBlock(context->builder);
			LLVMPositionBuilderAtEnd(context->builder, context->declarations_block);

			symbol->value_ref = LLVMBuildAlloca(context->builder, context->types.zval_pointer_type, buffer);
			LLVMBuildStore(context->builder, LLVMConstPointerNull(context->types.zval_pointer_type), symbol->value_ref);

			LLVMPositionBuilderAtEnd(context->builder, current_block);
			zephir_variable_init_variant(symbol, context);
			break;
	}
	efree(buffer);

	zephir_variable_incr_uses(symbol);
	zephir_variable_incr_mutations(symbol);

	return symbol;
}
