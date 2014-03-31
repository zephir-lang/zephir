
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#if PHP_VERSION_ID < 50500
#include <locale.h>
#endif

#include "php_ext.h"
#include "test.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"

zend_class_entry *test_diinterface_ce;
zend_class_entry *test_testinterface_ce;
zend_class_entry *test_constantsparent_ce;
zend_class_entry *test_oo_oodynamica_ce;
zend_class_entry *test_scallparent_ce;
zend_class_entry *test_arithmetic_ce;
zend_class_entry *test_assign_ce;
zend_class_entry *test_bench_foo_ce;
zend_class_entry *test_bitwise_ce;
zend_class_entry *test_branchprediction_ce;
zend_class_entry *test_builtin_charmethods_ce;
zend_class_entry *test_builtin_intmethods_ce;
zend_class_entry *test_builtin_stringmethods_ce;
zend_class_entry *test_cast_ce;
zend_class_entry *test_cblock_ce;
zend_class_entry *test_compare_ce;
zend_class_entry *test_concat_ce;
zend_class_entry *test_constants_ce;
zend_class_entry *test_constantsinterface_ce;
zend_class_entry *test_constantsinterfacea_ce;
zend_class_entry *test_constantsinterfaceb_ce;
zend_class_entry *test_declaretest_ce;
zend_class_entry *test_echoes_ce;
zend_class_entry *test_emptytest_ce;
zend_class_entry *test_exception_ce;
zend_class_entry *test_exceptions_ce;
zend_class_entry *test_exists_ce;
zend_class_entry *test_factorial_ce;
zend_class_entry *test_fannkuch_ce;
zend_class_entry *test_fcall_ce;
zend_class_entry *test_fetchtest_ce;
zend_class_entry *test_fibonnaci_ce;
zend_class_entry *test_flow_ce;
zend_class_entry *test_fortytwo_ce;
zend_class_entry *test_globals_ce;
zend_class_entry *test_instanceoff_ce;
zend_class_entry *test_internalclasses_ce;
zend_class_entry *test_internalinterfaces_ce;
zend_class_entry *test_issettest_ce;
zend_class_entry *test_json_ce;
zend_class_entry *test_logical_ce;
zend_class_entry *test_mcall_ce;
zend_class_entry *test_mcallchained_ce;
zend_class_entry *test_methodargs_ce;
zend_class_entry *test_nativearray_ce;
zend_class_entry *test_oo_abstractclass_ce;
zend_class_entry *test_oo_ce;
zend_class_entry *test_oo_constantsinterface_ce;
zend_class_entry *test_oo_extendpdoclass_ce;
zend_class_entry *test_oo_ooconstruct_ce;
zend_class_entry *test_oo_ooconstructparams_ce;
zend_class_entry *test_oo_oodynamicb_ce;
zend_class_entry *test_oo_oonativeimplements_ce;
zend_class_entry *test_oo_oonoconstruct_ce;
zend_class_entry *test_oo_ooparams_ce;
zend_class_entry *test_oo_propertyaccess_ce;
zend_class_entry *test_pregmatch_ce;
zend_class_entry *test_properties_privateproperties_ce;
zend_class_entry *test_properties_protectedproperties_ce;
zend_class_entry *test_properties_publicproperties_ce;
zend_class_entry *test_properties_staticprotectedproperties_ce;
zend_class_entry *test_properties_staticpublicproperties_ce;
zend_class_entry *test_regexdna_ce;
zend_class_entry *test_requires_ce;
zend_class_entry *test_resourcetest_ce;
zend_class_entry *test_returns_ce;
zend_class_entry *test_router_ce;
zend_class_entry *test_router_exception_ce;
zend_class_entry *test_router_route_ce;
zend_class_entry *test_scall_ce;
zend_class_entry *test_scallexternal_ce;
zend_class_entry *test_spectralnorm_ce;
zend_class_entry *test_spropertyaccess_ce;
zend_class_entry *test_strings_ce;
zend_class_entry *test_ternary_ce;
zend_class_entry *test_trie_ce;
zend_class_entry *test_trytest_ce;
zend_class_entry *test_typeoff_ce;
zend_class_entry *test_unknownclass_ce;
zend_class_entry *test_unsettest_ce;
zend_class_entry *test_usetest_ce;
zend_class_entry *test_vars_ce;

