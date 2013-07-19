
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

zend_class_entry *test_router_ce;

ZEND_DECLARE_MODULE_GLOBALS(test)

PHP_MINIT_FUNCTION(test){

	/** Init globals */
	//ZEND_INIT_MODULE_GLOBALS(phalcon, php_phalcon_init_globals, NULL);

	ZEPHIR_INIT(Test_Router);
	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(test){

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

PHP_RINIT_FUNCTION(test){

	//php_phalcon_init_globals(PHALCON_VGLOBAL TSRMLS_CC);

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(test){

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

zend_module_entry test_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_TEST_EXTNAME,
	NULL,
	PHP_MINIT(test),
	PHP_MSHUTDOWN(test),
	PHP_RINIT(test),
	PHP_RSHUTDOWN(test),
	NULL,
#if ZEND_MODULE_API_NO >= 20010901
	PHP_TEST_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_TEST
ZEND_GET_MODULE(test)
#endif

