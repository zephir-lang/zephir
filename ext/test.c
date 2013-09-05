
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_test.h"
#include "test.h"

#include "Zend/zend_operators.h"
#include "Zend/zend_exceptions.h"
#include "Zend/zend_interfaces.h"

#include "kernel/main.h"

zend_class_entry *test_constantsparent_ce;
zend_class_entry *test_scallparent_ce;
zend_class_entry *test_arithmetic_ce;
zend_class_entry *test_assign_ce;
zend_class_entry *test_cast_ce;
zend_class_entry *test_constants_ce;
zend_class_entry *test_declare_ce;
zend_class_entry *test_echoes_ce;
zend_class_entry *test_exception_ce;
zend_class_entry *test_exceptions_ce;
zend_class_entry *test_fannkuch_ce;
zend_class_entry *test_fcall_ce;
zend_class_entry *test_fibonnaci_ce;
zend_class_entry *test_flow_ce;
zend_class_entry *test_mcall_ce;
zend_class_entry *test_nativearray_ce;
zend_class_entry *test_oo_ooconstruct_ce;
zend_class_entry *test_oo_ooconstructparams_ce;
zend_class_entry *test_oo_oonoconstruct_ce;
zend_class_entry *test_oo_ce;
zend_class_entry *test_properties_protectedproperties_ce;
zend_class_entry *test_properties_publicproperties_ce;
zend_class_entry *test_regexdna_ce;
zend_class_entry *test_router_exception_ce;
zend_class_entry *test_router_route_ce;
zend_class_entry *test_router_ce;
zend_class_entry *test_scall_ce;
zend_class_entry *test_spectralnorm_ce;

ZEND_DECLARE_MODULE_GLOBALS(test)

PHP_MINIT_FUNCTION(test){

	/** Init globals */
	ZEND_INIT_MODULE_GLOBALS(test, php_test_init_globals, NULL);

	ZEPHIR_INIT(Test_ConstantsParent);
	ZEPHIR_INIT(Test_ScallParent);
	ZEPHIR_INIT(Test_Arithmetic);
	ZEPHIR_INIT(Test_Assign);
	ZEPHIR_INIT(Test_Cast);
	ZEPHIR_INIT(Test_Constants);
	ZEPHIR_INIT(Test_Declare);
	ZEPHIR_INIT(Test_Echoes);
	ZEPHIR_INIT(Test_Exception);
	ZEPHIR_INIT(Test_Exceptions);
	ZEPHIR_INIT(Test_Fannkuch);
	ZEPHIR_INIT(Test_Fcall);
	ZEPHIR_INIT(Test_Fibonnaci);
	ZEPHIR_INIT(Test_Flow);
	ZEPHIR_INIT(Test_Mcall);
	ZEPHIR_INIT(Test_NativeArray);
	ZEPHIR_INIT(Test_Oo_OoConstruct);
	ZEPHIR_INIT(Test_Oo_OoConstructParams);
	ZEPHIR_INIT(Test_Oo_OoNoConstruct);
	ZEPHIR_INIT(Test_Oo);
	ZEPHIR_INIT(Test_Properties_ProtectedProperties);
	ZEPHIR_INIT(Test_Properties_PublicProperties);
	ZEPHIR_INIT(Test_RegexDNA);
	ZEPHIR_INIT(Test_Router_Exception);
	ZEPHIR_INIT(Test_Router_Route);
	ZEPHIR_INIT(Test_Router);
	ZEPHIR_INIT(Test_Scall);
	ZEPHIR_INIT(Test_SpectralNorm);
	return SUCCESS;
}


PHP_MSHUTDOWN_FUNCTION(test){

	/*if (TEST_GLOBAL(zephir_memory) != NULL) {
		zephir_clean_shutdown_stack(TSRMLS_C);
	}

	if (TEST_GLOBAL(zephir_cache) != NULL) {
		zend_hash_destroy(TEST_GLOBAL(function_cache));
		FREE_HASHTABLE(TEST_GLOBAL(function_cache));
		TEST_GLOBAL(function_cache) = NULL;
	}*/

	return SUCCESS;
}

PHP_RINIT_FUNCTION(test){

	//php_phalcon_init_globals(PHALCON_VGLOBAL TSRMLS_CC);

	return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(test){

	/*if (TEST_GLOBAL(zephir_memory) != NULL) {
		zephir_clean_shutdown_stack(TSRMLS_C);
	}

	if (TEST_GLOBAL(zephir_cache) != NULL) {
		zend_hash_destroy(TEST_GLOBAL(function_cache));
		FREE_HASHTABLE(TEST_GLOBAL(function_cache));
		TEST_GLOBAL(function_cache) = NULL;
	}*/

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

