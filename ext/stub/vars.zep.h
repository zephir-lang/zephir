
extern zend_class_entry *stub_vars_ce;

ZEPHIR_INIT_CLASS(Stub_Vars);

PHP_METHOD(Stub_Vars, testParam);
PHP_METHOD(Stub_Vars, testVarDump);
PHP_METHOD(Stub_Vars, testVarDump2);
PHP_METHOD(Stub_Vars, testVarExport);
PHP_METHOD(Stub_Vars, test88Issue);
PHP_METHOD(Stub_Vars, test88IssueParam2InitString);
PHP_METHOD(Stub_Vars, testVarDump2param);
PHP_METHOD(Stub_Vars, testVarDump3param);
PHP_METHOD(Stub_Vars, testCountOptimizerVarDumpAndExport);
PHP_METHOD(Stub_Vars, testArrayTypeVarDumpAndExport);
PHP_METHOD(Stub_Vars, testIntVarDump);
PHP_METHOD(Stub_Vars, testDoubleVarDump);
PHP_METHOD(Stub_Vars, testBoolVarDump);
PHP_METHOD(Stub_Vars, testGetDefinedVars);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testparam, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, config, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testvardump, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testvardump2, 0, 0, 1)
	ZEND_ARG_INFO(0, ret)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testvarexport, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_test88issue, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, param1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, param2, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_test88issueparam2initstring, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, param1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, param2, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testvardump2param, 0, 0, 2)
	ZEND_ARG_INFO(0, p1)
	ZEND_ARG_INFO(0, p2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testvardump3param, 0, 0, 3)
	ZEND_ARG_INFO(0, p1)
	ZEND_ARG_INFO(0, p2)
	ZEND_ARG_INFO(0, p3)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testcountoptimizervardumpandexport, 0, 0, 1)
	ZEND_ARG_INFO(0, testVar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testarraytypevardumpandexport, 0, 0, 0)
ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, testVar, IS_ARRAY, 0, "[]")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testintvardump, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testdoublevardump, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testboolvardump, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_vars_testgetdefinedvars, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_vars_method_entry) {
	PHP_ME(Stub_Vars, testParam, arginfo_stub_vars_testparam, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Vars, testVarDump, arginfo_stub_vars_testvardump, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Vars, testVarDump2, arginfo_stub_vars_testvardump2, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Vars, testVarExport, arginfo_stub_vars_testvarexport, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Vars, test88Issue, arginfo_stub_vars_test88issue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Vars, test88IssueParam2InitString, arginfo_stub_vars_test88issueparam2initstring, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Vars, testVarDump2param, arginfo_stub_vars_testvardump2param, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Vars, testVarDump3param, arginfo_stub_vars_testvardump3param, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Vars, testCountOptimizerVarDumpAndExport, arginfo_stub_vars_testcountoptimizervardumpandexport, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Vars, testArrayTypeVarDumpAndExport, arginfo_stub_vars_testarraytypevardumpandexport, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Vars, testIntVarDump, arginfo_stub_vars_testintvardump, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Vars, testDoubleVarDump, arginfo_stub_vars_testdoublevardump, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Vars, testBoolVarDump, arginfo_stub_vars_testboolvardump, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Vars, testGetDefinedVars, arginfo_stub_vars_testgetdefinedvars, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
