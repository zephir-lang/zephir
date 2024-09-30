
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

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
	REGISTER_INI_ENTRIES();
	zephir_module_init();
	%CLASS_INITS%
	%MOD_INITIALIZERS%
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{
	%MOD_DESTRUCTORS%
	zephir_deinitialize_memory();
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_%PROJECT_LOWER%_globals *%PROJECT_LOWER%_globals)
{
	%PROJECT_LOWER%_globals->initialized = 0;

	/* Cache Enabled */
	%PROJECT_LOWER%_globals->cache_enabled = 1;

	/* Recursive Lock */
	%PROJECT_LOWER%_globals->recursive_lock = 0;

	/* Static cache */
	memset(%PROJECT_LOWER%_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);

	%INIT_GLOBALS%
}

/**
 * Initialize globals only on each thread started
 */
static void php_zephir_init_module_globals(zend_%PROJECT_LOWER%_globals *%PROJECT_LOWER%_globals)
{
	%INIT_MODULE_GLOBALS%
}

static PHP_RINIT_FUNCTION(%PROJECT_LOWER%)
{
	zend_%PROJECT_LOWER%_globals *%PROJECT_LOWER%_globals_ptr;
	%PROJECT_LOWER%_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(%PROJECT_LOWER%_globals_ptr);
	zephir_initialize_memory(%PROJECT_LOWER%_globals_ptr);

	%REQ_INITIALIZERS%
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{
	%REQ_DESTRUCTORS%
	zephir_deinitialize_memory();
	return SUCCESS;
}

%POSTREQ_DESTRUCTORS%

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
#if defined(COMPILE_DL_%PROJECT_UPPER%) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	php_zephir_init_globals(%PROJECT_LOWER%_globals);
	php_zephir_init_module_globals(%PROJECT_LOWER%_globals);
}

static PHP_GSHUTDOWN_FUNCTION(%PROJECT_LOWER%)
{
	%DESTROY_GLOBALS%
}

%FE_HEADER%
zend_function_entry php_%PROJECT_LOWER_SAFE%_functions[] = {
	%FE_ENTRIES%
};

static const zend_module_dep php_%PROJECT_LOWER_SAFE%_deps[] = {
	%PROJECT_DEPENDENCIES%
	ZEND_MOD_END
};

zend_module_entry %PROJECT_LOWER_SAFE%_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	php_%PROJECT_LOWER_SAFE%_deps,
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
#ifdef ZEPHIR_POST_REQUEST
	PHP_PRSHUTDOWN(%PROJECT_LOWER%),
#else
	NULL,
#endif
	STANDARD_MODULE_PROPERTIES_EX
};

/* implement standard "stub" routine to introduce ourselves to Zend */
#ifdef COMPILE_DL_%PROJECT_UPPER%
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(%PROJECT_LOWER_SAFE%)
#endif
