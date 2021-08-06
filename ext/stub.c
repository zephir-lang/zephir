
/* This file was generated automatically by Zephir do not modify it! */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <php.h>

#include "php_ext.h"
#include "stub.h"

#include <ext/standard/info.h>

#include <Zend/zend_operators.h>
#include <Zend/zend_exceptions.h>
#include <Zend/zend_interfaces.h>

#include "kernel/globals.h"
#include "kernel/main.h"
#include "kernel/fcall.h"
#include "kernel/memory.h"



zend_class_entry *stub_testinterface_ce;
zend_class_entry *stub_issue2165_issueextendinterface_ce;
zend_class_entry *stub_interfaces_interfaceint_ce;
zend_class_entry *stub_interfaces_interfaceintsignature_ce;
zend_class_entry *stub_issue2165_issueinterface_ce;
zend_class_entry *stub_methodinterface_ce;
zend_class_entry *stub_oo_scopes_scopetesterinterface_ce;
zend_class_entry *stub_ooimpl_zbeginning_ce;
zend_class_entry *stub_diinterface_ce;
zend_class_entry *stub_extendedinterface_ce;
zend_class_entry *stub_integration_psr_http_message_messageinterfaceex_ce;
zend_class_entry *stub_interfaces_interfacestaticmethod_ce;
zend_class_entry *stub_ooimpl_abeginning_ce;
zend_class_entry *stub_invokes_abstractprotected_ce;
zend_class_entry *stub_oo_extend_exception_ce;
zend_class_entry *stub_oo_extend_db_exception_ce;
zend_class_entry *stub_scallparent_ce;
zend_class_entry *stub_constantsparent_ce;
zend_class_entry *stub_globals_session_base_ce;
zend_class_entry *stub_invokes_abstractinvoker_ce;
zend_class_entry *stub_invokes_abstractinvokercomplex_ce;
zend_class_entry *stub_oo_abstractstatic_ce;
zend_class_entry *stub_oo_extend_db_query_exception_ce;
zend_class_entry *stub_oo_oodynamica_ce;
zend_class_entry *stub_oo_scopes_hasprivatemethod_ce;
zend_class_entry *stub_properties_publicproperties_ce;
zend_class_entry *stub_0__closure_ce;
zend_class_entry *stub_10__closure_ce;
zend_class_entry *stub_11__closure_ce;
zend_class_entry *stub_12__closure_ce;
zend_class_entry *stub_13__closure_ce;
zend_class_entry *stub_1__closure_ce;
zend_class_entry *stub_2__closure_ce;
zend_class_entry *stub_3__closure_ce;
zend_class_entry *stub_4__closure_ce;
zend_class_entry *stub_5__closure_ce;
zend_class_entry *stub_6__closure_ce;
zend_class_entry *stub_7__closure_ce;
zend_class_entry *stub_8__closure_ce;
zend_class_entry *stub_9__closure_ce;
zend_class_entry *stub_arithmetic_ce;
zend_class_entry *stub_arrayaccessobj_ce;
zend_class_entry *stub_arrayaccesstest_ce;
zend_class_entry *stub_arrayiterator_ce;
zend_class_entry *stub_arrayiteratortest_ce;
zend_class_entry *stub_arraymanipulation_ce;
zend_class_entry *stub_arrayobject_ce;
zend_class_entry *stub_arraysearch_ce;
zend_class_entry *stub_assign_ce;
zend_class_entry *stub_bench_foo_ce;
zend_class_entry *stub_bitwise_ce;
zend_class_entry *stub_branchprediction_ce;
zend_class_entry *stub_builtin_arraymethods_ce;
zend_class_entry *stub_builtin_charmethods_ce;
zend_class_entry *stub_builtin_intmethods_ce;
zend_class_entry *stub_builtin_stringmethods_ce;
zend_class_entry *stub_cast_ce;
zend_class_entry *stub_cblock_ce;
zend_class_entry *stub_chars_ce;
zend_class_entry *stub_closures_ce;
zend_class_entry *stub_compare_ce;
zend_class_entry *stub_concat_ce;
zend_class_entry *stub_constants_ce;
zend_class_entry *stub_constantsinterface_ce;
zend_class_entry *stub_constantsinterfacea_ce;
zend_class_entry *stub_constantsinterfaceb_ce;
zend_class_entry *stub_constructors_issue1803_ce;
zend_class_entry *stub_declaretest_ce;
zend_class_entry *stub_echoes_ce;
zend_class_entry *stub_emptytest_ce;
zend_class_entry *stub_evaltest_ce;
zend_class_entry *stub_exception_ce;
zend_class_entry *stub_exceptions_ce;
zend_class_entry *stub_exists_ce;
zend_class_entry *stub_exitdie_ce;
zend_class_entry *stub_factorial_ce;
zend_class_entry *stub_fannkuch_ce;
zend_class_entry *stub_fasta_ce;
zend_class_entry *stub_fcall_ce;
zend_class_entry *stub_fetchtest_ce;
zend_class_entry *stub_fibonnaci_ce;
zend_class_entry *stub_flow_ce;
zend_class_entry *stub_flow_switchflow_ce;
zend_class_entry *stub_fortytwo_ce;
zend_class_entry *stub_functional_ce;
zend_class_entry *stub_functionexists_ce;
zend_class_entry *stub_functions_ce;
zend_class_entry *stub_geometry_ce;
zend_class_entry *stub_globals_ce;
zend_class_entry *stub_globals_env_ce;
zend_class_entry *stub_globals_post_ce;
zend_class_entry *stub_globals_server_ce;
zend_class_entry *stub_globals_serverrequestfactory_ce;
zend_class_entry *stub_globals_session_child_ce;
zend_class_entry *stub_instance_ce;
zend_class_entry *stub_instanceoff_ce;
zend_class_entry *stub_integration_psr_extendexternal_ce;
zend_class_entry *stub_interfaces_implementint_ce;
zend_class_entry *stub_interfaces_implementinterface_ce;
zend_class_entry *stub_internalclasses_ce;
zend_class_entry *stub_internalinterfaces_ce;
zend_class_entry *stub_invoke_ce;
zend_class_entry *stub_invokes_invokeprotected_ce;
zend_class_entry *stub_invokes_invokeprotectedcomplex_ce;
zend_class_entry *stub_issettest_ce;
zend_class_entry *stub_issue1134_ce;
zend_class_entry *stub_issue1404_ce;
zend_class_entry *stub_issue1521_ce;
zend_class_entry *stub_issue2165_issue_ce;
zend_class_entry *stub_issue663_ce;
zend_class_entry *stub_issue887_ce;
zend_class_entry *stub_issue893_ce;
zend_class_entry *stub_issue914_ce;
zend_class_entry *stub_issues_ce;
zend_class_entry *stub_json_ce;
zend_class_entry *stub_logical_ce;
zend_class_entry *stub_mcall_caller_ce;
zend_class_entry *stub_mcall_ce;
zend_class_entry *stub_mcallchained_ce;
zend_class_entry *stub_mcalldynamic_ce;
zend_class_entry *stub_mcallinternal_ce;
zend_class_entry *stub_methodabstract_ce;
zend_class_entry *stub_methodargs_ce;
zend_class_entry *stub_namespaces_a_b_sub_ce;
zend_class_entry *stub_namespaces_classentry_ce;
zend_class_entry *stub_nativearray_ce;
zend_class_entry *stub_oo_abstractclass_ce;
zend_class_entry *stub_oo_ce;
zend_class_entry *stub_oo_concretestatic_ce;
zend_class_entry *stub_oo_constantsinterface_ce;
zend_class_entry *stub_oo_deprecatedmethods_ce;
zend_class_entry *stub_oo_dynamicprop_ce;
zend_class_entry *stub_oo_extend_db_query_placeholder_exception_ce;
zend_class_entry *stub_oo_extend_spl_arrayobject_ce;
zend_class_entry *stub_oo_extend_spl_directoryiterator_ce;
zend_class_entry *stub_oo_extend_spl_doublylinkedlist_ce;
zend_class_entry *stub_oo_extend_spl_fileinfo_ce;
zend_class_entry *stub_oo_extend_spl_fileobject_ce;
zend_class_entry *stub_oo_extend_spl_filesystemiterator_ce;
zend_class_entry *stub_oo_extend_spl_fixedarray_ce;
zend_class_entry *stub_oo_extend_spl_globiterator_ce;
zend_class_entry *stub_oo_extend_spl_heap_ce;
zend_class_entry *stub_oo_extend_spl_maxheap_ce;
zend_class_entry *stub_oo_extend_spl_minheap_ce;
zend_class_entry *stub_oo_extend_spl_priorityqueue_ce;
zend_class_entry *stub_oo_extend_spl_queue_ce;
zend_class_entry *stub_oo_extend_spl_recursivedirectoryiterator_ce;
zend_class_entry *stub_oo_extend_spl_stack_ce;
zend_class_entry *stub_oo_extend_spl_tempfileobject_ce;
zend_class_entry *stub_oo_extendpdoclass_ce;
zend_class_entry *stub_oo_ooconstruct_ce;
zend_class_entry *stub_oo_ooconstructparams_ce;
zend_class_entry *stub_oo_oodestruct_ce;
zend_class_entry *stub_oo_oodynamicb_ce;
zend_class_entry *stub_oo_oonativeimplements_ce;
zend_class_entry *stub_oo_oonoconstruct_ce;
zend_class_entry *stub_oo_ooparams_ce;
zend_class_entry *stub_oo_param_ce;
zend_class_entry *stub_oo_propertyaccess_ce;
zend_class_entry *stub_oo_scopes_abstractclass_ce;
zend_class_entry *stub_oo_scopes_abstractclassmagic_ce;
zend_class_entry *stub_oo_scopes_privatescopetester_ce;
zend_class_entry *stub_operator_ce;
zend_class_entry *stub_optimizers_acos_ce;
zend_class_entry *stub_optimizers_arraymerge_ce;
zend_class_entry *stub_optimizers_asin_ce;
zend_class_entry *stub_optimizers_cos_ce;
zend_class_entry *stub_optimizers_createarray_ce;
zend_class_entry *stub_optimizers_isscalar_ce;
zend_class_entry *stub_optimizers_ldexp_ce;
zend_class_entry *stub_optimizers_sin_ce;
zend_class_entry *stub_optimizers_sqrt_ce;
zend_class_entry *stub_optimizers_strreplace_ce;
zend_class_entry *stub_optimizers_substr_ce;
zend_class_entry *stub_optimizers_tan_ce;
zend_class_entry *stub_pdostatement_ce;
zend_class_entry *stub_pregmatch_ce;
zend_class_entry *stub_properties_app_ce;
zend_class_entry *stub_properties_extendspublicproperties_ce;
zend_class_entry *stub_properties_getobjectvars_ce;
zend_class_entry *stub_properties_privateproperties_ce;
zend_class_entry *stub_properties_propertyarray_ce;
zend_class_entry *stub_properties_propertyupdate_ce;
zend_class_entry *stub_properties_protectedproperties_ce;
zend_class_entry *stub_properties_staticprivateproperties_ce;
zend_class_entry *stub_properties_staticpropertyarray_ce;
zend_class_entry *stub_properties_staticprotectedproperties_ce;
zend_class_entry *stub_properties_staticpublicproperties_ce;
zend_class_entry *stub_quantum_ce;
zend_class_entry *stub_range_ce;
zend_class_entry *stub_references_ce;
zend_class_entry *stub_reflection_ce;
zend_class_entry *stub_regexdna_ce;
zend_class_entry *stub_requires_ce;
zend_class_entry *stub_requires_external3_ce;
zend_class_entry *stub_resourcetest_ce;
zend_class_entry *stub_returns_ce;
zend_class_entry *stub_router_ce;
zend_class_entry *stub_router_exception_ce;
zend_class_entry *stub_router_route_ce;
zend_class_entry *stub_scall_ce;
zend_class_entry *stub_scalldynamic_ce;
zend_class_entry *stub_scallexternal_ce;
zend_class_entry *stub_scalllateconstruct_ce;
zend_class_entry *stub_scope_ce;
zend_class_entry *stub_sort_ce;
zend_class_entry *stub_spectralnorm_ce;
zend_class_entry *stub_spl_ce;
zend_class_entry *stub_spropertyaccess_ce;
zend_class_entry *stub_statements_ce;
zend_class_entry *stub_strings_ce;
zend_class_entry *stub_stubs_ce;
zend_class_entry *stub_ternary_ce;
zend_class_entry *stub_trytest_ce;
zend_class_entry *stub_typehinting_testabstract_ce;
zend_class_entry *stub_typeinstances_ce;
zend_class_entry *stub_typeoff_ce;
zend_class_entry *stub_unknownclass_ce;
zend_class_entry *stub_unsettest_ce;
zend_class_entry *stub_usetest_ce;
zend_class_entry *stub_vars_ce;

