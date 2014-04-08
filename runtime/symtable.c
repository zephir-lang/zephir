
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
#include "variable.h"

zephir_symtable *zephir_symtable_new()
{
	zephir_symtable *symtable;

	symtable = emalloc(sizeof(zephir_symtable));
	symtable->variables = NULL;

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

zephir_variable *zephir_symtable_add(int type, const char *name, unsigned int name_length, zephir_context *context)
{
	zephir_variable *variable;
	zephir_symtable *symtable;

	variable = emalloc(sizeof(zephir_variable));
	variable->type = type;
	variable->name = zend_strndup(name, name_length);
	variable->name_length = name_length;
	variable->value_ref = NULL;

	symtable = context->symtable;
	if (symtable && !symtable->variables) {
		symtable->variables = emalloc(sizeof(HashTable));
		zend_hash_init(symtable->variables, 8, NULL, (dtor_func_t) zephir_symtable_dtor, 0);
	}

	zend_symtable_update(symtable->variables, name, name_length+1, &variable, sizeof(zephir_variable*), NULL);

	return variable;
}

int zephir_symtable_has(const char *name, unsigned int name_length, zephir_context *context)
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

zephir_variable *zephir_symtable_get_variable_for_write(zephir_symtable *symtable, const char *name, unsigned int name_length)
{
	zephir_variable *variable;

	if (_zephir_symtable_fetch_string(&variable, symtable->variables, name, name_length + 1 TSRMLS_CC) == FAILURE) {
		zend_error(E_ERROR, "Cannot read variable %s because it wasn't declared", name);
	}

	zephir_variable_incr_uses(variable);
	zephir_variable_incr_mutations(variable);

	return variable;
}

zephir_variable *zephir_symtable_get_variable_for_read(zephir_symtable *symtable, const char *name, unsigned int name_length)
{
	zephir_variable *variable;

	if (_zephir_symtable_fetch_string(&variable, symtable->variables, name, name_length + 1 TSRMLS_CC) == FAILURE) {
		zend_error(E_ERROR, "Cannot mutate variable %s because it wasn't defined", name);
	}

	return variable;
}
