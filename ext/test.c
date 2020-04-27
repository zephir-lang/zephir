
/* This file was generated automatically by Zephir do not modify it! */

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

#include "kernel/globals.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"



zend_class_entry *test_testinterface_ce;
zend_class_entry *test_methodinterface_ce;
zend_class_entry *test_oo_scopes_scopetesterinterface_ce;
zend_class_entry *test_ooimpl_zbeginning_ce;
zend_class_entry *test_diinterface_ce;
zend_class_entry *test_extendedinterface_ce;
zend_class_entry *test_integration_psr_http_message_messageinterfaceex_ce;
zend_class_entry *test_ooimpl_abeginning_ce;
zend_class_entry *test_oo_extend_exception_ce;
zend_class_entry *test_oo_extend_db_exception_ce;
zend_class_entry *test_scallparent_ce;
zend_class_entry *test_constantsparent_ce;
zend_class_entry *test_globals_session_base_ce;
zend_class_entry *test_oo_abstractstatic_ce;
zend_class_entry *test_oo_extend_db_query_exception_ce;
zend_class_entry *test_oo_oodynamica_ce;
zend_class_entry *test_oo_scopes_hasprivatemethod_ce;
zend_class_entry *test_properties_publicproperties_ce;
zend_class_entry *test_0__closure_ce;
zend_class_entry *test_10__closure_ce;
zend_class_entry *test_11__closure_ce;
zend_class_entry *test_12__closure_ce;
zend_class_entry *test_1__closure_ce;
zend_class_entry *test_2__closure_ce;
zend_class_entry *test_3__closure_ce;
zend_class_entry *test_4__closure_ce;
zend_class_entry *test_5__closure_ce;
zend_class_entry *test_6__closure_ce;
zend_class_entry *test_7__closure_ce;
zend_class_entry *test_8__closure_ce;
zend_class_entry *test_9__closure_ce;
zend_class_entry *test_arithmetic_ce;
zend_class_entry *test_arrayaccessobj_ce;
zend_class_entry *test_arrayaccesstest_ce;
zend_class_entry *test_arrayiterator_ce;
zend_class_entry *test_arrayiteratortest_ce;
zend_class_entry *test_arrayobject_ce;
zend_class_entry *test_arraysearch_ce;
zend_class_entry *test_assign_ce;
zend_class_entry *test_bench_foo_ce;
zend_class_entry *test_bitwise_ce;
zend_class_entry *test_branchprediction_ce;
zend_class_entry *test_builtin_arraymethods_ce;
zend_class_entry *test_builtin_charmethods_ce;
zend_class_entry *test_builtin_intmethods_ce;
zend_class_entry *test_builtin_stringmethods_ce;
zend_class_entry *test_cast_ce;
zend_class_entry *test_cblock_ce;
zend_class_entry *test_chars_ce;
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
zend_class_entry *test_fasta_ce;
zend_class_entry *test_fcall_ce;
zend_class_entry *test_fetchtest_ce;
zend_class_entry *test_fibonnaci_ce;
zend_class_entry *test_flow_ce;
zend_class_entry *test_flow_switchflow_ce;
zend_class_entry *test_fortytwo_ce;
zend_class_entry *test_functional_ce;
zend_class_entry *test_functionexists_ce;
zend_class_entry *test_geometry_ce;
zend_class_entry *test_globals_ce;
zend_class_entry *test_globals_env_ce;
zend_class_entry *test_globals_post_ce;
zend_class_entry *test_globals_server_ce;
zend_class_entry *test_globals_serverrequestfactory_ce;
zend_class_entry *test_globals_session_child_ce;
zend_class_entry *test_instance_ce;
zend_class_entry *test_instanceoff_ce;
zend_class_entry *test_internalclasses_ce;
zend_class_entry *test_internalinterfaces_ce;
zend_class_entry *test_invoke_ce;
zend_class_entry *test_issettest_ce;
zend_class_entry *test_issue1404_ce;
zend_class_entry *test_issue1521_ce;
zend_class_entry *test_issues_ce;
zend_class_entry *test_json_ce;
zend_class_entry *test_logical_ce;
zend_class_entry *test_mcall_caller_ce;
zend_class_entry *test_mcall_ce;
zend_class_entry *test_mcallchained_ce;
zend_class_entry *test_mcalldynamic_ce;
zend_class_entry *test_mcallinternal_ce;
zend_class_entry *test_methodabstract_ce;
zend_class_entry *test_methodargs_ce;
zend_class_entry *test_nativearray_ce;
zend_class_entry *test_oo_abstractclass_ce;
zend_class_entry *test_oo_ce;
zend_class_entry *test_oo_concretestatic_ce;
zend_class_entry *test_oo_constantsinterface_ce;
zend_class_entry *test_oo_deprecatedmethods_ce;
zend_class_entry *test_oo_dynamicprop_ce;
zend_class_entry *test_oo_extend_db_query_placeholder_exception_ce;
zend_class_entry *test_oo_extend_spl_arrayobject_ce;
zend_class_entry *test_oo_extend_spl_directoryiterator_ce;
zend_class_entry *test_oo_extend_spl_doublylinkedlist_ce;
zend_class_entry *test_oo_extend_spl_fileinfo_ce;
zend_class_entry *test_oo_extend_spl_fileobject_ce;
zend_class_entry *test_oo_extend_spl_filesystemiterator_ce;
zend_class_entry *test_oo_extend_spl_fixedarray_ce;
zend_class_entry *test_oo_extend_spl_globiterator_ce;
zend_class_entry *test_oo_extend_spl_heap_ce;
zend_class_entry *test_oo_extend_spl_maxheap_ce;
zend_class_entry *test_oo_extend_spl_minheap_ce;
zend_class_entry *test_oo_extend_spl_priorityqueue_ce;
zend_class_entry *test_oo_extend_spl_queue_ce;
zend_class_entry *test_oo_extend_spl_recursivedirectoryiterator_ce;
zend_class_entry *test_oo_extend_spl_stack_ce;
zend_class_entry *test_oo_extend_spl_tempfileobject_ce;
zend_class_entry *test_oo_extendpdoclass_ce;
zend_class_entry *test_oo_ooconstruct_ce;
zend_class_entry *test_oo_ooconstructparams_ce;
zend_class_entry *test_oo_oodestruct_ce;
zend_class_entry *test_oo_oodynamicb_ce;
zend_class_entry *test_oo_oonativeimplements_ce;
zend_class_entry *test_oo_oonoconstruct_ce;
zend_class_entry *test_oo_ooparams_ce;
zend_class_entry *test_oo_param_ce;
zend_class_entry *test_oo_propertyaccess_ce;
zend_class_entry *test_oo_scopes_abstractclass_ce;
zend_class_entry *test_oo_scopes_abstractclassmagic_ce;
zend_class_entry *test_oo_scopes_privatescopetester_ce;
zend_class_entry *test_operator_ce;
zend_class_entry *test_optimizers_acos_ce;
zend_class_entry *test_optimizers_arraymerge_ce;
zend_class_entry *test_optimizers_asin_ce;
zend_class_entry *test_optimizers_cos_ce;
zend_class_entry *test_optimizers_createarray_ce;
zend_class_entry *test_optimizers_isscalar_ce;
zend_class_entry *test_optimizers_ldexp_ce;
zend_class_entry *test_optimizers_sin_ce;
zend_class_entry *test_optimizers_sqrt_ce;
zend_class_entry *test_optimizers_strreplace_ce;
zend_class_entry *test_optimizers_substr_ce;
zend_class_entry *test_optimizers_tan_ce;
zend_class_entry *test_pdostatement_ce;
zend_class_entry *test_pregmatch_ce;
zend_class_entry *test_properties_app_ce;
zend_class_entry *test_properties_extendspublicproperties_ce;
zend_class_entry *test_properties_privateproperties_ce;
zend_class_entry *test_properties_propertyarray_ce;
zend_class_entry *test_properties_propertyupdate_ce;
zend_class_entry *test_properties_protectedproperties_ce;
zend_class_entry *test_properties_staticprivateproperties_ce;
zend_class_entry *test_properties_staticpropertyarray_ce;
zend_class_entry *test_properties_staticprotectedproperties_ce;
zend_class_entry *test_properties_staticpublicproperties_ce;
zend_class_entry *test_quantum_ce;
zend_class_entry *test_range_ce;
zend_class_entry *test_references_ce;
zend_class_entry *test_regexdna_ce;
zend_class_entry *test_requires_ce;
zend_class_entry *test_requires_external3_ce;
zend_class_entry *test_resourcetest_ce;
zend_class_entry *test_returns_ce;
zend_class_entry *test_router_ce;
zend_class_entry *test_router_exception_ce;
zend_class_entry *test_router_route_ce;
zend_class_entry *test_scall_ce;
zend_class_entry *test_scalldynamic_ce;
zend_class_entry *test_scallexternal_ce;
zend_class_entry *test_scope_ce;
zend_class_entry *test_sort_ce;
zend_class_entry *test_spectralnorm_ce;
zend_class_entry *test_spropertyaccess_ce;
zend_class_entry *test_statements_ce;
zend_class_entry *test_strings_ce;
zend_class_entry *test_stubs_ce;
zend_class_entry *test_ternary_ce;
zend_class_entry *test_trytest_ce;
zend_class_entry *test_typehinting_testabstract_ce;
zend_class_entry *test_typeinstances_ce;
zend_class_entry *test_typeoff_ce;
zend_class_entry *test_unknownclass_ce;
zend_class_entry *test_unsettest_ce;
zend_class_entry *test_usetest_ce;
zend_class_entry *test_vars_ce;

