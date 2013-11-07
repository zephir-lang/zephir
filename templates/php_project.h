

#ifndef PHP_%PROJECT_UPPER%_H
#define PHP_%PROJECT_UPPER%_H 1

#include "kernel/globals.h"

#define PHP_%PROJECT_UPPER%_VERSION "%PROJECT_VERSION%"
#define PHP_%PROJECT_UPPER%_EXTNAME "%PROJECT_EXTNAME%"

%EXTENSION_STRUCT_GLOBALS%

ZEND_BEGIN_MODULE_GLOBALS(%PROJECT_LOWER%)

	/* Memory */
	zephir_memory_entry *start_memory;
	zephir_memory_entry *active_memory;

	/* Virtual Symbol Tables */
	zephir_symbol_table *active_symbol_table;

	/* Function cache */
	HashTable *function_cache;

	/* Max recursion control */
	unsigned int recursive_lock;

	/* Global constants */
	zval *global_true;
	zval *global_false;
	zval *global_null;
	%EXTENSION_GLOBALS%
ZEND_END_MODULE_GLOBALS(%PROJECT_LOWER%)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(%PROJECT_LOWER%)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) TSRMG(%PROJECT_LOWER%_globals_id, zend_%PROJECT_LOWER%_globals *, v)
#else
	#define ZEPHIR_GLOBAL(v) (%PROJECT_LOWER%_globals.v)
#endif

#ifdef ZTS
	#define ZEPHIR_VGLOBAL ((zend_%PROJECT_LOWER%_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(%PROJECT_LOWER%_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(%PROJECT_LOWER%_globals)
#endif

#define zephir_globals %PROJECT_LOWER%_globals
#define zend_zephir_globals zend_%PROJECT_LOWER%_globals

extern zend_module_entry %PROJECT_LOWER%_module_entry;
#define phpext_%PROJECT_LOWER%_ptr &%PROJECT_LOWER%_module_entry

#endif