ZEND_DECLARE_MODULE_GLOBALS(stub)

PHP_INI_BEGIN()
	STD_PHP_INI_BOOLEAN("stub.db.my_setting_1", "0", PHP_INI_ALL, OnUpdateBool, db.my_setting_1, zend_stub_globals, stub_globals)
	
	
	
	STD_PHP_INI_BOOLEAN("stub.orm.cache_enable", "1", PHP_INI_ALL, OnUpdateBool, orm.cache_enable, zend_stub_globals, stub_globals)
	STD_PHP_INI_BOOLEAN("extension.test_ini_variable", "1", PHP_INI_ALL, OnUpdateBool, extension.test_ini_variable, zend_stub_globals, stub_globals)
	STD_PHP_INI_BOOLEAN("ini-entry.my_setting_1", "1", PHP_INI_ALL, OnUpdateBool, my_setting_1, zend_stub_globals, stub_globals)
	STD_PHP_INI_BOOLEAN("stub.test_setting_1", "1", PHP_INI_ALL, OnUpdateBool, test_setting_1, zend_stub_globals, stub_globals)
PHP_INI_END()

static PHP_MINIT_FUNCTION(stub)
{
	REGISTER_INI_ENTRIES();
	zephir_module_init();
	ZEPHIR_INIT(Stub_TestInterface);
	ZEPHIR_INIT(Stub_Issue2165_IssueExtendInterface);
	ZEPHIR_INIT(Stub_Interfaces_InterfaceInt);
	ZEPHIR_INIT(Stub_Interfaces_InterfaceIntSignature);
	ZEPHIR_INIT(Stub_Issue2165_IssueInterface);
	ZEPHIR_INIT(Stub_MethodInterface);
	ZEPHIR_INIT(Stub_OoImpl_ZBeginning);
	ZEPHIR_INIT(Stub_Oo_Scopes_ScopeTesterInterface);
	ZEPHIR_INIT(Stub_DiInterface);
	ZEPHIR_INIT(Stub_ExtendedInterface);
	ZEPHIR_INIT(Stub_Integration_Psr_Http_Message_MessageInterfaceEx);
	ZEPHIR_INIT(Stub_Interfaces_InterfaceStaticMethod);
	ZEPHIR_INIT(Stub_OoImpl_ABeginning);
	ZEPHIR_INIT(Stub_Invokes_AbstractProtected);
	ZEPHIR_INIT(Stub_Oo_Extend_Exception);
	ZEPHIR_INIT(Stub_Oo_Extend_Db_Exception);
	ZEPHIR_INIT(Stub_ScallParent);
	ZEPHIR_INIT(Stub_ConstantsParent);
	ZEPHIR_INIT(Stub_Globals_Session_Base);
	ZEPHIR_INIT(Stub_Invokes_AbstractInvoker);
	ZEPHIR_INIT(Stub_Invokes_AbstractInvokerComplex);
	ZEPHIR_INIT(Stub_Oo_AbstractStatic);
	ZEPHIR_INIT(Stub_Oo_Extend_Db_Query_Exception);
	ZEPHIR_INIT(Stub_Oo_OoDynamicA);
	ZEPHIR_INIT(Stub_Oo_Scopes_HasPrivateMethod);
	ZEPHIR_INIT(Stub_Properties_PublicProperties);
	ZEPHIR_INIT(Stub_Arithmetic);
	ZEPHIR_INIT(Stub_ArrayAccessObj);
	ZEPHIR_INIT(Stub_ArrayAccessTest);
	ZEPHIR_INIT(Stub_ArrayIterator);
	ZEPHIR_INIT(Stub_ArrayIteratorTest);
	ZEPHIR_INIT(Stub_ArrayManipulation);
	ZEPHIR_INIT(Stub_ArrayObject);
	ZEPHIR_INIT(Stub_ArraySearch);
	ZEPHIR_INIT(Stub_Assign);
	ZEPHIR_INIT(Stub_Bench_Foo);
	ZEPHIR_INIT(Stub_Bitwise);
	ZEPHIR_INIT(Stub_BranchPrediction);
	ZEPHIR_INIT(Stub_BuiltIn_ArrayMethods);
	ZEPHIR_INIT(Stub_BuiltIn_CharMethods);
	ZEPHIR_INIT(Stub_BuiltIn_IntMethods);
	ZEPHIR_INIT(Stub_BuiltIn_StringMethods);
	ZEPHIR_INIT(Stub_Cast);
	ZEPHIR_INIT(Stub_Cblock);
	ZEPHIR_INIT(Stub_Chars);
	ZEPHIR_INIT(Stub_Closures);
	ZEPHIR_INIT(Stub_Compare);
	ZEPHIR_INIT(Stub_Concat);
	ZEPHIR_INIT(Stub_Constants);
	ZEPHIR_INIT(Stub_ConstantsInterface);
	ZEPHIR_INIT(Stub_ConstantsInterfaceA);
	ZEPHIR_INIT(Stub_ConstantsInterfaceB);
	ZEPHIR_INIT(Stub_Constructors_Issue1803);
	ZEPHIR_INIT(Stub_DeclareTest);
	ZEPHIR_INIT(Stub_Echoes);
	ZEPHIR_INIT(Stub_EmptyTest);
	ZEPHIR_INIT(Stub_EvalTest);
	ZEPHIR_INIT(Stub_Exception);
	ZEPHIR_INIT(Stub_Exceptions);
	ZEPHIR_INIT(Stub_Exists);
	ZEPHIR_INIT(Stub_ExitDie);
	ZEPHIR_INIT(Stub_Factorial);
	ZEPHIR_INIT(Stub_Fannkuch);
	ZEPHIR_INIT(Stub_Fasta);
	ZEPHIR_INIT(Stub_Fcall);
	ZEPHIR_INIT(Stub_FetchTest);
	ZEPHIR_INIT(Stub_Fibonnaci);
	ZEPHIR_INIT(Stub_Flow);
	ZEPHIR_INIT(Stub_Flow_SwitchFlow);
	ZEPHIR_INIT(Stub_FortyTwo);
	ZEPHIR_INIT(Stub_FunctionExists);
	ZEPHIR_INIT(Stub_Functional);
	ZEPHIR_INIT(Stub_Functions);
	ZEPHIR_INIT(Stub_Geometry);
	ZEPHIR_INIT(Stub_Globals);
	ZEPHIR_INIT(Stub_Globals_Env);
	ZEPHIR_INIT(Stub_Globals_Post);
	ZEPHIR_INIT(Stub_Globals_Server);
	ZEPHIR_INIT(Stub_Globals_ServerRequestFactory);
	ZEPHIR_INIT(Stub_Globals_Session_Child);
	ZEPHIR_INIT(Stub_Instance);
	ZEPHIR_INIT(Stub_Instanceoff);
	ZEPHIR_INIT(Stub_Integration_Psr_ExtendExternal);
	ZEPHIR_INIT(Stub_Interfaces_ImplementInt);
	ZEPHIR_INIT(Stub_Interfaces_ImplementInterface);
	ZEPHIR_INIT(Stub_InternalClasses);
	ZEPHIR_INIT(Stub_InternalInterfaces);
	ZEPHIR_INIT(Stub_Invoke);
	ZEPHIR_INIT(Stub_Invokes_InvokeProtected);
	ZEPHIR_INIT(Stub_Invokes_InvokeProtectedComplex);
	ZEPHIR_INIT(Stub_IssetTest);
	ZEPHIR_INIT(Stub_Issue1134);
	ZEPHIR_INIT(Stub_Issue1404);
	ZEPHIR_INIT(Stub_Issue1521);
	ZEPHIR_INIT(Stub_Issue2165_Issue);
	ZEPHIR_INIT(Stub_Issue663);
	ZEPHIR_INIT(Stub_Issue887);
	ZEPHIR_INIT(Stub_Issue893);
	ZEPHIR_INIT(Stub_Issue914);
	ZEPHIR_INIT(Stub_Issues);
	ZEPHIR_INIT(Stub_Json);
	ZEPHIR_INIT(Stub_Logical);
	ZEPHIR_INIT(Stub_Mcall);
	ZEPHIR_INIT(Stub_McallChained);
	ZEPHIR_INIT(Stub_McallDynamic);
	ZEPHIR_INIT(Stub_McallInternal);
	ZEPHIR_INIT(Stub_Mcall_Caller);
	ZEPHIR_INIT(Stub_MethodAbstract);
	ZEPHIR_INIT(Stub_MethodArgs);
	ZEPHIR_INIT(Stub_Namespaces_A_B_Sub);
	ZEPHIR_INIT(Stub_Namespaces_ClassEntry);
	ZEPHIR_INIT(Stub_NativeArray);
	ZEPHIR_INIT(Stub_Oo);
	ZEPHIR_INIT(Stub_Oo_AbstractClass);
	ZEPHIR_INIT(Stub_Oo_ConcreteStatic);
	ZEPHIR_INIT(Stub_Oo_ConstantsInterface);
	ZEPHIR_INIT(Stub_Oo_DeprecatedMethods);
	ZEPHIR_INIT(Stub_Oo_DynamicProp);
	ZEPHIR_INIT(Stub_Oo_ExtendPdoClass);
	ZEPHIR_INIT(Stub_Oo_Extend_Db_Query_Placeholder_Exception);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_ArrayObject);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_DirectoryIterator);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_DoublyLinkedList);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_FileInfo);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_FileObject);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_FilesystemIterator);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_FixedArray);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_GlobIterator);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_Heap);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_MaxHeap);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_MinHeap);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_PriorityQueue);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_Queue);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_RecursiveDirectoryIterator);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_Stack);
	ZEPHIR_INIT(Stub_Oo_Extend_Spl_TempFileObject);
	ZEPHIR_INIT(Stub_Oo_OoConstruct);
	ZEPHIR_INIT(Stub_Oo_OoConstructParams);
	ZEPHIR_INIT(Stub_Oo_OoDestruct);
	ZEPHIR_INIT(Stub_Oo_OoDynamicB);
	ZEPHIR_INIT(Stub_Oo_OoNativeImplements);
	ZEPHIR_INIT(Stub_Oo_OoNoConstruct);
	ZEPHIR_INIT(Stub_Oo_OoParams);
	ZEPHIR_INIT(Stub_Oo_Param);
	ZEPHIR_INIT(Stub_Oo_PropertyAccess);
	ZEPHIR_INIT(Stub_Oo_Scopes_AbstractClass);
	ZEPHIR_INIT(Stub_Oo_Scopes_AbstractClassMagic);
	ZEPHIR_INIT(Stub_Oo_Scopes_PrivateScopeTester);
	ZEPHIR_INIT(Stub_Operator);
	ZEPHIR_INIT(Stub_Optimizers_ACos);
	ZEPHIR_INIT(Stub_Optimizers_ASin);
	ZEPHIR_INIT(Stub_Optimizers_ArrayMerge);
	ZEPHIR_INIT(Stub_Optimizers_Cos);
	ZEPHIR_INIT(Stub_Optimizers_CreateArray);
	ZEPHIR_INIT(Stub_Optimizers_IsScalar);
	ZEPHIR_INIT(Stub_Optimizers_Ldexp);
	ZEPHIR_INIT(Stub_Optimizers_Sin);
	ZEPHIR_INIT(Stub_Optimizers_Sqrt);
	ZEPHIR_INIT(Stub_Optimizers_StrReplace);
	ZEPHIR_INIT(Stub_Optimizers_Substr);
	ZEPHIR_INIT(Stub_Optimizers_Tan);
	ZEPHIR_INIT(Stub_PdoStatement);
	ZEPHIR_INIT(Stub_Pregmatch);
	ZEPHIR_INIT(Stub_Properties_App);
	ZEPHIR_INIT(Stub_Properties_ExtendsPublicProperties);
	ZEPHIR_INIT(Stub_Properties_GetObjectVars);
	ZEPHIR_INIT(Stub_Properties_PrivateProperties);
	ZEPHIR_INIT(Stub_Properties_PropertyArray);
	ZEPHIR_INIT(Stub_Properties_PropertyUpdate);
	ZEPHIR_INIT(Stub_Properties_ProtectedProperties);
	ZEPHIR_INIT(Stub_Properties_StaticPrivateProperties);
	ZEPHIR_INIT(Stub_Properties_StaticPropertyArray);
	ZEPHIR_INIT(Stub_Properties_StaticProtectedProperties);
	ZEPHIR_INIT(Stub_Properties_StaticPublicProperties);
	ZEPHIR_INIT(Stub_Quantum);
	ZEPHIR_INIT(Stub_Range);
	ZEPHIR_INIT(Stub_References);
	ZEPHIR_INIT(Stub_Reflection);
	ZEPHIR_INIT(Stub_RegexDNA);
	ZEPHIR_INIT(Stub_Requires);
	ZEPHIR_INIT(Stub_Requires_External3);
	ZEPHIR_INIT(Stub_ResourceTest);
	ZEPHIR_INIT(Stub_Returns);
	ZEPHIR_INIT(Stub_Router);
	ZEPHIR_INIT(Stub_Router_Exception);
	ZEPHIR_INIT(Stub_Router_Route);
	ZEPHIR_INIT(Stub_SPropertyAccess);
	ZEPHIR_INIT(Stub_Scall);
	ZEPHIR_INIT(Stub_ScallDynamic);
	ZEPHIR_INIT(Stub_ScallExternal);
	ZEPHIR_INIT(Stub_ScallLateConstruct);
	ZEPHIR_INIT(Stub_Scope);
	ZEPHIR_INIT(Stub_Sort);
	ZEPHIR_INIT(Stub_SpectralNorm);
	ZEPHIR_INIT(Stub_Spl);
	ZEPHIR_INIT(Stub_Statements);
	ZEPHIR_INIT(Stub_Strings);
	ZEPHIR_INIT(Stub_Stubs);
	ZEPHIR_INIT(Stub_Ternary);
	ZEPHIR_INIT(Stub_TryTest);
	ZEPHIR_INIT(Stub_TypeHinting_TestAbstract);
	ZEPHIR_INIT(Stub_TypeInstances);
	ZEPHIR_INIT(Stub_Typeoff);
	ZEPHIR_INIT(Stub_UnknownClass);
	ZEPHIR_INIT(Stub_Unsettest);
	ZEPHIR_INIT(Stub_UseTest);
	ZEPHIR_INIT(Stub_Vars);
	ZEPHIR_INIT(stub_0__closure);
	ZEPHIR_INIT(stub_10__closure);
	ZEPHIR_INIT(stub_11__closure);
	ZEPHIR_INIT(stub_12__closure);
	ZEPHIR_INIT(stub_13__closure);
	ZEPHIR_INIT(stub_1__closure);
	ZEPHIR_INIT(stub_2__closure);
	ZEPHIR_INIT(stub_3__closure);
	ZEPHIR_INIT(stub_4__closure);
	ZEPHIR_INIT(stub_5__closure);
	ZEPHIR_INIT(stub_6__closure);
	ZEPHIR_INIT(stub_7__closure);
	ZEPHIR_INIT(stub_8__closure);
	ZEPHIR_INIT(stub_9__closure);
	
	return SUCCESS;
}

