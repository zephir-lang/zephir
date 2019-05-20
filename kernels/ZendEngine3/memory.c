
/*
  +------------------------------------------------------------------------+
  | Zephir Language                                                        |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2017 Zephir Team (http://www.zephir-lang.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@zephir-lang.com so we can send you a copy immediately.      |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@zephir-lang.com>                     |
  |          Eduar Carvajal <eduar@zephir-lang.com>                        |
  |          Vladimir Kolesnikov <vladimir@extrememember.com>              |
  +------------------------------------------------------------------------+
*/

#include "php.h"
#include "php_ext.h"
#include "kernel/memory.h"

#include <Zend/zend_alloc.h>

#include "kernel/fcall.h"
#include "kernel/backtrace.h"

/*
 * Memory Frames/Virtual Symbol Scopes
 *------------------------------------
 *
 * Zephir uses memory frames to track the variables used within a method
 * in order to free them or reduce their reference counting accordingly before
 * exit the method in execution.
 *
 * This adds a minimum overhead to execution but save us the work of
 * free memory in each method manually.
 *
 * The whole memory frame is an open double-linked list which start is an
 * allocated empty frame that points to the real first frame. The start
 * memory frame is globally accessed using ZEPHIR_GLOBAL(start_frame)
 *
 * Not all methods must grow/restore the zephir_memory_entry.
 */

static zend_always_inline zend_execute_data* find_symbol_table(zend_execute_data* ex)
{
    while (ex && (!ex->func || !ZEND_USER_CODE(ex->func->common.type))) {
        ex = ex->prev_execute_data;
    }

    return ex;
}

/**
 * Adds a memory frame in the current executed method
 */
void ZEPHIR_FASTCALL zephir_memory_grow_stack(zephir_memory_def *zephir_memory_ptr)
{
	assert(zephir_memory_ptr != NULL);

	zephir_memory_ptr->active_entry = NULL;
	zephir_memory_ptr->active_symbol_table = NULL;
}

/**
 * Observes a memory pointer to release its memory at the end of the request
 */
void ZEPHIR_FASTCALL zephir_memory_observe(zephir_memory_def *zephir_memory_ptr, zval *var)
{
	if (var && Z_REFCOUNTED_P(var)) {
		zephir_memory_entry *entry;
		entry = (zephir_memory_entry*)emalloc(sizeof(zephir_memory_entry));
		entry->prev = zephir_memory_ptr->active_entry;
		ZVAL_COPY_VALUE(&entry->val, var);
		zephir_memory_ptr->active_entry = entry;
	}
}

/**
 * Finishes the current memory stack by releasing allocated memory
 */
void ZEPHIR_FASTCALL zephir_memory_restore_stack(zephir_memory_def *zephir_memory_ptr)
{
	assert(zephir_memory_ptr != NULL);
	
	if (zephir_memory_ptr->active_entry) {
		zephir_memory_entry *entry = zephir_memory_ptr->active_entry;
		while (entry) {
			zval_ptr_dtor(&entry->val);
			zephir_memory_ptr->active_entry = entry->prev;
			efree(entry);
			entry = zephir_memory_ptr->active_entry;
		}
	}

	if (EXPECTED(!CG(unclean_shutdown))) {
		/* Clean active symbol table */
		if (zephir_memory_ptr->active_symbol_table) {
			zephir_symbol_table *active_symbol_table = zephir_memory_ptr->active_symbol_table;
			while (active_symbol_table) {
				zend_execute_data *ex = find_symbol_table(EG(current_execute_data));
#ifndef ZEPHIR_RELEASE
				if (UNEXPECTED(!ex)) {
					fprintf(stderr, "ERROR: unable to find a symbol table");
					zephir_print_backtrace();
					return;
				}
#endif
				zend_hash_destroy(ex->symbol_table);
				efree(ex->symbol_table);
				ex->symbol_table = active_symbol_table->symbol_table;
				zend_attach_symbol_table(ex);
				zend_rebuild_symbol_table();

				zephir_memory_ptr->active_symbol_table = active_symbol_table->prev;
				efree(active_symbol_table);
				active_symbol_table = zephir_memory_ptr->active_symbol_table;
			}
		}
	}
}

