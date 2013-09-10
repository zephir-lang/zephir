
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_%PROJECT_LOWER%.h"
#include "%PROJECT_LOWER%.h"

#include "ext/standard/info.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"
#include "kernel/memory.h"

%CLASS_ENTRIES%

ZEND_DECLARE_MODULE_GLOBALS(%PROJECT_LOWER%)

PHP_MINIT_FUNCTION(%PROJECT_LOWER%){

	/** Init globals */
	ZEND_INIT_MODULE_GLOBALS(%PROJECT_LOWER%, php_zephir_init_globals, NULL);

	%CLASS_INITS%
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(%PROJECT_LOWER%){

	assert(ZEPHIR_GLOBAL(function_cache) == NULL);
	//assert(ZEPHIR_GLOBAL(orm).parser_cache == NULL);
	//assert(ZEPHIR_GLOBAL(orm).ast_cache == NULL);

	return SUCCESS;
}
#endif

static PHP_RINIT_FUNCTION(%PROJECT_LOWER%){

	php_zephir_init_globals(ZEPHIR_VGLOBAL TSRMLS_CC);
	//%PROJECT_LOWER%_init_interned_strings(TSRMLS_C);

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(%PROJECT_LOWER%){

	if (ZEPHIR_GLOBAL(start_memory) != NULL) {
		zephir_clean_restore_stack(TSRMLS_C);
	}

	if (ZEPHIR_GLOBAL(function_cache) != NULL) {
		zend_hash_destroy(ZEPHIR_GLOBAL(function_cache));
		FREE_HASHTABLE(ZEPHIR_GLOBAL(function_cache));
		ZEPHIR_GLOBAL(function_cache) = NULL;
	}

	return SUCCESS;
}

static PHP_MINFO_FUNCTION(%PROJECT_LOWER%)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Version", PHP_%PROJECT_UPPER%_VERSION);
	php_info_print_table_end();
}

static PHP_GINIT_FUNCTION(%PROJECT_LOWER%)
{
	zephir_memory_entry *start;

	php_zephir_init_globals(%PROJECT_LOWER%_globals TSRMLS_CC);

	start = (zephir_memory_entry *) pecalloc(1, sizeof(zephir_memory_entry), 1);
	start->addresses       = pecalloc(24, sizeof(zval*), 1);
	start->capacity        = 24;
	start->hash_addresses  = pecalloc(8, sizeof(zval*), 1);
	start->hash_capacity   = 8;

	%PROJECT_LOWER%_globals->start_memory = start;
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