#ifndef ZEPHIR_RELEASE
static PHP_MSHUTDOWN_FUNCTION(stub)
{
	
	zephir_deinitialize_memory();
	UNREGISTER_INI_ENTRIES();
	return SUCCESS;
}
#endif

/**
 * Initialize globals on each request or each thread started
 */
static void php_zephir_init_globals(zend_stub_globals *stub_globals)
{
	stub_globals->initialized = 0;

	/* Cache Enabled */
	stub_globals->cache_enabled = 1;

	/* Recursive Lock */
	stub_globals->recursive_lock = 0;

	/* Static cache */
	memset(stub_globals->scache, '\0', sizeof(zephir_fcall_cache_entry*) * ZEPHIR_MAX_CACHE_SLOTS);

	
	stub_globals->db.my_setting_2 = 100;
	stub_globals->db.my_setting_3 = 7.5;
	stub_globals->orm.cache_level = 3;


	stub_globals->my_setting_1 = 1;
	stub_globals->test_setting_1 = 1;
	stub_globals->my_setting_2 = 10;
	stub_globals->my_setting_3 = 15.2;
	stub_globals->my_setting_4 = 'A';

	
}

/**
 * Initialize globals only on each thread started
 */
static void php_zephir_init_module_globals(zend_stub_globals *stub_globals)
{
	
}

