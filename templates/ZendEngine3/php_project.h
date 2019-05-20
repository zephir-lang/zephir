
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_%PROJECT_UPPER%_H
#define PHP_%PROJECT_UPPER%_H 1

#ifdef PHP_WIN32
#define ZEPHIR_RELEASE 1
#endif

#include "kernel/globals.h"

#define PHP_%PROJECT_UPPER%_NAME        "%PROJECT_NAME%"
#define PHP_%PROJECT_UPPER%_VERSION     "%PROJECT_VERSION%"
#define PHP_%PROJECT_UPPER%_EXTNAME     "%PROJECT_EXTNAME%"
#define PHP_%PROJECT_UPPER%_AUTHOR      "%PROJECT_AUTHOR%"
#define PHP_%PROJECT_UPPER%_ZEPVERSION  "%PROJECT_ZEPVERSION%"
#define PHP_%PROJECT_UPPER%_DESCRIPTION "%PROJECT_DESCRIPTION%"

%EXTENSION_STRUCT_GLOBALS%

ZEND_BEGIN_MODULE_GLOBALS(%PROJECT_LOWER%)

	int initialized;

	/** Function cache */
	HashTable *fcache;

	zephir_fcall_cache_entry *scache[ZEPHIR_MAX_CACHE_SLOTS];

	/* Cache enabled */
	unsigned int cache_enabled;

	/* Max recursion control */
	unsigned int recursive_lock;

	%EXTENSION_GLOBALS%
ZEND_END_MODULE_GLOBALS(%PROJECT_LOWER%)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(%PROJECT_LOWER%)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) ZEND_MODULE_GLOBALS_ACCESSOR(%PROJECT_LOWER%, v)
#else
	#define ZEPHIR_GLOBAL(v) (%PROJECT_LOWER%_globals.v)
#endif

#ifdef ZTS
	void ***tsrm_ls;
	#define ZEPHIR_VGLOBAL ((zend_%PROJECT_LOWER%_globals *) (*((void ***) tsrm_get_ls_cache()))[TSRM_UNSHUFFLE_RSRC_ID(%PROJECT_LOWER%_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(%PROJECT_LOWER%_globals)
#endif

#define ZEPHIR_API ZEND_API

#define zephir_globals_def %PROJECT_LOWER%_globals
#define zend_zephir_globals_def zend_%PROJECT_LOWER%_globals

extern zend_module_entry %PROJECT_LOWER_SAFE%_module_entry;
#define phpext_%PROJECT_LOWER%_ptr &%PROJECT_LOWER_SAFE%_module_entry

#endif
