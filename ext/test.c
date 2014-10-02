
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

zend_class_entry *test_testinterface_ce;
zend_class_entry *test_methodinterface_ce;
zend_class_entry *test_ooimpl_zbeginning_ce;
zend_class_entry *test_diinterface_ce;
zend_class_entry *test_extendedinterface_ce;
zend_class_entry *test_ooimpl_abeginning_ce;
zend_class_entry *test_constantsparent_ce;
zend_class_entry *test_oo_oodynamica_ce;
zend_class_entry *test_properties_publicproperties_ce;
zend_class_entry *test_scallparent_ce;
zend_class_entry *test_arithmetic_ce;
zend_class_entry *test_arrayobject_ce;
zend_class_entry *test_assign_ce;
zend_class_entry *test_bench_foo_ce;
zend_class_entry *test_bitwise_ce;
zend_class_entry *test_branchprediction_ce;
zend_class_entry *test_builtin_charmethods_ce;
zend_class_entry *test_builtin_intmethods_ce;
zend_class_entry *test_builtin_stringmethods_ce;
zend_class_entry *test_cast_ce;
zend_class_entry *test_cblock_ce;
zend_class_entry *test_closures_ce;
zend_class_entry *test_compare_ce;
zend_class_entry *test_concat_ce;
zend_class_entry *test_constants_ce;
zend_class_entry *test_constantsinterface_ce;
zend_class_entry *test_constantsinterfacea_ce;
zend_class_entry *test_constantsinterfaceb_ce;
zend_class_entry *test_declaretest_ce;
zend_class_entry *test_echoes_ce;
zend_class_entry *test_emptytest_ce;
zend_class_entry *test_evaltest_ce;
zend_class_entry *test_exception_ce;
zend_class_entry *test_exceptions_ce;
zend_class_entry *test_exists_ce;
zend_class_entry *test_exitdie_ce;
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
zend_class_entry *test_issues_ce;
zend_class_entry *test_json_ce;
zend_class_entry *test_logical_ce;
zend_class_entry *test_mcall_ce;
zend_class_entry *test_mcallchained_ce;
zend_class_entry *test_methodabstract_ce;
zend_class_entry *test_methodargs_ce;
zend_class_entry *test_nativearray_ce;
zend_class_entry *test_oo_abstractclass_ce;
zend_class_entry *test_oo_ce;
zend_class_entry *test_oo_constantsinterface_ce;
zend_class_entry *test_oo_deprecatedmethods_ce;
zend_class_entry *test_oo_dynamicprop_ce;
zend_class_entry *test_oo_extend_spl_arrayobject_ce;
zend_class_entry *test_oo_extend_spl_directoryiterator_ce;
zend_class_entry *test_oo_extend_spl_fileinfo_ce;
zend_class_entry *test_oo_extend_spl_fileobject_ce;
zend_class_entry *test_oo_extend_spl_filesystemiterator_ce;
zend_class_entry *test_oo_extend_spl_fixedarray_ce;
zend_class_entry *test_oo_extend_spl_globiterator_ce;
zend_class_entry *test_oo_extend_spl_heap_ce;
zend_class_entry *test_oo_extend_spl_maxheap_ce;
zend_class_entry *test_oo_extend_spl_minheap_ce;
zend_class_entry *test_oo_extend_spl_priorityqueue_ce;
zend_class_entry *test_oo_extend_spl_recursivedirectoryiterator_ce;
zend_class_entry *test_oo_extend_spl_tempfileobject_ce;
zend_class_entry *test_oo_extendpdoclass_ce;
zend_class_entry *test_oo_ooconstruct_ce;
zend_class_entry *test_oo_ooconstructparams_ce;
zend_class_entry *test_oo_oodynamicb_ce;
zend_class_entry *test_oo_oonativeimplements_ce;
zend_class_entry *test_oo_oonoconstruct_ce;
zend_class_entry *test_oo_ooparams_ce;
zend_class_entry *test_oo_param_ce;
zend_class_entry *test_oo_propertyaccess_ce;
zend_class_entry *test_operator_ce;
zend_class_entry *test_optimizers_arraymerge_ce;
zend_class_entry *test_pdostatement_ce;
zend_class_entry *test_pregmatch_ce;
zend_class_entry *test_properties_extendspublicproperties_ce;
zend_class_entry *test_properties_privateproperties_ce;
zend_class_entry *test_properties_propertyarray_ce;
zend_class_entry *test_properties_protectedproperties_ce;
zend_class_entry *test_properties_staticprotectedproperties_ce;
zend_class_entry *test_properties_staticpublicproperties_ce;
zend_class_entry *test_quantum_ce;
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
zend_class_entry *test_statements_ce;
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

