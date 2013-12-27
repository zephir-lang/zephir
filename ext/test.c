
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>
#include "php_ext.h"
#include "test.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/memory.h"

zend_class_entry *test_testinterface_ce;
zend_class_entry *test_constantsparent_ce;
zend_class_entry *test_oo_oodynamica_ce;
zend_class_entry *test_scallparent_ce;
zend_class_entry *test_arithmetic_ce;
zend_class_entry *test_assign_ce;
zend_class_entry *test_cast_ce;
zend_class_entry *test_cblock_ce;
zend_class_entry *test_constants_ce;
zend_class_entry *test_constantsinterface_ce;
zend_class_entry *test_declare_ce;
zend_class_entry *test_echoes_ce;
zend_class_entry *test_exception_ce;
zend_class_entry *test_exceptions_ce;
zend_class_entry *test_fannkuch_ce;
zend_class_entry *test_fcall_ce;
zend_class_entry *test_fibonnaci_ce;
zend_class_entry *test_flow_ce;
zend_class_entry *test_fortytwo_ce;
zend_class_entry *test_mcall_ce;
zend_class_entry *test_mcallchained_ce;
zend_class_entry *test_nativearray_ce;
zend_class_entry *test_oo_ce;
zend_class_entry *test_oo_ooconstruct_ce;
zend_class_entry *test_oo_ooconstructparams_ce;
zend_class_entry *test_oo_oodynamicb_ce;
zend_class_entry *test_oo_oonoconstruct_ce;
zend_class_entry *test_oo_ooparams_ce;
zend_class_entry *test_properties_privateproperties_ce;
zend_class_entry *test_properties_protectedproperties_ce;
zend_class_entry *test_properties_publicproperties_ce;
zend_class_entry *test_regexdna_ce;
zend_class_entry *test_returns_ce;
zend_class_entry *test_router_ce;
zend_class_entry *test_router_exception_ce;
zend_class_entry *test_router_route_ce;
zend_class_entry *test_scall_ce;
zend_class_entry *test_scallexternal_ce;
zend_class_entry *test_spectralnorm_ce;

ZEND_DECLARE_MODULE_GLOBALS(test)

PHP_MINIT_FUNCTION(test){

	ZEPHIR_INIT(Test_TestInterface);
	ZEPHIR_INIT(Test_ConstantsParent);
	ZEPHIR_INIT(Test_Oo_OoDynamicA);
	ZEPHIR_INIT(Test_ScallParent);
	ZEPHIR_INIT(Test_Arithmetic);
	ZEPHIR_INIT(Test_Assign);
	ZEPHIR_INIT(Test_Cast);
	ZEPHIR_INIT(Test_Cblock);
	ZEPHIR_INIT(Test_Constants);
	ZEPHIR_INIT(Test_ConstantsInterface);
	ZEPHIR_INIT(Test_Declare);
	ZEPHIR_INIT(Test_Echoes);
	ZEPHIR_INIT(Test_Exception);
	ZEPHIR_INIT(Test_Exceptions);
	ZEPHIR_INIT(Test_Fannkuch);
	ZEPHIR_INIT(Test_Fcall);
	ZEPHIR_INIT(Test_Fibonnaci);
	ZEPHIR_INIT(Test_Flow);
	ZEPHIR_INIT(Test_FortyTwo);
	ZEPHIR_INIT(Test_Mcall);
	ZEPHIR_INIT(Test_McallChained);
	ZEPHIR_INIT(Test_NativeArray);
	ZEPHIR_INIT(Test_Oo);
	ZEPHIR_INIT(Test_Oo_OoConstruct);
	ZEPHIR_INIT(Test_Oo_OoConstructParams);
	ZEPHIR_INIT(Test_Oo_OoDynamicB);
	ZEPHIR_INIT(Test_Oo_OoNoConstruct);
	ZEPHIR_INIT(Test_Oo_OoParams);
	ZEPHIR_INIT(Test_Properties_PrivateProperties);
	ZEPHIR_INIT(Test_Properties_ProtectedProperties);
	ZEPHIR_INIT(Test_Properties_PublicProperties);
	ZEPHIR_INIT(Test_RegexDNA);
	ZEPHIR_INIT(Test_Returns);
	ZEPHIR_INIT(Test_Router);
	ZEPHIR_INIT(Test_Router_Exception);
	ZEPHIR_INIT(Test_Router_Route);
	ZEPHIR_INIT(Test_Scall);
	ZEPHIR_INIT(Test_ScallExternal);
	ZEPHIR_INIT(Test_SpectralNorm);
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(test){

	assert(ZEPHIR_GLOBAL(function_cache) == NULL);
	//assert(ZEPHIR_GLOBAL(orm).parser_cache == NULL);
	//assert(ZEPHIR_GLOBAL(orm).ast_cache == NULL);

	return SUCCESS;
}
#endif

static PHP_RINIT_FUNCTION(test){

	php_zephir_init_globals(ZEPHIR_VGLOBAL TSRMLS_CC);
	//test_init_interned_strings(TSRMLS_C);

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(test){

	if (ZEPHIR_GLOBAL(start_memory) != NULL) {
		zephir_clean_restore_stack(TSRMLS_C);
	}

	if (ZEPHIR_GLOBAL(function_cache) != NULL) {
		zend_hash_destroy(ZEPHIR_GLOBAL(function_cache));
		FREE_HASHTABLE(ZEPHIR_GLOBAL(function_cache));
		ZEPHIR_GLOBAL(function_cache) = NULL;
	}

	return SUCCESS;
}

static PHP_MINFO_FUNCTION(test)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "Version", PHP_TEST_VERSION);
	php_info_print_table_end();
}