static PHP_RINIT_FUNCTION(stub)
{
	zend_stub_globals *stub_globals_ptr;
	stub_globals_ptr = ZEPHIR_VGLOBAL;

	php_zephir_init_globals(stub_globals_ptr);
	zephir_initialize_memory(stub_globals_ptr);

		zephir_init_static_properties_Stub_Properties_StaticPropertyArray();
		zephir_init_static_properties_Stub_SPropertyAccess();
	
	return SUCCESS;
}

static PHP_RSHUTDOWN_FUNCTION(stub)
{
	
	zephir_deinitialize_memory();
	return SUCCESS;
}



static PHP_MINFO_FUNCTION(stub)
{
	php_info_print_box_start(0);
	php_printf("%s", PHP_STUB_DESCRIPTION);
	php_info_print_box_end();

	php_info_print_table_start();
	php_info_print_table_header(2, PHP_STUB_NAME, "enabled");
	php_info_print_table_row(2, "Author", PHP_STUB_AUTHOR);
	php_info_print_table_row(2, "Version", PHP_STUB_VERSION);
	php_info_print_table_row(2, "Build Date", __DATE__ " " __TIME__ );
	php_info_print_table_row(2, "Powered by Zephir", "Version " PHP_STUB_ZEPVERSION);
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

static PHP_GINIT_FUNCTION(stub)
{
#if defined(COMPILE_DL_STUB) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	php_zephir_init_globals(stub_globals);
	php_zephir_init_module_globals(stub_globals);
}

static PHP_GSHUTDOWN_FUNCTION(stub)
{
	
}

PHP_FUNCTION(g_stub_zephir_global_method_test);
ZEND_BEGIN_ARG_INFO_EX(arginfo_g_stub_zephir_global_method_test, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

PHP_FUNCTION(g_stub_zephir_global_method_with_type_casting);
ZEND_BEGIN_ARG_INFO_EX(arginfo_g_stub_zephir_global_method_with_type_casting, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, variable, stdClass, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Stub_zephir_namespaced_method_test);
ZEND_BEGIN_ARG_INFO_EX(arginfo_f_stub_zephir_namespaced_method_test, 0, 0, 1)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Stub_test_call_relative_object_hint);
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_stub_test_call_relative_object_hint, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, a, Stub\\Oo\\PropertyAccess, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Stub_zephir_namespaced_method_with_type_casting);
ZEND_BEGIN_ARG_INFO_EX(arginfo_f_stub_zephir_namespaced_method_with_type_casting, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, variable, stdClass, 0)
ZEND_END_ARG_INFO()