#define ZEPHIR_NUM_PREALLOCATED_FRAMES 25

void zephir_initialize_memory(zend_zephir_globals_def *zephir_globals_ptr TSRMLS_DC)
{
	zephir_memory_entry *start;
	size_t i;

	start = (zephir_memory_entry *) pecalloc(ZEPHIR_NUM_PREALLOCATED_FRAMES, sizeof(zephir_memory_entry), 1);
/* pecalloc() will take care of these members for every frame
	start->pointer      = 0;
	start->hash_pointer = 0;
	start->prev = NULL;
	start->next = NULL;
*/
	for (i = 0; i < ZEPHIR_NUM_PREALLOCATED_FRAMES; ++i) {
		start[i].addresses       = pecalloc(24, sizeof(zval*), 1);
		start[i].capacity        = 24;
		start[i].hash_addresses  = pecalloc(8, sizeof(zval*), 1);
		start[i].hash_capacity   = 8;

#ifndef ZEPHIR_RELEASE
		start[i].permanent = 1;
#endif
	}

	start[0].next = &start[1];
	start[ZEPHIR_NUM_PREALLOCATED_FRAMES - 1].prev = &start[ZEPHIR_NUM_PREALLOCATED_FRAMES - 2];

	for (i = 1; i < ZEPHIR_NUM_PREALLOCATED_FRAMES - 1; ++i) {
		start[i].next = &start[i + 1];
		start[i].prev = &start[i - 1];
	}

	zephir_globals_ptr->start_memory = start;
	zephir_globals_ptr->end_memory   = start + ZEPHIR_NUM_PREALLOCATED_FRAMES;

	zephir_globals_ptr->fcache = pemalloc(sizeof(HashTable), 1);
	zend_hash_init(zephir_globals_ptr->fcache, 128, NULL, NULL, 1); // zephir_fcall_cache_dtor

	/* 'Allocator sizeof operand mismatch' warning can be safely ignored */
	ALLOC_INIT_ZVAL(zephir_globals_ptr->global_null);
	Z_SET_REFCOUNT_P(zephir_globals_ptr->global_null, 2);

	/* 'Allocator sizeof operand mismatch' warning can be safely ignored */
	ALLOC_INIT_ZVAL(zephir_globals_ptr->global_false);
	Z_SET_REFCOUNT_P(zephir_globals_ptr->global_false, 2);
	ZVAL_FALSE(zephir_globals_ptr->global_false);

	/* 'Allocator sizeof operand mismatch' warning can be safely ignored */
	ALLOC_INIT_ZVAL(zephir_globals_ptr->global_true);
	Z_SET_REFCOUNT_P(zephir_globals_ptr->global_true, 2);
	ZVAL_TRUE(zephir_globals_ptr->global_true);

	//zephir_globals_ptr->initialized = 1;
}