ZEND_DECLARE_MODULE_GLOBALS(test)

static PHP_MINIT_FUNCTION(test)
{
#if PHP_VERSION_ID < 50500
	const char* old_lc_all = setlocale(LC_ALL, NULL);
	if (old_lc_all) {
		char *tmp = calloc(strlen(old_lc_all)+1, 1);
		memcpy(tmp, old_lc_all, strlen(old_lc_all));
		old_lc_all = tmp;
	}

	setlocale(LC_ALL, "C");
#endif

	ZEPHIR_INIT(Test_DiInterface);
	ZEPHIR_INIT(Test_TestInterface);
	ZEPHIR_INIT(Test_ConstantsParent);
	ZEPHIR_INIT(Test_Oo_OoDynamicA);
	ZEPHIR_INIT(Test_ScallParent);
	ZEPHIR_INIT(Test_Arithmetic);
	ZEPHIR_INIT(Test_Assign);
	ZEPHIR_INIT(Test_Bench_Foo);
	ZEPHIR_INIT(Test_Bitwise);
	ZEPHIR_INIT(Test_BranchPrediction);
	ZEPHIR_INIT(Test_BuiltIn_CharMethods);
	ZEPHIR_INIT(Test_BuiltIn_IntMethods);
	ZEPHIR_INIT(Test_BuiltIn_StringMethods);
	ZEPHIR_INIT(Test_Cast);
	ZEPHIR_INIT(Test_Cblock);
	ZEPHIR_INIT(Test_Compare);
	ZEPHIR_INIT(Test_Concat);
	ZEPHIR_INIT(Test_Constants);
	ZEPHIR_INIT(Test_ConstantsInterface);
	ZEPHIR_INIT(Test_ConstantsInterfaceA);
	ZEPHIR_INIT(Test_ConstantsInterfaceB);
	ZEPHIR_INIT(Test_DeclareTest);
	ZEPHIR_INIT(Test_Echoes);
	ZEPHIR_INIT(Test_EmptyTest);
	ZEPHIR_INIT(Test_Exception);
	ZEPHIR_INIT(Test_Exceptions);
	ZEPHIR_INIT(Test_Exists);
	ZEPHIR_INIT(Test_Factorial);
	ZEPHIR_INIT(Test_Fannkuch);
	ZEPHIR_INIT(Test_Fcall);
	ZEPHIR_INIT(Test_FetchTest);
	ZEPHIR_INIT(Test_Fibonnaci);
	ZEPHIR_INIT(Test_Flow);
	ZEPHIR_INIT(Test_FortyTwo);
	ZEPHIR_INIT(Test_Globals);
	ZEPHIR_INIT(Test_Instanceoff);
	ZEPHIR_INIT(Test_InternalClasses);
	ZEPHIR_INIT(Test_InternalInterfaces);
	ZEPHIR_INIT(Test_IssetTest);
	ZEPHIR_INIT(Test_Json);
	ZEPHIR_INIT(Test_Logical);
	ZEPHIR_INIT(Test_Mcall);
	ZEPHIR_INIT(Test_McallChained);
	ZEPHIR_INIT(Test_MethodArgs);
	ZEPHIR_INIT(Test_NativeArray);
	ZEPHIR_INIT(Test_Oo);
	ZEPHIR_INIT(Test_Oo_AbstractClass);
	ZEPHIR_INIT(Test_Oo_ConstantsInterface);
	ZEPHIR_INIT(Test_Oo_ExtendPdoClass);
	ZEPHIR_INIT(Test_Oo_OoConstruct);
	ZEPHIR_INIT(Test_Oo_OoConstructParams);
	ZEPHIR_INIT(Test_Oo_OoDynamicB);
	ZEPHIR_INIT(Test_Oo_OoNativeImplements);
	ZEPHIR_INIT(Test_Oo_OoNoConstruct);
	ZEPHIR_INIT(Test_Oo_OoParams);
	ZEPHIR_INIT(Test_Oo_PropertyAccess);
	ZEPHIR_INIT(Test_Pregmatch);
	ZEPHIR_INIT(Test_Properties_PrivateProperties);
	ZEPHIR_INIT(Test_Properties_ProtectedProperties);
	ZEPHIR_INIT(Test_Properties_PublicProperties);
	ZEPHIR_INIT(Test_Properties_StaticProtectedProperties);
	ZEPHIR_INIT(Test_Properties_StaticPublicProperties);
	ZEPHIR_INIT(Test_RegexDNA);
	ZEPHIR_INIT(Test_Requires);
	ZEPHIR_INIT(Test_ResourceTest);
	ZEPHIR_INIT(Test_Returns);
	ZEPHIR_INIT(Test_Router);
	ZEPHIR_INIT(Test_Router_Exception);
	ZEPHIR_INIT(Test_Router_Route);
	ZEPHIR_INIT(Test_SPropertyAccess);
	ZEPHIR_INIT(Test_Scall);
	ZEPHIR_INIT(Test_ScallExternal);
	ZEPHIR_INIT(Test_SpectralNorm);
	ZEPHIR_INIT(Test_Strings);
	ZEPHIR_INIT(Test_Ternary);
	ZEPHIR_INIT(Test_Trie);
	ZEPHIR_INIT(Test_TryTest);
	ZEPHIR_INIT(Test_Typeoff);
	ZEPHIR_INIT(Test_UnknownClass);
	ZEPHIR_INIT(Test_Unsettest);
	ZEPHIR_INIT(Test_UseTest);
	ZEPHIR_INIT(Test_Vars);

#if PHP_VERSION_ID < 50500
	setlocale(LC_ALL, old_lc_all);
	free(old_lc_all);
#endif
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(test)
{
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_zephir_globals *zephir_globals TSRMLS_DC)
{

	/* Memory options */
	zephir_globals->active_memory = NULL;

	/* Virtual Symbol Tables */
	zephir_globals->active_symbol_table = NULL;

	/* Recursive Lock */
	zephir_globals->recursive_lock = 0;

	zephir_globals->test.my_setting_1 = 1;
	zephir_globals->test.my_setting_2 = 100;
	zephir_globals->test.my_setting_3 = 7.5;
	zephir_globals->my_setting_1 = 1;
	zephir_globals->my_setting_2 = 10;
	zephir_globals->my_setting_3 = 15.2;

}

#ifndef ZEPHIR_RELEASE
static void zephir_fcall_cache_dtor(void *pData)
{
	zephir_fcall_cache_entry **entry = (zephir_fcall_cache_entry**)pData;
	free(*entry);
}
#endif

static int zephir_cleanup_fcache(void *pDest TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
	zephir_fcall_cache_entry **entry = (zephir_fcall_cache_entry**)pDest;
	zend_class_entry *scope;
	uint len = hash_key->nKeyLength;

	assert(hash_key->arKey != NULL);
	assert(hash_key->nKeyLength > 2*sizeof(zend_class_entry**));

	memcpy(&scope, &hash_key->arKey[len - 2*sizeof(zend_class_entry**)], sizeof(zend_class_entry*));

#ifndef ZEPHIR_RELEASE
if ((*entry)->f->type != ZEND_INTERNAL_FUNCTION || (scope && scope->type != ZEND_INTERNAL_CLASS)) {
	return ZEND_HASH_APPLY_REMOVE;
}
#else
if ((*entry)->type != ZEND_INTERNAL_FUNCTION || (scope && scope->type != ZEND_INTERNAL_CLASS)) {
	return ZEND_HASH_APPLY_REMOVE;
}
#endif

#if PHP_VERSION_ID >= 50400
if (scope && scope->type == ZEND_INTERNAL_CLASS && scope->info.internal.module->type != MODULE_PERSISTENT) {
	return ZEND_HASH_APPLY_REMOVE;
}
#else
if (scope && scope->type == ZEND_INTERNAL_CLASS && scope->module->type != MODULE_PERSISTENT) {
	return ZEND_HASH_APPLY_REMOVE;
}
#endif

	return ZEND_HASH_APPLY_KEEP;
}

static PHP_RINIT_FUNCTION(test)
{

	php_zephir_init_globals(ZEPHIR_VGLOBAL TSRMLS_CC);
	//test_init_interned_strings(TSRMLS_C);

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(test)
{

	if (ZEPHIR_GLOBAL(start_memory) != NULL) {
		zephir_clean_restore_stack(TSRMLS_C);
	}

	/*if (ZEPHIR_GLOBAL(function_cache) != NULL) {
		zend_hash_destroy(ZEPHIR_GLOBAL(function_cache));
		FREE_HASHTABLE(ZEPHIR_GLOBAL(function_cache));
		ZEPHIR_GLOBAL(function_cache) = NULL;
	}*/

	zend_hash_apply_with_arguments(ZEPHIR_GLOBAL(fcache) TSRMLS_CC, zephir_cleanup_fcache, 0);

	return SUCCESS;
}

static PHP_MINFO_FUNCTION(test)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_TEST_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_TEST_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_TEST_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_TEST_VERSION);
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_TEST_ZEPVERSION);
	php_info_print_table_end();

	php_info_print_table_start();
	php_info_print_table_header(2, "Directive", "Value");
	php_info_print_table_row(2, "setting1", "value1");
	php_info_print_table_row(2, "setting2", "value2");
	php_info_print_table_end();
	php_info_print_table_start();
	php_info_print_table_header(2, "Directive", "Value");
	php_info_print_table_row(2, "setting3", "value3");
	php_info_print_table_row(2, "setting4", "value4");
	php_info_print_table_end();

}