#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif

ZEND_DECLARE_MODULE_GLOBALS(test)

PHP_INI_BEGIN()
	STD_PHP_INI_BOOLEAN("test.db.my_setting_1", "0", PHP_INI_ALL, OnUpdateBool, db.my_setting_1, zend_test_globals, test_globals)
	
	
	
	STD_PHP_INI_BOOLEAN("test.orm.cache_enable", "1", PHP_INI_ALL, OnUpdateBool, orm.cache_enable, zend_test_globals, test_globals)
	STD_PHP_INI_BOOLEAN("extension.test_ini_variable", "1", PHP_INI_ALL, OnUpdateBool, extension.test_ini_variable, zend_test_globals, test_globals)
	STD_PHP_INI_BOOLEAN("ini-entry.my_setting_1", "1", PHP_INI_ALL, OnUpdateBool, my_setting_1, zend_test_globals, test_globals)
	STD_PHP_INI_BOOLEAN("test.test_setting_1", "1", PHP_INI_ALL, OnUpdateBool, test_setting_1, zend_test_globals, test_globals)
PHP_INI_END()

static PHP_MINIT_FUNCTION(test)
{
	REGISTER_INI_ENTRIES();
	zephir_module_init();
	ZEPHIR_INIT(Test_TestInterface);
	ZEPHIR_INIT(Test_MethodInterface);
	ZEPHIR_INIT(Test_OoImpl_ZBeginning);
	ZEPHIR_INIT(Test_Oo_Scopes_ScopeTesterInterface);
	ZEPHIR_INIT(Test_DiInterface);
	ZEPHIR_INIT(Test_ExtendedInterface);
	ZEPHIR_INIT(Test_Integration_Psr_Http_Message_MessageInterfaceEx);
	ZEPHIR_INIT(Test_OoImpl_ABeginning);
	ZEPHIR_INIT(Test_Oo_Extend_Exception);
	ZEPHIR_INIT(Test_Oo_Extend_Db_Exception);
	ZEPHIR_INIT(Test_ScallParent);
	ZEPHIR_INIT(Test_ConstantsParent);
	ZEPHIR_INIT(Test_Globals_Session_Base);
	ZEPHIR_INIT(Test_Oo_AbstractStatic);
	ZEPHIR_INIT(Test_Oo_Extend_Db_Query_Exception);
	ZEPHIR_INIT(Test_Oo_OoDynamicA);
	ZEPHIR_INIT(Test_Oo_Scopes_HasPrivateMethod);
	ZEPHIR_INIT(Test_Properties_PublicProperties);
	ZEPHIR_INIT(Test_Arithmetic);
	ZEPHIR_INIT(Test_ArrayAccessObj);
	ZEPHIR_INIT(Test_ArrayAccessTest);
	ZEPHIR_INIT(Test_ArrayIterator);
	ZEPHIR_INIT(Test_ArrayIteratorTest);
	ZEPHIR_INIT(Test_ArrayObject);
	ZEPHIR_INIT(Test_ArraySearch);
	ZEPHIR_INIT(Test_Assign);
	ZEPHIR_INIT(Test_Bench_Foo);
	ZEPHIR_INIT(Test_Bitwise);
	ZEPHIR_INIT(Test_BranchPrediction);
	ZEPHIR_INIT(Test_BuiltIn_ArrayMethods);
	ZEPHIR_INIT(Test_BuiltIn_CharMethods);
	ZEPHIR_INIT(Test_BuiltIn_IntMethods);
	ZEPHIR_INIT(Test_BuiltIn_StringMethods);
	ZEPHIR_INIT(Test_Cast);
	ZEPHIR_INIT(Test_Cblock);
	ZEPHIR_INIT(Test_Chars);
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
	ZEPHIR_INIT(Test_Fasta);
	ZEPHIR_INIT(Test_Fcall);
	ZEPHIR_INIT(Test_FetchTest);
	ZEPHIR_INIT(Test_Fibonnaci);
	ZEPHIR_INIT(Test_Flow);
	ZEPHIR_INIT(Test_Flow_SwitchFlow);
	ZEPHIR_INIT(Test_FortyTwo);
	ZEPHIR_INIT(Test_FunctionExists);
	ZEPHIR_INIT(Test_Functional);
	ZEPHIR_INIT(Test_Geometry);
	ZEPHIR_INIT(Test_Globals);
	ZEPHIR_INIT(Test_Globals_Env);
	ZEPHIR_INIT(Test_Globals_Post);
	ZEPHIR_INIT(Test_Globals_Server);
	ZEPHIR_INIT(Test_Globals_ServerRequestFactory);
	ZEPHIR_INIT(Test_Globals_Session_Child);
	ZEPHIR_INIT(Test_Instance);
	ZEPHIR_INIT(Test_Instanceoff);
	ZEPHIR_INIT(Test_InternalClasses);
	ZEPHIR_INIT(Test_InternalInterfaces);
	ZEPHIR_INIT(Test_Invoke);
	ZEPHIR_INIT(Test_IssetTest);
	ZEPHIR_INIT(Test_Issue1404);
	ZEPHIR_INIT(Test_Issue1521);
	ZEPHIR_INIT(Test_Issues);
	ZEPHIR_INIT(Test_Json);
	ZEPHIR_INIT(Test_Logical);
	ZEPHIR_INIT(Test_Mcall);
	ZEPHIR_INIT(Test_McallChained);
	ZEPHIR_INIT(Test_McallDynamic);
	ZEPHIR_INIT(Test_McallInternal);
	ZEPHIR_INIT(Test_Mcall_Caller);
	ZEPHIR_INIT(Test_MethodAbstract);
	ZEPHIR_INIT(Test_MethodArgs);
	ZEPHIR_INIT(Test_NativeArray);
	ZEPHIR_INIT(Test_Oo);
	ZEPHIR_INIT(Test_Oo_AbstractClass);
	ZEPHIR_INIT(Test_Oo_ConcreteStatic);
	ZEPHIR_INIT(Test_Oo_ConstantsInterface);
	ZEPHIR_INIT(Test_Oo_DeprecatedMethods);
	ZEPHIR_INIT(Test_Oo_DynamicProp);
	ZEPHIR_INIT(Test_Oo_ExtendPdoClass);
	ZEPHIR_INIT(Test_Oo_Extend_Db_Query_Placeholder_Exception);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_ArrayObject);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_DirectoryIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_DoublyLinkedList);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FileInfo);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FileObject);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FilesystemIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_FixedArray);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_GlobIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_Heap);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_MaxHeap);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_MinHeap);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_PriorityQueue);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_Queue);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_RecursiveDirectoryIterator);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_Stack);
	ZEPHIR_INIT(Test_Oo_Extend_Spl_TempFileObject);
	ZEPHIR_INIT(Test_Oo_OoConstruct);
	ZEPHIR_INIT(Test_Oo_OoConstructParams);
	ZEPHIR_INIT(Test_Oo_OoDestruct);
	ZEPHIR_INIT(Test_Oo_OoDynamicB);
	ZEPHIR_INIT(Test_Oo_OoNativeImplements);
	ZEPHIR_INIT(Test_Oo_OoNoConstruct);
	ZEPHIR_INIT(Test_Oo_OoParams);
	ZEPHIR_INIT(Test_Oo_Param);
	ZEPHIR_INIT(Test_Oo_PropertyAccess);
	ZEPHIR_INIT(Test_Oo_Scopes_AbstractClass);
	ZEPHIR_INIT(Test_Oo_Scopes_AbstractClassMagic);
	ZEPHIR_INIT(Test_Oo_Scopes_PrivateScopeTester);
	ZEPHIR_INIT(Test_Operator);
	ZEPHIR_INIT(Test_Optimizers_ACos);
	ZEPHIR_INIT(Test_Optimizers_ASin);
	ZEPHIR_INIT(Test_Optimizers_ArrayMerge);
	ZEPHIR_INIT(Test_Optimizers_Cos);
	ZEPHIR_INIT(Test_Optimizers_CreateArray);
	ZEPHIR_INIT(Test_Optimizers_IsScalar);
	ZEPHIR_INIT(Test_Optimizers_Ldexp);
	ZEPHIR_INIT(Test_Optimizers_Sin);
	ZEPHIR_INIT(Test_Optimizers_Sqrt);
	ZEPHIR_INIT(Test_Optimizers_StrReplace);
	ZEPHIR_INIT(Test_Optimizers_Substr);
	ZEPHIR_INIT(Test_Optimizers_Tan);
	ZEPHIR_INIT(Test_PdoStatement);
	ZEPHIR_INIT(Test_Pregmatch);
	ZEPHIR_INIT(Test_Properties_App);
	ZEPHIR_INIT(Test_Properties_ExtendsPublicProperties);
	ZEPHIR_INIT(Test_Properties_PrivateProperties);
	ZEPHIR_INIT(Test_Properties_PropertyArray);
	ZEPHIR_INIT(Test_Properties_PropertyUpdate);
	ZEPHIR_INIT(Test_Properties_ProtectedProperties);
	ZEPHIR_INIT(Test_Properties_StaticPrivateProperties);
	ZEPHIR_INIT(Test_Properties_StaticPropertyArray);
	ZEPHIR_INIT(Test_Properties_StaticProtectedProperties);
	ZEPHIR_INIT(Test_Properties_StaticPublicProperties);
	ZEPHIR_INIT(Test_Quantum);
	ZEPHIR_INIT(Test_Range);
	ZEPHIR_INIT(Test_References);
	ZEPHIR_INIT(Test_RegexDNA);
	ZEPHIR_INIT(Test_Requires);
	ZEPHIR_INIT(Test_Requires_External3);
	ZEPHIR_INIT(Test_ResourceTest);
	ZEPHIR_INIT(Test_Returns);
	ZEPHIR_INIT(Test_Router);
	ZEPHIR_INIT(Test_Router_Exception);
	ZEPHIR_INIT(Test_Router_Route);
	ZEPHIR_INIT(Test_SPropertyAccess);
	ZEPHIR_INIT(Test_Scall);
	ZEPHIR_INIT(Test_ScallDynamic);
	ZEPHIR_INIT(Test_ScallExternal);
	ZEPHIR_INIT(Test_Scope);
	ZEPHIR_INIT(Test_Sort);
	ZEPHIR_INIT(Test_SpectralNorm);
	ZEPHIR_INIT(Test_Statements);
	ZEPHIR_INIT(Test_Strings);
	ZEPHIR_INIT(Test_Stubs);
	ZEPHIR_INIT(Test_Ternary);
	ZEPHIR_INIT(Test_TryTest);
	ZEPHIR_INIT(Test_TypeHinting_TestAbstract);
	ZEPHIR_INIT(Test_TypeInstances);
	ZEPHIR_INIT(Test_Typeoff);
	ZEPHIR_INIT(Test_UnknownClass);
	ZEPHIR_INIT(Test_Unsettest);
	ZEPHIR_INIT(Test_UseTest);
	ZEPHIR_INIT(Test_Vars);
	ZEPHIR_INIT(test_0__closure);
	ZEPHIR_INIT(test_10__closure);
	ZEPHIR_INIT(test_11__closure);
	ZEPHIR_INIT(test_12__closure);
	ZEPHIR_INIT(test_1__closure);
	ZEPHIR_INIT(test_2__closure);
	ZEPHIR_INIT(test_3__closure);
	ZEPHIR_INIT(test_4__closure);
	ZEPHIR_INIT(test_5__closure);
	ZEPHIR_INIT(test_6__closure);
	ZEPHIR_INIT(test_7__closure);
	ZEPHIR_INIT(test_8__closure);
	ZEPHIR_INIT(test_9__closure);
	
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(test)
{
	
	zephir_deinitialize_memory(TSRMLS_C);
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_test_globals *test_globals TSRMLS_DC)
{
	test_globals->initialized = 0;

	/* Cache Enabled */
	test_globals->cache_enabled = 1;

	/* Recursive Lock */
	test_globals->recursive_lock = 0;

	/* Static cache */
	memset(test_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);

	
	test_globals->db.my_setting_2 = 100;
	test_globals->db.my_setting_3 = 7.5;
	test_globals->orm.cache_level = 3;


	test_globals->my_setting_1 = 1;
	test_globals->test_setting_1 = 1;
	test_globals->my_setting_2 = 10;
	test_globals->my_setting_3 = 15.2;
	test_globals->my_setting_4 = 'A';

	
}

