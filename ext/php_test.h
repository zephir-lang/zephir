
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
#define PHP_TEST_ZEPVERSION  "0.12.3-$Id$"
#define PHP_TEST_DESCRIPTION "Description <b>test</b> for<br/>Test Extension."

typedef struct _zephir_struct_db { 
	zend_bool my_setting_1;
	int my_setting_2;
	double my_setting_3;
} zephir_struct_db;

typedef struct _zephir_struct_orm { 
	int cache_level;
	zend_bool cache_enable;
} zephir_struct_orm;

typedef struct _zephir_struct_extension { 
	zend_bool test_ini_variable;
} zephir_struct_extension;



ZEND_BEGIN_MODULE_GLOBALS(test)

	int initialized;

	/** Function cache */
	HashTable *fcache;

	zephir_fcall_cache_entry *scache[ZEPHIR_MAX_CACHE_SLOTS];

	/* Cache enabled */
	unsigned int cache_enabled;

	/* Max recursion control */
	unsigned int recursive_lock;

	
	zephir_struct_db db;

	zephir_struct_orm orm;

	zephir_struct_extension extension;

	zend_bool my_setting_1;

	zend_bool test_setting_1;

	int my_setting_2;

	double my_setting_3;

	char my_setting_4;


ZEND_END_MODULE_GLOBALS(test)

#ifdef ZTS
#include "TSRM.h"
#endif

ZEND_EXTERN_MODULE_GLOBALS(test)

#ifdef ZTS
	#define ZEPHIR_GLOBAL(v) ZEND_MODULE_GLOBALS_ACCESSOR(test, v)
#else
	#define ZEPHIR_GLOBAL(v) (test_globals.v)
#endif

#ifdef ZTS
	void ***tsrm_ls;
	#define ZEPHIR_VGLOBAL ((zend_test_globals *) (*((void ***) tsrm_get_ls_cache()))[TSRM_UNSHUFFLE_RSRC_ID(test_globals_id)])
#else
	#define ZEPHIR_VGLOBAL &(test_globals)
#endif

#define ZEPHIR_API ZEND_API

#define zephir_globals_def test_globals
#define zend_zephir_globals_def zend_test_globals

extern zend_module_entry test_module_entry;
#define phpext_test_ptr &test_module_entry

#endif
