
/* This file was generated automatically by Zephir do not modify it! */

#ifndef PHP_TEST_H
#define PHP_TEST_H 1

#ifdef PHP_WIN32
#define ZEPHIR_RELEASE 1
#endif

#include "kernel/globals.h"

#define PHP_TEST_NAME        "Test Extension"
#define PHP_TEST_VERSION     "1.0.0"
#define PHP_TEST_EXTNAME     "test"
#define PHP_TEST_AUTHOR      "Zephir Team and contributors"
#define PHP_TEST_ZEPVERSION  "0.7.0a"
#define PHP_TEST_DESCRIPTION "Description test for<br/>Test Extension"

typedef struct _zephir_struct_test { 
	zend_bool my_setting_1;
	int my_setting_2;
	double my_setting_3;
} zephir_struct_test;



ZEND_BEGIN_MODULE_GLOBALS(test)

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

	/* Global constants */
	zval *global_true;
	zval *global_false;
	zval *global_null;
	
	zephir_struct_test test;

	zend_bool my_setting_1;

	int my_setting_2;

	double my_setting_3;


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

#define ZEPHIR_API ZEND_API

#define zephir_globals_def test_globals
#define zend_zephir_globals_def zend_test_globals

extern zend_module_entry test_module_entry;
#define phpext_test_ptr &test_module_entry

#endif