/**
 * Initialize globals only on each thread started
 */
static void php_zephir_init_module_globals(zend_test_globals *test_globals TSRMLS_DC)
{
	
}

static PHP_RINIT_FUNCTION(test)
{
	zend_test_globals *test_globals_ptr;
	test_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(test_globals_ptr);
	zephir_initialize_memory(test_globals_ptr);

		zephir_init_static_properties_Test_Properties_StaticPropertyArray(TSRMLS_C);
		zephir_init_static_properties_Test_SPropertyAccess(TSRMLS_C);
	
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
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_TEST_ZEPVERSION);
	php_info_print_table_end();
		php_info_print_table_start();
	php_info_print_table_header(2, "Test Extension support", "Value");
	php_info_print_table_row(2, "Lifecycle hooks", "PHP provides several lifecycle events, which extensions can use to perform common initialization or shutdown tasks.");
	php_info_print_table_row(2, "Static Analysis", "Test extensions' compiler provides static analysis of the compiled code.");
	php_info_print_table_end();
	php_info_print_table_start();
	php_info_print_table_header(2, "Test variable", "Value");
	php_info_print_table_row(2, "Extension", "Installed");
	php_info_print_table_row(2, "NFR", "Contributions are welcome!");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}

static PHP_GINIT_FUNCTION(test)
{
#ifdef ZTS
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	php_zephir_init_globals(test_globals);
	php_zephir_init_module_globals(test_globals);
}

