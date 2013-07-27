
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_%PROJECT_LOWER%.h"
#include "%PROJECT_LOWER%.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"

%CLASS_ENTRIES%

ZEND_DECLARE_MODULE_GLOBALS(%PROJECT_LOWER%)

PHP_MINIT_FUNCTION(%PROJECT_LOWER%){

	/** Init globals */
	ZEND_INIT_MODULE_GLOBALS(%PROJECT_LOWER%, php_%PROJECT_LOWER%_init_globals, NULL);

	%CLASS_INITS%
	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(%PROJECT_LOWER%){

	/*if (PHALCON_GLOBAL(active_memory) != NULL) {
		phalcon_clean_shutdown_stack(TSRMLS_C);
	}

	if (PHALCON_GLOBAL(function_cache) != NULL) {
		zend_hash_destroy(PHALCON_GLOBAL(function_cache));
		FREE_HASHTABLE(PHALCON_GLOBAL(function_cache));
		PHALCON_GLOBAL(function_cache) = NULL;
	}*/

	return SUCCESS;
}

PHP_RINIT_FUNCTION(%PROJECT_LOWER%){

	//php_phalcon_init_globals(PHALCON_VGLOBAL TSRMLS_CC);

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(%PROJECT_LOWER%){

	/*if (PHALCON_GLOBAL(active_memory) != NULL) {
		phalcon_clean_shutdown_stack(TSRMLS_C);
	}

	if (PHALCON_GLOBAL(function_cache) != NULL) {
		zend_hash_destroy(PHALCON_GLOBAL(function_cache));
		FREE_HASHTABLE(PHALCON_GLOBAL(function_cache));
		PHALCON_GLOBAL(function_cache) = NULL;
	}

	phalcon_orm_destroy_cache(TSRMLS_C);*/

	return SUCCESS;
}

zend_module_entry %PROJECT_LOWER%_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_%PROJECT_UPPER%_EXTNAME,
	NULL,
	PHP_MINIT(%PROJECT_LOWER%),
	PHP_MSHUTDOWN(%PROJECT_LOWER%),
	PHP_RINIT(%PROJECT_LOWER%),
	PHP_RSHUTDOWN(%PROJECT_LOWER%),
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_%PROJECT_UPPER%_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_%PROJECT_UPPER%
ZEND_GET_MODULE(%PROJECT_LOWER%)
#endif

