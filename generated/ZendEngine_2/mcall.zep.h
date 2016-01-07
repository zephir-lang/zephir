
extern zend_class_entry *test_mcall_ce;

ZEPHIR_INIT_CLASS(Test_Mcall);

PHP_METHOD(Test_Mcall, testMethod1);
PHP_METHOD(Test_Mcall, testMethod2);
PHP_METHOD(Test_Mcall, testMethod3);
PHP_METHOD(Test_Mcall, testMethod4);
PHP_METHOD(Test_Mcall, testMethod5);
PHP_METHOD(Test_Mcall, testMethod6);
PHP_METHOD(Test_Mcall, testMethod7);
PHP_METHOD(Test_Mcall, testCall1);
PHP_METHOD(Test_Mcall, testCall2);
PHP_METHOD(Test_Mcall, testCall3);
PHP_METHOD(Test_Mcall, testCall4);
PHP_METHOD(Test_Mcall, testCall5);
PHP_METHOD(Test_Mcall, testCall6);
PHP_METHOD(Test_Mcall, testCall7);
PHP_METHOD(Test_Mcall, testCall8);
PHP_METHOD(Test_Mcall, testCall9);
PHP_METHOD(Test_Mcall, testCall10);
PHP_METHOD(Test_Mcall, testCall11);
PHP_METHOD(Test_Mcall, testCall12);
PHP_METHOD(Test_Mcall, testCall13);
PHP_METHOD(Test_Mcall, testCall14);
PHP_METHOD(Test_Mcall, testCall15);
PHP_METHOD(Test_Mcall, testCall16);
PHP_METHOD(Test_Mcall, testCall17);
PHP_METHOD(Test_Mcall, testCall18);
PHP_METHOD(Test_Mcall, testMethod19);
PHP_METHOD(Test_Mcall, testCall20);
PHP_METHOD(Test_Mcall, testMethod21);
PHP_METHOD(Test_Mcall, testCall22);
PHP_METHOD(Test_Mcall, optionalRequereString);
PHP_METHOD(Test_Mcall, optionalParameterString);
PHP_METHOD(Test_Mcall, optionalParameterStringNull);
PHP_METHOD(Test_Mcall, optionalParameterInt);
PHP_METHOD(Test_Mcall, optionalParameterVar);
PHP_METHOD(Test_Mcall, optionalParameterBoolTrue);
PHP_METHOD(Test_Mcall, optionalParameterBoolFalse);
PHP_METHOD(Test_Mcall, optionalParameterBoolean);
PHP_METHOD(Test_Mcall, optionalParameterBooleanNull);
PHP_METHOD(Test_Mcall, arrayParamWithDefaultEmptyArray);
PHP_METHOD(Test_Mcall, arrayParamWithDefaultNullValue);
PHP_METHOD(Test_Mcall, arrayParam);
PHP_METHOD(Test_Mcall, objectParamCastStdClass);
PHP_METHOD(Test_Mcall, objectParamCastOoParam);
PHP_METHOD(Test_Mcall, bb);
PHP_METHOD(Test_Mcall, testCallablePass);
PHP_METHOD(Test_Mcall, testCallableArrayThisMethodPass);
PHP_METHOD(Test_Mcall, aa);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall7, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall8, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall9, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall13, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall14, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall15, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall16, 0, 0, 2)
	ZEND_ARG_INFO(0, c)
	ZEND_ARG_INFO(0, d)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall17, 0, 0, 2)
	ZEND_ARG_INFO(0, c)
	ZEND_ARG_INFO(0, d)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod19, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall20, 0, 0, 2)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod21, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall22, 0, 0, 2)
	ZEND_ARG_INFO(0, k)
	ZEND_ARG_INFO(0, p)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalrequerestring, 0, 0, 1)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparameterstring, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparameterstringnull, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparameterint, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparametervar, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparameterbooltrue, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparameterboolfalse, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparameterboolean, 0, 0, 0)
	ZEND_ARG_INFO(0, start)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_optionalparameterbooleannull, 0, 0, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_arrayparamwithdefaultemptyarray, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_arrayparamwithdefaultnullvalue, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_arrayparam, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, driverOptions, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_objectparamcaststdclass, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, param, StdClass, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_objectparamcastooparam, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, param, Test\\Oo\\Param, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_mcall_method_entry) {
	PHP_ME(Test_Mcall, testMethod1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testMethod2, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Test_Mcall, testMethod3, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Mcall, testMethod4, arginfo_test_mcall_testmethod4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testMethod5, arginfo_test_mcall_testmethod5, ZEND_ACC_PROTECTED)
	PHP_ME(Test_Mcall, testMethod6, arginfo_test_mcall_testmethod6, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Mcall, testMethod7, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Mcall, testCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall4, arginfo_test_mcall_testcall4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall5, arginfo_test_mcall_testcall5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall6, arginfo_test_mcall_testcall6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall7, arginfo_test_mcall_testcall7, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall8, arginfo_test_mcall_testcall8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall9, arginfo_test_mcall_testcall9, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall13, arginfo_test_mcall_testcall13, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall14, arginfo_test_mcall_testcall14, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall15, arginfo_test_mcall_testcall15, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall16, arginfo_test_mcall_testcall16, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall17, arginfo_test_mcall_testcall17, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall18, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testMethod19, arginfo_test_mcall_testmethod19, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall20, arginfo_test_mcall_testcall20, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testMethod21, arginfo_test_mcall_testmethod21, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Mcall, testCall22, arginfo_test_mcall_testcall22, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalRequereString, arginfo_test_mcall_optionalrequerestring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterString, arginfo_test_mcall_optionalparameterstring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterStringNull, arginfo_test_mcall_optionalparameterstringnull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterInt, arginfo_test_mcall_optionalparameterint, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterVar, arginfo_test_mcall_optionalparametervar, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterBoolTrue, arginfo_test_mcall_optionalparameterbooltrue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterBoolFalse, arginfo_test_mcall_optionalparameterboolfalse, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterBoolean, arginfo_test_mcall_optionalparameterboolean, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, optionalParameterBooleanNull, arginfo_test_mcall_optionalparameterbooleannull, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, arrayParamWithDefaultEmptyArray, arginfo_test_mcall_arrayparamwithdefaultemptyarray, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, arrayParamWithDefaultNullValue, arginfo_test_mcall_arrayparamwithdefaultnullvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, arrayParam, arginfo_test_mcall_arrayparam, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, objectParamCastStdClass, arginfo_test_mcall_objectparamcaststdclass, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, objectParamCastOoParam, arginfo_test_mcall_objectparamcastooparam, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, bb, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Mcall, testCallablePass, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCallableArrayThisMethodPass, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, aa, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
