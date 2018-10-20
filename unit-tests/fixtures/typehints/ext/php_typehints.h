
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_TYPEHINTS_H
#define PHP_TYPEHINTS_H 1

#ifdef PHP_WIN32
#define ZEPHIR_RELEASE 1
#endif

#include "kernel/globals.h"

#define PHP_TYPEHINTS_NAME        "TypeHints Test Extension"
#define PHP_TYPEHINTS_VERSION     "1.0.0"
#define PHP_TYPEHINTS_EXTNAME     "typehints"
#define PHP_TYPEHINTS_AUTHOR      "Zephir Team"
#define PHP_TYPEHINTS_ZEPVERSION  "0.11.2-4b7d4ed670"
#define PHP_TYPEHINTS_DESCRIPTION "Description test for<br/>TypeHints Test Extension"



ZEND_BEGIN_MODULE_GLOBALS(typehints)

	int initialized;

	/* Memory */
	zephir_memory_entry *start_memory; /**< The first preallocated frame */
	zephir_memory_entry *end_memory; /**< The last preallocate frame */
	zephir_memory_entry *active_memory; /**< The current memory frame */

	/* Virtual Symbol Tables */
	zephir_symbol_table *active_symbol_table;

	/** Function cache */
	HashTable *fcache;

	zephir_fcall_cache_entry *scache[ZEPHIR_MAX_CACHE_SLOTS];

	/* Cache enabled */
	unsigned int cache_enabled;

	/* Max recursion control */
	unsigned int recursive_lock;

	
ZEND_END_MODULE_GLOBALS(typehints)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(typehints)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) ZEND_MODULE_GLOBALS_ACCESSOR(typehints, v)
#else
	#define ZEPHIR_GLOBAL(v) (typehints_globals.v)
#endif

#ifdef ZTS
	void ***tsrm_ls;
	#define ZEPHIR_VGLOBAL ((zend_typehints_globals *) (*((void ***) tsrm_get_ls_cache()))[TSRM_UNSHUFFLE_RSRC_ID(typehints_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(typehints_globals)
#endif

#define ZEPHIR_API ZEND_API

#define zephir_globals_def typehints_globals
#define zend_zephir_globals_def zend_typehints_globals

extern zend_module_entry typehints_module_entry;
#define phpext_typehints_ptr &typehints_module_entry

#endif