/**
 * Creates a virtual symbol tables dynamically
 */
void zephir_create_symbol_table(zephir_memory_def *zephir_memory_ptr)
{
	zephir_symbol_table *entry;
	zend_array *symbol_table;

	zend_execute_data* ex = find_symbol_table(EG(current_execute_data));
#ifndef ZEPHIR_RELEASE
    if (UNEXPECTED(!ex)) {
        fprintf(stderr, "ERROR: unable to find a symbol table");
        zephir_print_backtrace();
        return;
    }
#endif

	zend_rebuild_symbol_table();
	zend_detach_symbol_table(ex);

	entry               = (zephir_symbol_table*)emalloc(sizeof(zephir_symbol_table));
	entry->symbol_table = ex->symbol_table;
	entry->prev         = zephir_memory_ptr->active_symbol_table;

	symbol_table = (zend_array*)emalloc(sizeof(zend_array));
	zend_hash_init(symbol_table, 0, NULL, ZVAL_PTR_DTOR, 0);
	zend_hash_real_init(symbol_table, 0);

	ex->symbol_table          = symbol_table;
	zephir_memory_ptr->active_symbol_table = entry;
}

/**
 * Exports symbols to the active symbol table
 */
int zephir_set_symbol(zval *key_name, zval *value)
{
	zend_array *symbol_table;

	symbol_table = zend_rebuild_symbol_table();
	if (!symbol_table) {
		php_error_docref(NULL, E_WARNING, "Cannot find a valid symbol_table");
		return FAILURE;
	}

	if (Z_TYPE_P(key_name) == IS_STRING) {
		Z_TRY_ADDREF_P(value);
		zend_hash_update(symbol_table, Z_STR_P(key_name), value);
	}

	return SUCCESS;
}

/**
 * Exports a string symbol to the active symbol table
 */
int zephir_set_symbol_str(char *key_name, unsigned int key_length, zval *value)
{
	zend_array *symbol_table = zend_rebuild_symbol_table();

	if (!symbol_table) {
		php_error_docref(NULL, E_WARNING, "Cannot find a valid symbol_table");
		return FAILURE;
	}

	Z_TRY_ADDREF_P(value);
	zend_hash_str_update(symbol_table, key_name, key_length, value);

	return SUCCESS;
}

/**
 * Cleans the function/method cache up
 */
int zephir_cleanup_fcache(void *pDest, int num_args, va_list args, zend_hash_key *hash_key)
{
	zephir_fcall_cache_entry **entry = (zephir_fcall_cache_entry**) pDest;
	zend_class_entry *scope;
	uint len = ZSTR_LEN(hash_key->key);

	assert(hash_key->key != NULL);
	assert(len > 2 * sizeof(zend_class_entry**));

	memcpy(&scope, &ZSTR_VAL(hash_key->key)[(len -1) - 2 * sizeof(zend_class_entry**)], sizeof(zend_class_entry*));

/*
#ifndef ZEPHIR_RELEASE
	{
		zend_class_entry *cls;
		memcpy(&cls, &hash_key->arKey[len - sizeof(zend_class_entry**)], sizeof(zend_class_entry*));

		fprintf(stderr, "func: %s, cls: %s, scope: %s [%u]\n", (*entry)->f->common.function_name, (cls ? cls->name : "N/A"), (scope ? scope->name : "N/A"), (uint)(*entry)->times);
	}
#endif
*/

	if ((*entry)->type != ZEND_INTERNAL_FUNCTION || (scope && scope->type != ZEND_INTERNAL_CLASS)) {
		return ZEND_HASH_APPLY_REMOVE;
	}

	if (scope && scope->type == ZEND_INTERNAL_CLASS && scope->info.internal.module->type != MODULE_PERSISTENT) {
		return ZEND_HASH_APPLY_REMOVE;
	}

	return ZEND_HASH_APPLY_KEEP;
}