static PHP_GINIT_FUNCTION(test)
{
	zephir_memory_entry *start;
	int num_preallocated_frames = 24;
	size_t i;

	php_zephir_init_globals(test_globals TSRMLS_CC);

	/* pre-allocated memory frames */
	start = (zephir_memory_entry *) pecalloc(num_preallocated_frames, sizeof(zephir_memory_entry), 1);

	for (i = 0; i < num_preallocated_frames; ++i) {
		start[i].addresses = pecalloc(16, sizeof(zval*), 1);
		start[i].capacity = 16;
		start[i].hash_addresses = pecalloc(4, sizeof(zval*), 1);
		start[i].hash_capacity = 4;

#ifndef ZEPHIR_RELEASE
		start[i].permanent = 1;
#endif
	}

	start[0].next = &start[1];
	start[num_preallocated_frames - 1].prev = &start[num_preallocated_frames - 2];

	for (i = 1; i < num_preallocated_frames - 1; ++i) {
		start[i].next = &start[i + 1];
		start[i].prev = &start[i - 1];
	}

	test_globals->start_memory = start;
	test_globals->end_memory = start + num_preallocated_frames;

	/* Function call cache */
	test_globals->fcache = pemalloc(sizeof(HashTable), 1);
#ifndef ZEPHIR_RELEASE
	zend_hash_init(test_globals->fcache, 128, NULL, zephir_fcall_cache_dtor, 1);
#else
	zend_hash_init(test_globals->fcache, 128, NULL, NULL, 1);
#endif

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
	size_t i;
	int num_preallocated_frames = 24;

	assert(test_globals->start_memory != NULL);

	for (i = 0; i < num_preallocated_frames; ++i) {
		pefree(test_globals->start_memory[i].hash_addresses, 1);
		pefree(test_globals->start_memory[i].addresses, 1);
	}

	pefree(test_globals->start_memory, 1);
	test_globals->start_memory = NULL;

	zend_hash_destroy(test_globals->fcache);
	pefree(test_globals->fcache, 1);
	test_globals->fcache = NULL;
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