static PHP_GINIT_FUNCTION(test)
{
	zephir_memory_entry *start;

	php_zephir_init_globals(test_globals TSRMLS_CC);

	/* Start Memory Frame */
	start = (zephir_memory_entry *) pecalloc(1, sizeof(zephir_memory_entry), 1);
	start->addresses       = pecalloc(24, sizeof(zval*), 1);
	start->capacity        = 24;
	start->hash_addresses  = pecalloc(8, sizeof(zval*), 1);
	start->hash_capacity   = 8;

	test_globals->start_memory = start;

	/* Global Constants */
	ALLOC_PERMANENT_ZVAL(test_globals->global_false);
	INIT_PZVAL(test_globals->global_false);
	ZVAL_FALSE(test_globals->global_false);
	Z_ADDREF_P(test_globals->global_false);

	ALLOC_PERMANENT_ZVAL(test_globals->global_true);
	INIT_PZVAL(test_globals->global_true);
	ZVAL_TRUE(test_globals->global_true);
	Z_ADDREF_P(test_globals->global_true);

	ALLOC_PERMANENT_ZVAL(test_globals->global_null);
	INIT_PZVAL(test_globals->global_null);
	ZVAL_NULL(test_globals->global_null);
	Z_ADDREF_P(test_globals->global_null);
}

static PHP_GSHUTDOWN_FUNCTION(test)
{
	assert(test_globals->start_memory != NULL);

	pefree(test_globals->start_memory->hash_addresses, 1);
	pefree(test_globals->start_memory->addresses, 1);
	pefree(test_globals->start_memory, 1);
	test_globals->start_memory = NULL;
}

zend_module_entry test_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	NULL,
	PHP_TEST_EXTNAME,
	NULL,
	PHP_MINIT(test),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(test),
#else
	NULL,
#endif
	PHP_RINIT(test),
	PHP_RSHUTDOWN(test),
	PHP_MINFO(test),
	PHP_TEST_VERSION,
	ZEND_MODULE_GLOBALS(test),
	PHP_GINIT(test),
	PHP_GSHUTDOWN(test),
	NULL,
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_TEST
ZEND_GET_MODULE(test)
#endif