PHP_FUNCTION(f_Stub_test_call_object_hint);
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_f_stub_test_call_object_hint, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_OBJ_INFO(0, a, Stub\\Oo\\PropertyAccess, 0)
ZEND_END_ARG_INFO()


zend_function_entry php_stub_functions[] = {
	ZEND_NAMED_FE(zephir_global_method_test, ZEND_FN(g_stub_zephir_global_method_test), arginfo_g_stub_zephir_global_method_test)
ZEND_NAMED_FE(zephir_global_method_with_type_casting, ZEND_FN(g_stub_zephir_global_method_with_type_casting), arginfo_g_stub_zephir_global_method_with_type_casting)
ZEND_NS_NAMED_FE("Stub", zephir_namespaced_method_test, ZEND_FN(f_Stub_zephir_namespaced_method_test), arginfo_f_stub_zephir_namespaced_method_test)
ZEND_NS_NAMED_FE("Stub", test_call_relative_object_hint, ZEND_FN(f_Stub_test_call_relative_object_hint), arginfo_f_stub_test_call_relative_object_hint)
ZEND_NS_NAMED_FE("Stub", zephir_namespaced_method_with_type_casting, ZEND_FN(f_Stub_zephir_namespaced_method_with_type_casting), arginfo_f_stub_zephir_namespaced_method_with_type_casting)
ZEND_NS_NAMED_FE("Stub", test_call_object_hint, ZEND_FN(f_Stub_test_call_object_hint), arginfo_f_stub_test_call_object_hint)
ZEND_FE_END

};

