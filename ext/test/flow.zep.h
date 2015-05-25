
extern zend_class_entry *test_flow_ce;

ZEPHIR_INIT_CLASS(Test_Flow);

PHP_METHOD(Test_Flow, testIf1);
PHP_METHOD(Test_Flow, testIf2);
PHP_METHOD(Test_Flow, testIf3);
PHP_METHOD(Test_Flow, testIf4);
PHP_METHOD(Test_Flow, testIf5);
PHP_METHOD(Test_Flow, testIf6);
PHP_METHOD(Test_Flow, testIf7);
PHP_METHOD(Test_Flow, testIf8);
PHP_METHOD(Test_Flow, testIf9);
PHP_METHOD(Test_Flow, testIf10);
PHP_METHOD(Test_Flow, testIf12);
PHP_METHOD(Test_Flow, testIf13);
PHP_METHOD(Test_Flow, testIf14);
PHP_METHOD(Test_Flow, testIf15);
PHP_METHOD(Test_Flow, testIf16);
PHP_METHOD(Test_Flow, testIf17);
PHP_METHOD(Test_Flow, testLoop1);
PHP_METHOD(Test_Flow, testLoop2);
PHP_METHOD(Test_Flow, testLoop3);
PHP_METHOD(Test_Flow, testLoop4);
PHP_METHOD(Test_Flow, testWhile1);
PHP_METHOD(Test_Flow, testWhile2);
PHP_METHOD(Test_Flow, testWhile3);
PHP_METHOD(Test_Flow, testWhile4);
PHP_METHOD(Test_Flow, testWhile5);
PHP_METHOD(Test_Flow, testWhile6);
PHP_METHOD(Test_Flow, testWhile7);
PHP_METHOD(Test_Flow, testWhile8);
PHP_METHOD(Test_Flow, testWhile9);
PHP_METHOD(Test_Flow, testWhile10);
PHP_METHOD(Test_Flow, testWhile11);
PHP_METHOD(Test_Flow, testWhile12);
PHP_METHOD(Test_Flow, testWhile13);
PHP_METHOD(Test_Flow, testDoWhile1);
PHP_METHOD(Test_Flow, testWhileNextTest);
PHP_METHOD(Test_Flow, testWhileDoNextTest);
PHP_METHOD(Test_Flow, testFor1);
PHP_METHOD(Test_Flow, testFor2);
PHP_METHOD(Test_Flow, testFor3);
PHP_METHOD(Test_Flow, testFor4);
PHP_METHOD(Test_Flow, testFor5);
PHP_METHOD(Test_Flow, testFor6);
PHP_METHOD(Test_Flow, testFor7);
PHP_METHOD(Test_Flow, testFor8);
PHP_METHOD(Test_Flow, testFor9);
PHP_METHOD(Test_Flow, testFor10);
PHP_METHOD(Test_Flow, testFor11);
PHP_METHOD(Test_Flow, testFor12);
PHP_METHOD(Test_Flow, testFor13);
PHP_METHOD(Test_Flow, testFor14);
PHP_METHOD(Test_Flow, testFor15);
PHP_METHOD(Test_Flow, testFor16);
PHP_METHOD(Test_Flow, testFor17);
PHP_METHOD(Test_Flow, testFor18);
PHP_METHOD(Test_Flow, testFor19);
PHP_METHOD(Test_Flow, testFor20);
PHP_METHOD(Test_Flow, testFor21);
PHP_METHOD(Test_Flow, testFor22);
PHP_METHOD(Test_Flow, testFor23);
PHP_METHOD(Test_Flow, testFor24);
PHP_METHOD(Test_Flow, testFor30);
PHP_METHOD(Test_Flow, testFor31);
PHP_METHOD(Test_Flow, testFor32);
PHP_METHOD(Test_Flow, testFor33);
PHP_METHOD(Test_Flow, testFor34);
PHP_METHOD(Test_Flow, testFor35Aux);
PHP_METHOD(Test_Flow, testFor35);
PHP_METHOD(Test_Flow, testFor36Aux);
PHP_METHOD(Test_Flow, testFor36);
PHP_METHOD(Test_Flow, testFor37);
PHP_METHOD(Test_Flow, testFor38);
PHP_METHOD(Test_Flow, testFor39);
PHP_METHOD(Test_Flow, testFor40);
PHP_METHOD(Test_Flow, testSwitch1);
PHP_METHOD(Test_Flow, testSwitch2);
PHP_METHOD(Test_Flow, testSwitch3);
PHP_METHOD(Test_Flow, testSwitch4);
PHP_METHOD(Test_Flow, testSwitch5);
PHP_METHOD(Test_Flow, testSwitch6);
PHP_METHOD(Test_Flow, testSwitch7);
PHP_METHOD(Test_Flow, testSwitch8);
PHP_METHOD(Test_Flow, testSwitch9);
PHP_METHOD(Test_Flow, testSwitch10);
PHP_METHOD(Test_Flow, testSwitch11);
PHP_METHOD(Test_Flow, testSwitch12);
PHP_METHOD(Test_Flow, testUnrechable1);
PHP_METHOD(Test_Flow, testUnrechable2);
PHP_METHOD(Test_Flow, testUnrechable3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testif16, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testif17, 0, 0, 1)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testwhile10, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testwhile11, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, d)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testwhilenexttest, 0, 0, 1)
	ZEND_ARG_INFO(0, variable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testwhiledonexttest, 0, 0, 1)
	ZEND_ARG_INFO(0, variable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testfor15, 0, 0, 2)
	ZEND_ARG_INFO(0, c)
	ZEND_ARG_INFO(0, d)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testfor32, 0, 0, 1)
	ZEND_ARG_INFO(0, e)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testfor33, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, e, Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testfor34, 0, 0, 0)
	ZEND_ARG_OBJ_INFO(0, e, Iterator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testfor35aux, 0, 0, 1)
	ZEND_ARG_INFO(0, hello)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testfor36aux, 0, 0, 1)
	ZEND_ARG_INFO(0, hello)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_flow_testswitch12, 0, 0, 2)
	ZEND_ARG_INFO(0, var1)
	ZEND_ARG_INFO(0, var2)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_flow_method_entry) {
	PHP_ME(Test_Flow, testIf1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf15, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf16, arginfo_test_flow_testif16, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testIf17, arginfo_test_flow_testif17, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testLoop1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testLoop2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testLoop3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testLoop4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile10, arginfo_test_flow_testwhile10, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile11, arginfo_test_flow_testwhile11, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhile13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testDoWhile1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhileNextTest, arginfo_test_flow_testwhilenexttest, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testWhileDoNextTest, arginfo_test_flow_testwhiledonexttest, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor15, arginfo_test_flow_testfor15, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor16, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor17, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor18, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor19, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor20, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor21, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor22, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor23, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor24, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor30, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor31, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor32, arginfo_test_flow_testfor32, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor33, arginfo_test_flow_testfor33, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor34, arginfo_test_flow_testfor34, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor35Aux, arginfo_test_flow_testfor35aux, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor35, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor36Aux, arginfo_test_flow_testfor36aux, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor36, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor37, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor38, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor39, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor40, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testSwitch12, arginfo_test_flow_testswitch12, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testUnrechable1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testUnrechable2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testUnrechable3, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
