
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#include "php_ext.h"
#include "lifecycle.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/globals.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"


#include "takedown_funcs.h"
#include "setup_funcs.h"
#include "prep_funcs.h"

zend_class_entry *lifecycle_test_ce;

ZEND_DECLARE_MODULE_GLOBALS(lifecycle)

PHP_INI_BEGIN()
	
PHP_INI_END()

static PHP_MINIT_FUNCTION(lifecycle)
{
	REGISTER_INI_ENTRIES();
	zephir_module_init();
	ZEPHIR_INIT(Lifecycle_Test);
	ext_setup_module();
	ext_prep_module();
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(lifecycle)
{
	ext_takedown_module();
	ext_kill_module();
	zephir_deinitialize_memory(TSRMLS_C);
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_lifecycle_globals *lifecycle_globals TSRMLS_DC)
{
	lifecycle_globals->initialized = 0;

	/* Cache Enabled */
	lifecycle_globals->cache_enabled = 1;

	/* Recursive Lock */
	lifecycle_globals->recursive_lock = 0;

	/* Static cache */
	memset(lifecycle_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);

	
	ext_setup_globals();
}

/**
 * Initialize globals only on each thread started
 */
static void php_zephir_init_module_globals(zend_lifecycle_globals *lifecycle_globals TSRMLS_DC)
{
	
}

static PHP_RINIT_FUNCTION(lifecycle)
{
	zend_lifecycle_globals *lifecycle_globals_ptr;
#ifdef ZTS
	tsrm_ls = ts_resource(0);
#endif
	lifecycle_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(lifecycle_globals_ptr TSRMLS_CC);
	zephir_initialize_memory(lifecycle_globals_ptr TSRMLS_CC);

	ext_setup_request();
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(lifecycle)
{
	ext_takedown_request();
	zephir_deinitialize_memory(TSRMLS_C);
	return SUCCESS;
}

#define ZEPHIR_POST_REQUEST 1
static PHP_PRSHUTDOWN_FUNCTION(lifecycle)
{
	ext_takedown_request();
}

static PHP_MINFO_FUNCTION(lifecycle)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_LIFECYCLE_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_LIFECYCLE_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_LIFECYCLE_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_LIFECYCLE_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_LIFECYCLE_ZEPVERSION);
	php_info_print_table_end();
	
	DISPLAY_INI_ENTRIES();
}

static PHP_GINIT_FUNCTION(lifecycle)
{
	php_zephir_init_globals(lifecycle_globals TSRMLS_CC);
	php_zephir_init_module_globals(lifecycle_globals TSRMLS_CC);
}

static PHP_GSHUTDOWN_FUNCTION(lifecycle)
{
	ext_takedown_globals();
}


zend_function_entry php_lifecycle_functions[] = {
	ZEND_FE_END

};

static const zend_module_dep php_lifecycle_deps[] = {
	
	ZEND_MOD_END
};

zend_module_entry lifecycle_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	php_lifecycle_deps,
	PHP_LIFECYCLE_EXTNAME,
	php_lifecycle_functions,
	PHP_MINIT(lifecycle),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(lifecycle),
#else
	NULL,
#endif
	PHP_RINIT(lifecycle),
	PHP_RSHUTDOWN(lifecycle),
	PHP_MINFO(lifecycle),
	PHP_LIFECYCLE_VERSION,
	ZEND_MODULE_GLOBALS(lifecycle),
	PHP_GINIT(lifecycle),
	PHP_GSHUTDOWN(lifecycle),
#ifdef ZEPHIR_POST_REQUEST
	PHP_PRSHUTDOWN(lifecycle),
#else
	NULL,
#endif
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_LIFECYCLE
ZEND_GET_MODULE(lifecycle)
#endif
