

#ifndef PHP_TEST_H
#define PHP_TEST_H 1

#include "kernel/globals.h"

#define PHP_TEST_VERSION "0.0.1"
#define PHP_TEST_EXTNAME "test"



ZEND_BEGIN_MODULE_GLOBALS(test)

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
	
ZEND_END_MODULE_GLOBALS(test)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(test)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) TSRMG(test_globals_id, zend_test_globals *, v)
#else
	#define ZEPHIR_GLOBAL(v) (test_globals.v)
#endif

#ifdef ZTS
	#define ZEPHIR_VGLOBAL ((zend_test_globals *) (*((void ***) tsrm_ls))[TSRM_UNSHUFFLE_RSRC_ID(test_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(test_globals)
#endif

#define zephir_globals test_globals
#define zend_zephir_globals zend_test_globals

extern zend_module_entry test_module_entry;
#define phpext_test_ptr &test_module_entry

#endif
