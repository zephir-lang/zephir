
extern zend_class_entry *stub_mcall_ce;

ZEPHIR_INIT_CLASS(Stub_Mcall);

PHP_METHOD(Stub_Mcall, testMethod1);
PHP_METHOD(Stub_Mcall, testMethod2);
PHP_METHOD(Stub_Mcall, testMethod3);
PHP_METHOD(Stub_Mcall, testMethod4);
PHP_METHOD(Stub_Mcall, testMethod5);
PHP_METHOD(Stub_Mcall, testMethod6);
PHP_METHOD(Stub_Mcall, testMethod7);
PHP_METHOD(Stub_Mcall, testCall1);
PHP_METHOD(Stub_Mcall, testCall2);
PHP_METHOD(Stub_Mcall, testCall3);
PHP_METHOD(Stub_Mcall, testCall4);
PHP_METHOD(Stub_Mcall, testCall5);
PHP_METHOD(Stub_Mcall, testCall6);
PHP_METHOD(Stub_Mcall, testCall7);
PHP_METHOD(Stub_Mcall, testCall8);
PHP_METHOD(Stub_Mcall, testCall9);
PHP_METHOD(Stub_Mcall, testCall10);
PHP_METHOD(Stub_Mcall, testCall11);
PHP_METHOD(Stub_Mcall, testCall12);
PHP_METHOD(Stub_Mcall, testCall13);
PHP_METHOD(Stub_Mcall, testCall14);
PHP_METHOD(Stub_Mcall, testCall15);
PHP_METHOD(Stub_Mcall, testCall16);
PHP_METHOD(Stub_Mcall, testCall17);
PHP_METHOD(Stub_Mcall, testCall18);
PHP_METHOD(Stub_Mcall, testMethod19);
PHP_METHOD(Stub_Mcall, testCall20);
PHP_METHOD(Stub_Mcall, testMethod21);
PHP_METHOD(Stub_Mcall, testCall22);
PHP_METHOD(Stub_Mcall, optionalRequereString);
PHP_METHOD(Stub_Mcall, optionalParameterString);
PHP_METHOD(Stub_Mcall, optionalParameterStringNull);
PHP_METHOD(Stub_Mcall, optionalParameterInt);
PHP_METHOD(Stub_Mcall, optionalParameterVar);
PHP_METHOD(Stub_Mcall, optionalParameterBoolTrue);
PHP_METHOD(Stub_Mcall, optionalParameterBoolFalse);
PHP_METHOD(Stub_Mcall, optionalParameterBoolean);
PHP_METHOD(Stub_Mcall, optionalParameterBooleanNull);
PHP_METHOD(Stub_Mcall, testArrayParamWithDefaultEmptyArray);
PHP_METHOD(Stub_Mcall, testArrayParamWithDefaultNullValue);
PHP_METHOD(Stub_Mcall, testArrayParam);
PHP_METHOD(Stub_Mcall, testObjectParamCastStdClass);
PHP_METHOD(Stub_Mcall, testObjectParamCastOoParam);
PHP_METHOD(Stub_Mcall, bb);
PHP_METHOD(Stub_Mcall, testCallablePass);
PHP_METHOD(Stub_Mcall, testCallableArrayThisMethodPass);
PHP_METHOD(Stub_Mcall, aa);
PHP_METHOD(Stub_Mcall, issue1136);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_mcall_testmethod7, 0, 0, stdClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall3, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall7, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall8, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall9, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall10, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall11, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall12, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall13, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall14, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall15, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall16, 0, 0, 2)
	ZEND_ARG_INFO(0, c)
	ZEND_ARG_INFO(0, d)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall17, 0, 0, 2)
	ZEND_ARG_INFO(0, c)
	ZEND_ARG_INFO(0, d)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall18, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod19, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall20, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testmethod21, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, a, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcall22, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 0)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalrequerestring, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, param, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparameterstring, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, param, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparameterstringnull, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, param, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparameterint, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, param, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparametervar, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparameterbooltrue, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, param, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparameterboolfalse, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, param, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparameterboolean, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, start, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_optionalparameterbooleannull, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, value, _IS_BOOL, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testarrayparamwithdefaultemptyarray, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, driverOptions, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testarrayparamwithdefaultnullvalue, 0, 0, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, driverOptions, IS_ARRAY, 1, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testarrayparam, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testobjectparamcaststdclass, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, param, StdClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testobjectparamcastooparam, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, param, Stub\\Oo\\Param, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_bb, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcallablepass, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_testcallablearraythismethodpass, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_aa, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_mcall_issue1136, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_mcall_method_entry) {
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testMethod1, arginfo_stub_mcall_testmethod1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testMethod1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testMethod2, arginfo_stub_mcall_testmethod2, ZEND_ACC_PROTECTED)
#else
	PHP_ME(Stub_Mcall, testMethod2, NULL, ZEND_ACC_PROTECTED)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testMethod3, arginfo_stub_mcall_testmethod3, ZEND_ACC_PRIVATE)