static PHP_GSHUTDOWN_FUNCTION(test)
{
	
}

PHP_FUNCTION(g_test_zephir_global_method_test);
ZEND_BEGIN_ARG_INFO_EX(arginfo_g_test_zephir_global_method_test, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

PHP_FUNCTION(g_test_zephir_global_method_with_type_casting);
ZEND_BEGIN_ARG_INFO_EX(arginfo_g_test_zephir_global_method_with_type_casting, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, variable, stdClass, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Test_zephir_namespaced_method_test);
ZEND_BEGIN_ARG_INFO_EX(arginfo_f_test_zephir_namespaced_method_test, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Test_test_call_relative_object_hint);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_test_test_call_relative_object_hint, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_test_test_call_relative_object_hint, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_OBJ_INFO(0, a, Test\\Oo\\PropertyAccess, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Test_zephir_namespaced_method_with_type_casting);
ZEND_BEGIN_ARG_INFO_EX(arginfo_f_test_zephir_namespaced_method_with_type_casting, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, variable, stdClass, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Test_test_call_object_hint);
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_test_test_call_object_hint, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_test_test_call_object_hint, 0, 1, _IS_BOOL, NULL, 0)
#endif
	ZEND_ARG_OBJ_INFO(0, a, Test\\Oo\\PropertyAccess, 0)
