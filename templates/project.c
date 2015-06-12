
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#if PHP_VERSION_ID < 50500
#include <locale.h>
#endif

#include "php_ext.h"
#include "%PROJECT_LOWER_SAFE%.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/globals.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"

%EXTRA_INCLUDES%

%CLASS_ENTRIES%

ZEND_DECLARE_MODULE_GLOBALS(%PROJECT_LOWER%)

PHP_INI_BEGIN()
	%PROJECT_INI_ENTRIES%
PHP_INI_END()

static PHP_MINIT_FUNCTION(%PROJECT_LOWER%)
{
#if PHP_VERSION_ID < 50500
	char* old_lc_all = setlocale(LC_ALL, NULL);
	if (old_lc_all) {
		size_t len = strlen(old_lc_all);
		char *tmp  = calloc(len+1, 1);
		if (UNEXPECTED(!tmp)) {
			return FAILURE;
		}

		memcpy(tmp, old_lc_all, len);
		old_lc_all = tmp;
	}

	setlocale(LC_ALL, "C");
#endif
	REGISTER_INI_ENTRIES();
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

	zephir_deinitialize_memory(TSRMLS_C);
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_%PROJECT_LOWER%_globals *%PROJECT_LOWER%_globals TSRMLS_DC)
{
	%PROJECT_LOWER%_globals->initialized = 0;

	/* Memory options */
	%PROJECT_LOWER%_globals->active_memory = NULL;

	/* Virtual Symbol Tables */
	%PROJECT_LOWER%_globals->active_symbol_table = NULL;

	/* Cache Enabled */
	%PROJECT_LOWER%_globals->cache_enabled = 1;

	/* Recursive Lock */
	%PROJECT_LOWER%_globals->recursive_lock = 0;

	/* Static cache */
	memset(%PROJECT_LOWER%_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);

%INIT_GLOBALS%
}

static PHP_RINIT_FUNCTION(%PROJECT_LOWER%)
{

	zend_%PROJECT_LOWER%_globals *%PROJECT_LOWER%_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(%PROJECT_LOWER%_globals_ptr TSRMLS_CC);
	//zephir_init_interned_strings(TSRMLS_C);

	zephir_initialize_memory(%PROJECT_LOWER%_globals_ptr TSRMLS_CC);

%INITIALIZERS%
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{

	%DESTRUCTORS%

	zephir_deinitialize_memory(TSRMLS_C);
	return SUCCESS;
}

static PHP_MINFO_FUNCTION(%PROJECT_LOWER%)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_%PROJECT_UPPER%_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_%PROJECT_UPPER%_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_%PROJECT_UPPER%_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_%PROJECT_UPPER%_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_%PROJECT_UPPER%_ZEPVERSION);
	php_info_print_table_end();
%EXTENSION_INFO%
	DISPLAY_INI_ENTRIES();
}

static PHP_GINIT_FUNCTION(%PROJECT_LOWER%)
{
	php_zephir_init_globals(%PROJECT_LOWER%_globals TSRMLS_CC);
}

static PHP_GSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{

}

%FE_HEADER%
zend_function_entry php_%PROJECT_LOWER_SAFE%_functions[] = {
%FE_ENTRIES%
};

zend_module_entry %PROJECT_LOWER_SAFE%_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	PHP_%PROJECT_UPPER%_EXTNAME,
	php_%PROJECT_LOWER_SAFE%_functions,
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
ZEND_GET_MODULE(%PROJECT_LOWER_SAFE%)
#endif
