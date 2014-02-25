
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#if PHP_VERSION_ID < 50500
#include <locale.h>
#endif

#include "php_ext.h"
#include "%PROJECT_LOWER%.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"

%CLASS_ENTRIES%

ZEND_DECLARE_MODULE_GLOBALS(%PROJECT_LOWER%)

static PHP_MINIT_FUNCTION(%PROJECT_LOWER%)
{
#if PHP_VERSION_ID < 50500
	const char* old_lc_all = setlocale(LC_ALL, NULL);
	if (old_lc_all) {
		char *tmp = calloc(strlen(old_lc_all)+1, 1);
		memcpy(tmp, old_lc_all, strlen(old_lc_all));
		old_lc_all = tmp;
	}

	setlocale(LC_ALL, "C");
#endif

	%CLASS_INITS%

#if PHP_VERSION_ID < 50500
	setlocale(LC_ALL, old_lc_all);
	free(old_lc_all);
#endif
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{

	assert(ZEPHIR_GLOBAL(fcache) == NULL);

	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_zephir_globals *zephir_globals TSRMLS_DC)
{

	/* Memory options */
	zephir_globals->active_memory = NULL;

	/* Virtual Symbol Tables */
	zephir_globals->active_symbol_table = NULL;

	/* Cache options */
	zephir_globals->fcache = NULL;

	/* Recursive Lock */
	zephir_globals->recursive_lock = 0;

%INIT_GLOBALS%
}

#ifndef ZEPHIR_RELEASE
static void zephir_fcall_cache_dtor(void *pData)
{
	zephir_fcall_cache_entry **entry = (zephir_fcall_cache_entry**)pData;
	free(*entry);
}
#endif

static int zephir_cleanup_fcache(void *pDest TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
	zephir_fcall_cache_entry **entry = (zephir_fcall_cache_entry**)pDest;
	zend_class_entry *scope;
	uint len = hash_key->nKeyLength;

	assert(hash_key->arKey != NULL);
	assert(hash_key->nKeyLength > 2*sizeof(zend_class_entry**));

	memcpy(&scope, &hash_key->arKey[len - 2*sizeof(zend_class_entry**)], sizeof(zend_class_entry*));

#ifndef ZEPHIR_RELEASE
if ((*entry)->f->type != ZEND_INTERNAL_FUNCTION || (scope && scope->type != ZEND_INTERNAL_CLASS)) {
	return ZEND_HASH_APPLY_REMOVE;
}
#else
if ((*entry)->type != ZEND_INTERNAL_FUNCTION || (scope && scope->type != ZEND_INTERNAL_CLASS)) {
	return ZEND_HASH_APPLY_REMOVE;
}
#endif

#if PHP_VERSION_ID >= 50400
if (scope && scope->type == ZEND_INTERNAL_CLASS && scope->info.internal.module->type != MODULE_PERSISTENT) {
	return ZEND_HASH_APPLY_REMOVE;
}
#else
if (scope && scope->type == ZEND_INTERNAL_CLASS && scope->module->type != MODULE_PERSISTENT) {
	return ZEND_HASH_APPLY_REMOVE;
}
#endif

	return ZEND_HASH_APPLY_KEEP;
}

static PHP_RINIT_FUNCTION(%PROJECT_LOWER%)
{

	php_zephir_init_globals(ZEPHIR_VGLOBAL TSRMLS_CC);
	//%PROJECT_LOWER%_init_interned_strings(TSRMLS_C);

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{

	if (ZEPHIR_GLOBAL(start_memory) != NULL) {
		zephir_clean_restore_stack(TSRMLS_C);
	}

	/*if (ZEPHIR_GLOBAL(function_cache) != NULL) {
		zend_hash_destroy(ZEPHIR_GLOBAL(function_cache));
		FREE_HASHTABLE(ZEPHIR_GLOBAL(function_cache));
		ZEPHIR_GLOBAL(function_cache) = NULL;
	}*/

	zend_hash_apply_with_arguments(ZEPHIR_GLOBAL(fcache) TSRMLS_CC, zephir_cleanup_fcache, 0);

	return SUCCESS;
}

static PHP_MINFO_FUNCTION(%PROJECT_LOWER%)
{
	php_info_print_table_start();
	php_info_print_table_header(2, PHP_%PROJECT_UPPER%_NAME, "enabled");
	php_info_print_table_row(2, "Version", PHP_%PROJECT_UPPER%_VERSION);
	php_info_print_table_end();

%EXTENSION_INFO%
}

static PHP_GINIT_FUNCTION(%PROJECT_LOWER%)
{
	zephir_memory_entry *start;

	php_zephir_init_globals(%PROJECT_LOWER%_globals TSRMLS_CC);

	/* Start Memory Frame */
	start = (zephir_memory_entry *) pecalloc(1, sizeof(zephir_memory_entry), 1);
	start->addresses       = pecalloc(16, sizeof(zval*), 1);
	start->capacity        = 16;
	start->hash_addresses  = pecalloc(4, sizeof(zval*), 1);
	start->hash_capacity   = 4;

	%PROJECT_LOWER%_globals->start_memory = start;

	%PROJECT_LOWER%_globals->fcache = pemalloc(sizeof(HashTable), 1);
#ifndef ZEPHIR_RELEASE
	zend_hash_init(%PROJECT_LOWER%_globals->fcache, 128, NULL, zephir_fcall_cache_dtor, 1);
#else
	zend_hash_init(%PROJECT_LOWER%_globals->fcache, 128, NULL, NULL, 1);
#endif

	/* Global Constants */
	ALLOC_PERMANENT_ZVAL(%PROJECT_LOWER%_globals->global_false);
	INIT_PZVAL(%PROJECT_LOWER%_globals->global_false);
	ZVAL_FALSE(%PROJECT_LOWER%_globals->global_false);
	Z_ADDREF_P(%PROJECT_LOWER%_globals->global_false);

	ALLOC_PERMANENT_ZVAL(%PROJECT_LOWER%_globals->global_true);
	INIT_PZVAL(%PROJECT_LOWER%_globals->global_true);
	ZVAL_TRUE(%PROJECT_LOWER%_globals->global_true);
	Z_ADDREF_P(%PROJECT_LOWER%_globals->global_true);

	ALLOC_PERMANENT_ZVAL(%PROJECT_LOWER%_globals->global_null);
	INIT_PZVAL(%PROJECT_LOWER%_globals->global_null);
	ZVAL_NULL(%PROJECT_LOWER%_globals->global_null);
	Z_ADDREF_P(%PROJECT_LOWER%_globals->global_null);

}

static PHP_GSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{
	assert(%PROJECT_LOWER%_globals->start_memory != NULL);

	pefree(%PROJECT_LOWER%_globals->start_memory->hash_addresses, 1);
	pefree(%PROJECT_LOWER%_globals->start_memory->addresses, 1);
	pefree(%PROJECT_LOWER%_globals->start_memory, 1);
	%PROJECT_LOWER%_globals->start_memory = NULL;
}

zend_module_entry %PROJECT_LOWER%_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	PHP_%PROJECT_UPPER%_EXTNAME,
	NULL,
	PHP_MINIT(%PROJECT_LOWER%),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(%PROJECT_LOWER%),
#else
	NULL,
#endif
	PHP_RINIT(%PROJECT_LOWER%),
	PHP_RSHUTDOWN(%PROJECT_LOWER%),
	PHP_MINFO(%PROJECT_LOWER%),
	PHP_%PROJECT_UPPER%_VERSION,
	ZEND_MODULE_GLOBALS(%PROJECT_LOWER%),
	PHP_GINIT(%PROJECT_LOWER%),
	PHP_GSHUTDOWN(%PROJECT_LOWER%),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_%PROJECT_UPPER%
ZEND_GET_MODULE(%PROJECT_LOWER%)
#endif