int zephir_cleanup_fcache(void *pDest TSRMLS_DC, int num_args, va_list args, zend_hash_key *hash_key)
{
	zephir_fcall_cache_entry **entry = (zephir_fcall_cache_entry**)pDest;
	zend_class_entry *scope;
	uint len = hash_key->nKeyLength;

	assert(hash_key->arKey != NULL);
	assert(hash_key->nKeyLength > 2 * sizeof(zend_class_entry**));

	memcpy(&scope, &hash_key->arKey[len - 2 * sizeof(zend_class_entry**)], sizeof(zend_class_entry*));

/*
#ifndef ZEPHIR_RELEASE
	{
		zend_class_entry *cls;
		memcpy(&cls, &hash_key->arKey[len - sizeof(zend_class_entry**)], sizeof(zend_class_entry*));

		fprintf(stderr, "func: %s, cls: %s, scope: %s [%u]\n", (*entry)->f->common.function_name, (cls ? cls->name : "N/A"), (scope ? scope->name : "N/A"), (uint)(*entry)->times);
	}
#endif
*/

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

void zephir_deinitialize_memory(TSRMLS_D)
{
	size_t i;
	zend_zephir_globals_def *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	//if (zephir_globals_ptr->initialized != 1) {
	//	zephir_globals_ptr->initialized = 0;
	//	return;
	//}

	if (zephir_globals_ptr->start_memory != NULL) {
		zephir_clean_restore_stack(TSRMLS_C);
	}

	//zephir_orm_destroy_cache(TSRMLS_C);

	zend_hash_apply_with_arguments(zephir_globals_ptr->fcache TSRMLS_CC, zephir_cleanup_fcache, 0);

#ifndef ZEPHIR_RELEASE
	assert(zephir_globals_ptr->start_memory != NULL);
#endif

	for (i = 0; i < ZEPHIR_NUM_PREALLOCATED_FRAMES; ++i) {
		pefree(zephir_globals_ptr->start_memory[i].hash_addresses, 1);
		pefree(zephir_globals_ptr->start_memory[i].addresses, 1);
	}

	pefree(zephir_globals_ptr->start_memory, 1);
	zephir_globals_ptr->start_memory = NULL;

	zend_hash_destroy(zephir_globals_ptr->fcache);
	pefree(zephir_globals_ptr->fcache, 1);
	zephir_globals_ptr->fcache = NULL;

	for (i = 0; i < 2; i++) {
		zval_ptr_dtor(&zephir_globals_ptr->global_null);
		zval_ptr_dtor(&zephir_globals_ptr->global_false);
		zval_ptr_dtor(&zephir_globals_ptr->global_true);
	}

	//zephir_globals_ptr->initialized = 0;
}

static PHP_MINIT_FUNCTION(test)
{
#if PHP_VERSION_ID < 50500
	char* old_lc_all = setlocale(LC_ALL, NULL);
	if (old_lc_all) {
		size_t len = strlen(old_lc_all);
		char *tmp  = calloc(len+1, 1);
		if (UNEXPECTED(!tmp)) {
			return FAILURE;
		}

		memcpy(tmp, old_lc_all, len);
		old_lc_all = tmp;
	}

	setlocale(LC_ALL, "C");
#endif

	ZEPHIR_INIT(Test_TestInterface);
	ZEPHIR_INIT(Test_MethodInterface);
	ZEPHIR_INIT(Test_OoImpl_ZBeginning);
	ZEPHIR_INIT(Test_DiInterface);
	ZEPHIR_INIT(Test_ExtendedInterface);
	ZEPHIR_INIT(Test_OoImpl_ABeginning);
	ZEPHIR_INIT(Test_ConstantsParent);
	ZEPHIR_INIT(Test_Oo_OoDynamicA);
	ZEPHIR_INIT(Test_Properties_PublicProperties);
	ZEPHIR_INIT(Test_ScallParent);
	ZEPHIR_INIT(Test_Arithmetic);
	ZEPHIR_INIT(Test_ArrayObject);
	ZEPHIR_INIT(Test_Assign);
	ZEPHIR_INIT(Test_Bench_Foo);
	ZEPHIR_INIT(Test_Bitwise);
	ZEPHIR_INIT(Test_BranchPrediction);
	ZEPHIR_INIT(Test_BuiltIn_CharMethods);
	ZEPHIR_INIT(Test_BuiltIn_IntMethods);
	ZEPHIR_INIT(Test_BuiltIn_StringMethods);
	ZEPHIR_INIT(Test_Cast);
	ZEPHIR_INIT(Test_Cblock);
	ZEPHIR_INIT(Test_Closures);
	ZEPHIR_INIT(Test_Compare);
	ZEPHIR_INIT(Test_Concat);
	ZEPHIR_INIT(Test_Constants);
	ZEPHIR_INIT(Test_ConstantsInterface);
	ZEPHIR_INIT(Test_ConstantsInterfaceA);
	ZEPHIR_INIT(Test_ConstantsInterfaceB);
	ZEPHIR_INIT(Test_DeclareTest);
	ZEPHIR_INIT(Test_Echoes);
	ZEPHIR_INIT(Test_EmptyTest);
	ZEPHIR_INIT(Test_EvalTest);
	ZEPHIR_INIT(Test_Exception);
	ZEPHIR_INIT(Test_Exceptions);
	ZEPHIR_INIT(Test_Exists);
	ZEPHIR_INIT(Test_ExitDie);
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
	ZEPHIR_INIT(Test_Issues);
	ZEPHIR_INIT(Test_Json);
	ZEPHIR_INIT(Test_Logical);
	ZEPHIR_INIT(Test_Mcall);
	ZEPHIR_INIT(Test_McallChained);
	ZEPHIR_INIT(Test_MethodAbstract);
	ZEPHIR_INIT(Test_MethodArgs);
	ZEPHIR_INIT(Test_NativeArray);
	ZEPHIR_INIT(Test_Oo);
	ZEPHIR_INIT(Test_Oo_AbstractClass);
	ZEPHIR_INIT(Test_Oo_ConstantsInterface);
	ZEPHIR_INIT(Test_Oo_DeprecatedMethods);
	ZEPHIR_INIT(Test_Oo_DynamicProp);
	ZEPHIR_INIT(Test_Oo_ExtendPdoClass);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_ArrayObject);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_DirectoryIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FileInfo);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FileObject);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FilesystemIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FixedArray);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_GlobIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_Heap);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_MaxHeap);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_MinHeap);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_PriorityQueue);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_RecursiveDirectoryIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_TempFileObject);
	ZEPHIR_INIT(Test_Oo_OoConstruct);
	ZEPHIR_INIT(Test_Oo_OoConstructParams);
	ZEPHIR_INIT(Test_Oo_OoDynamicB);
	ZEPHIR_INIT(Test_Oo_OoNativeImplements);
	ZEPHIR_INIT(Test_Oo_OoNoConstruct);
	ZEPHIR_INIT(Test_Oo_OoParams);
	ZEPHIR_INIT(Test_Oo_Param);
	ZEPHIR_INIT(Test_Oo_PropertyAccess);
	ZEPHIR_INIT(Test_Operator);
	ZEPHIR_INIT(Test_Optimizers_ArrayMerge);
	ZEPHIR_INIT(Test_PdoStatement);
	ZEPHIR_INIT(Test_Pregmatch);
	ZEPHIR_INIT(Test_Properties_ExtendsPublicProperties);
	ZEPHIR_INIT(Test_Properties_PrivateProperties);
	ZEPHIR_INIT(Test_Properties_PropertyArray);
	ZEPHIR_INIT(Test_Properties_ProtectedProperties);
	ZEPHIR_INIT(Test_Properties_StaticProtectedProperties);
	ZEPHIR_INIT(Test_Properties_StaticPublicProperties);
	ZEPHIR_INIT(Test_Quantum);
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
	ZEPHIR_INIT(Test_Statements);
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

	zephir_deinitialize_memory(TSRMLS_C);

	//assert(ZEPHIR_GLOBAL(orm).parser_cache == NULL);
	//assert(ZEPHIR_GLOBAL(orm).ast_cache == NULL);

	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_test_globals *zephir_globals TSRMLS_DC)
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

static PHP_RINIT_FUNCTION(test)
{

	zend_test_globals *zephir_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(zephir_globals_ptr TSRMLS_CC);
	//zephir_init_interned_strings(TSRMLS_C);

	zephir_initialize_memory(zephir_globals_ptr TSRMLS_CC);

	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(test)
{

	zephir_deinitialize_memory(TSRMLS_C);
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
	php_zephir_init_globals(test_globals TSRMLS_CC);
}

static PHP_GSHUTDOWN_FUNCTION(test)
{

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