static const zend_module_dep php_stub_deps[] = {
	ZEND_MOD_REQUIRED("pdo")
	ZEND_MOD_REQUIRED("spl")
	ZEND_MOD_REQUIRED("standard")
	ZEND_MOD_REQUIRED("hash")
	ZEND_MOD_REQUIRED("json")
	ZEND_MOD_END
};

zend_module_entry stub_module_entry = {
	STANDARD_MODULE_HEADER_EX,
	NULL,
	php_stub_deps,
	PHP_STUB_EXTNAME,
	php_stub_functions,
	PHP_MINIT(stub),
#ifndef ZEPHIR_RELEASE
	PHP_MSHUTDOWN(stub),
#else
	NULL,
#endif
	PHP_RINIT(stub),
	PHP_RSHUTDOWN(stub),
	PHP_MINFO(stub),
	PHP_STUB_VERSION,
	ZEND_MODULE_GLOBALS(stub),
	PHP_GINIT(stub),
	PHP_GSHUTDOWN(stub),
#ifdef ZEPHIR_POST_REQUEST
	PHP_PRSHUTDOWN(stub),
#else
	NULL,
#endif
	STANDARD_MODULE_PROPERTIES_EX
};

/* implement standard "stub" routine to introduce ourselves to Zend */
#ifdef COMPILE_DL_STUB
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(stub)
#endif