ZEND_END_ARG_INFO()


zend_function_entry php_test_functions[] = {
	ZEND_NAMED_FE(zephir_global_method_test, ZEND_FN(g_test_zephir_global_method_test), arginfo_g_test_zephir_global_method_test)
ZEND_NAMED_FE(zephir_global_method_with_type_casting, ZEND_FN(g_test_zephir_global_method_with_type_casting), arginfo_g_test_zephir_global_method_with_type_casting)
ZEND_NS_NAMED_FE("Test", zephir_namespaced_method_test, ZEND_FN(f_Test_zephir_namespaced_method_test), arginfo_f_test_zephir_namespaced_method_test)
ZEND_NS_NAMED_FE("Test", test_call_relative_object_hint, ZEND_FN(f_Test_test_call_relative_object_hint), arginfo_f_test_test_call_relative_object_hint)
ZEND_NS_NAMED_FE("Test", zephir_namespaced_method_with_type_casting, ZEND_FN(f_Test_zephir_namespaced_method_with_type_casting), arginfo_f_test_zephir_namespaced_method_with_type_casting)
ZEND_NS_NAMED_FE("Test", test_call_object_hint, ZEND_FN(f_Test_test_call_object_hint), arginfo_f_test_test_call_object_hint)
ZEND_FE_END

};

static const zend_module_dep php_test_deps[] = {
	ZEND_MOD_REQUIRED("pdo")
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("standard")
	ZEND_MOD_REQUIRED("hash")
	ZEND_MOD_REQUIRED("json")
	ZEND_MOD_END
};

zend_module_entry test_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	php_test_deps,
	PHP_TEST_EXTNAME,
	php_test_functions,
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
#ifdef ZEPHIR_POST_REQUEST
	PHP_PRSHUTDOWN(test),
#else
	NULL,
#endif
	STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_TEST
ZEND_GET_MODULE(test)
#endif
