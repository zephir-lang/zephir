
extern zend_class_entry *test_vars_ce;

ZEPHIR_INIT_CLASS(Test_Vars);

PHP_METHOD(Test_Vars, testVarDump);
PHP_METHOD(Test_Vars, testVarDump2);
PHP_METHOD(Test_Vars, testVarExport);
PHP_METHOD(Test_Vars, test88Issue);
PHP_METHOD(Test_Vars, test88IssueParam2InitString);
PHP_METHOD(Test_Vars, testVarDump2param);
PHP_METHOD(Test_Vars, testVarDump3param);
PHP_METHOD(Test_Vars, testCountOptimizerVarDumpAndExport);
PHP_METHOD(Test_Vars, testArrayTypeVarDumpAndExport);
PHP_METHOD(Test_Vars, testIntVarDump);
PHP_METHOD(Test_Vars, testDoubleVarDump);
PHP_METHOD(Test_Vars, testBoolVarDump);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_vars_testvardump2, 0, 0, 1)
	ZEND_ARG_INFO(0, ret)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_vars_test88issue, 0, 0, 1)
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_vars_test88issueparam2initstring, 0, 0, 1)
	ZEND_ARG_INFO(0, param1)
	ZEND_ARG_INFO(0, param2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_vars_testvardump2param, 0, 0, 2)
	ZEND_ARG_INFO(0, p1)
	ZEND_ARG_INFO(0, p2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_vars_testvardump3param, 0, 0, 3)
	ZEND_ARG_INFO(0, p1)
	ZEND_ARG_INFO(0, p2)
	ZEND_ARG_INFO(0, p3)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_vars_testcountoptimizervardumpandexport, 0, 0, 1)
	ZEND_ARG_INFO(0, testVar)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_vars_testarraytypevardumpandexport, 0, 0, 0)
	ZEND_ARG_ARRAY_INFO(0, testVar, 1)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_vars_method_entry) {
	PHP_ME(Test_Vars, testVarDump, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testVarDump2, arginfo_test_vars_testvardump2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testVarExport, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, test88Issue, arginfo_test_vars_test88issue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, test88IssueParam2InitString, arginfo_test_vars_test88issueparam2initstring, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testVarDump2param, arginfo_test_vars_testvardump2param, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testVarDump3param, arginfo_test_vars_testvardump3param, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testCountOptimizerVarDumpAndExport, arginfo_test_vars_testcountoptimizervardumpandexport, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testArrayTypeVarDumpAndExport, arginfo_test_vars_testarraytypevardumpandexport, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testIntVarDump, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testDoubleVarDump, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Vars, testBoolVarDump, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