#else
	PHP_ME(Stub_Mcall, testMethod3, NULL, ZEND_ACC_PRIVATE)
#endif
	PHP_ME(Stub_Mcall, testMethod4, arginfo_stub_mcall_testmethod4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testMethod5, arginfo_stub_mcall_testmethod5, ZEND_ACC_PROTECTED)
	PHP_ME(Stub_Mcall, testMethod6, arginfo_stub_mcall_testmethod6, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Mcall, testMethod7, arginfo_stub_mcall_testmethod7, ZEND_ACC_PRIVATE)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCall1, arginfo_stub_mcall_testcall1, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCall1, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCall2, arginfo_stub_mcall_testcall2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCall2, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCall3, arginfo_stub_mcall_testcall3, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCall3, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Mcall, testCall4, arginfo_stub_mcall_testcall4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall5, arginfo_stub_mcall_testcall5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall6, arginfo_stub_mcall_testcall6, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall7, arginfo_stub_mcall_testcall7, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall8, arginfo_stub_mcall_testcall8, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall9, arginfo_stub_mcall_testcall9, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCall10, arginfo_stub_mcall_testcall10, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCall10, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCall11, arginfo_stub_mcall_testcall11, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCall11, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCall12, arginfo_stub_mcall_testcall12, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCall12, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Mcall, testCall13, arginfo_stub_mcall_testcall13, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall14, arginfo_stub_mcall_testcall14, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall15, arginfo_stub_mcall_testcall15, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall16, arginfo_stub_mcall_testcall16, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall17, arginfo_stub_mcall_testcall17, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCall18, arginfo_stub_mcall_testcall18, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCall18, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_ME(Stub_Mcall, testMethod19, arginfo_stub_mcall_testmethod19, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testCall20, arginfo_stub_mcall_testcall20, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testMethod21, arginfo_stub_mcall_testmethod21, ZEND_ACC_PRIVATE)
	PHP_ME(Stub_Mcall, testCall22, arginfo_stub_mcall_testcall22, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalRequereString, arginfo_stub_mcall_optionalrequerestring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterString, arginfo_stub_mcall_optionalparameterstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterStringNull, arginfo_stub_mcall_optionalparameterstringnull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterInt, arginfo_stub_mcall_optionalparameterint, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterVar, arginfo_stub_mcall_optionalparametervar, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterBoolTrue, arginfo_stub_mcall_optionalparameterbooltrue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterBoolFalse, arginfo_stub_mcall_optionalparameterboolfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterBoolean, arginfo_stub_mcall_optionalparameterboolean, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, optionalParameterBooleanNull, arginfo_stub_mcall_optionalparameterbooleannull, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testArrayParamWithDefaultEmptyArray, arginfo_stub_mcall_testarrayparamwithdefaultemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testArrayParamWithDefaultNullValue, arginfo_stub_mcall_testarrayparamwithdefaultnullvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testArrayParam, arginfo_stub_mcall_testarrayparam, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testObjectParamCastStdClass, arginfo_stub_mcall_testobjectparamcaststdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Mcall, testObjectParamCastOoParam, arginfo_stub_mcall_testobjectparamcastooparam, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, bb, arginfo_stub_mcall_bb, ZEND_ACC_PRIVATE)
#else
	PHP_ME(Stub_Mcall, bb, NULL, ZEND_ACC_PRIVATE)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCallablePass, arginfo_stub_mcall_testcallablepass, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCallablePass, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, testCallableArrayThisMethodPass, arginfo_stub_mcall_testcallablearraythismethodpass, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, testCallableArrayThisMethodPass, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, aa, arginfo_stub_mcall_aa, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, aa, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Mcall, issue1136, arginfo_stub_mcall_issue1136, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Mcall, issue1136, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
