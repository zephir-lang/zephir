
extern zend_class_entry *stub_flow_ce;

ZEPHIR_INIT_CLASS(Stub_Flow);

PHP_METHOD(Stub_Flow, testIf1);
PHP_METHOD(Stub_Flow, testIf2);
PHP_METHOD(Stub_Flow, testIf3);
PHP_METHOD(Stub_Flow, testIf4);
PHP_METHOD(Stub_Flow, testIf5);
PHP_METHOD(Stub_Flow, testIf6);
PHP_METHOD(Stub_Flow, testIf7);
PHP_METHOD(Stub_Flow, testIf8);
PHP_METHOD(Stub_Flow, testIf9);
PHP_METHOD(Stub_Flow, testIf10);
PHP_METHOD(Stub_Flow, testIf12);
PHP_METHOD(Stub_Flow, testIf13);
PHP_METHOD(Stub_Flow, testIf14);
PHP_METHOD(Stub_Flow, testIf15);
PHP_METHOD(Stub_Flow, testIf16);
PHP_METHOD(Stub_Flow, testIf17);
PHP_METHOD(Stub_Flow, testLoop1);
PHP_METHOD(Stub_Flow, testLoop2);
PHP_METHOD(Stub_Flow, testLoop3);
PHP_METHOD(Stub_Flow, testLoop4);
PHP_METHOD(Stub_Flow, testWhile1);
PHP_METHOD(Stub_Flow, testWhile2);
PHP_METHOD(Stub_Flow, testWhile3);
PHP_METHOD(Stub_Flow, testWhile4);
PHP_METHOD(Stub_Flow, testWhile5);
PHP_METHOD(Stub_Flow, testWhile6);
PHP_METHOD(Stub_Flow, testWhile7);
PHP_METHOD(Stub_Flow, testWhile8);
PHP_METHOD(Stub_Flow, testWhile9);
PHP_METHOD(Stub_Flow, testWhile10);
PHP_METHOD(Stub_Flow, testWhile11);
PHP_METHOD(Stub_Flow, testWhile12);
PHP_METHOD(Stub_Flow, testWhile13);
PHP_METHOD(Stub_Flow, testDoWhile1);
PHP_METHOD(Stub_Flow, testWhileNextTest);
PHP_METHOD(Stub_Flow, testWhileDoNextTest);
PHP_METHOD(Stub_Flow, testFor1);
PHP_METHOD(Stub_Flow, testFor2);
PHP_METHOD(Stub_Flow, testFor3);
PHP_METHOD(Stub_Flow, testFor4);
PHP_METHOD(Stub_Flow, testFor5);
PHP_METHOD(Stub_Flow, testFor6);
PHP_METHOD(Stub_Flow, testFor7);
PHP_METHOD(Stub_Flow, testFor8);
PHP_METHOD(Stub_Flow, testFor9);
PHP_METHOD(Stub_Flow, testFor10);
PHP_METHOD(Stub_Flow, testFor11);
PHP_METHOD(Stub_Flow, testFor12);
PHP_METHOD(Stub_Flow, testFor13);
PHP_METHOD(Stub_Flow, testFor14);
PHP_METHOD(Stub_Flow, testFor15);
PHP_METHOD(Stub_Flow, testFor16);
PHP_METHOD(Stub_Flow, testFor17);
PHP_METHOD(Stub_Flow, testFor18);
PHP_METHOD(Stub_Flow, testFor19);
PHP_METHOD(Stub_Flow, testFor20);
PHP_METHOD(Stub_Flow, testFor21);
PHP_METHOD(Stub_Flow, testFor22);
PHP_METHOD(Stub_Flow, testFor23);
PHP_METHOD(Stub_Flow, testFor24);
PHP_METHOD(Stub_Flow, testFor30);
PHP_METHOD(Stub_Flow, testFor31);
PHP_METHOD(Stub_Flow, testFor32);
PHP_METHOD(Stub_Flow, testFor33);
PHP_METHOD(Stub_Flow, testFor34);
PHP_METHOD(Stub_Flow, testFor35Aux);
PHP_METHOD(Stub_Flow, testFor35);
PHP_METHOD(Stub_Flow, testFor36Aux);
PHP_METHOD(Stub_Flow, testFor36);
PHP_METHOD(Stub_Flow, testFor37);
PHP_METHOD(Stub_Flow, testFor38);
PHP_METHOD(Stub_Flow, testFor39);
PHP_METHOD(Stub_Flow, testFor40);
PHP_METHOD(Stub_Flow, testUnrechable1);
PHP_METHOD(Stub_Flow, testUnrechable2);
PHP_METHOD(Stub_Flow, testUnrechable3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testif16, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testif17, 0, 0, 1)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testwhile10, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testwhile11, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, d)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testwhilenexttest, 0, 0, 1)
	ZEND_ARG_INFO(0, variable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testwhiledonexttest, 0, 0, 1)
	ZEND_ARG_INFO(0, variable)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testfor15, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, c, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, c)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, d, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, d)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testfor32, 0, 0, 1)
	ZEND_ARG_INFO(0, e)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testfor33, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, e, Iterator, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testfor34, 0, 0, 0)
	ZEND_ARG_OBJ_INFO(0, e, Iterator, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testfor35aux, 0, 0, 1)
	ZEND_ARG_INFO(0, hello)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_flow_testfor36aux, 0, 0, 1)
	ZEND_ARG_INFO(0, hello)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_flow_method_entry) {
	PHP_ME(Stub_Flow, testIf1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf15, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf16, arginfo_stub_flow_testif16, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testIf17, arginfo_stub_flow_testif17, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testLoop1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testLoop2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testLoop3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testLoop4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile10, arginfo_stub_flow_testwhile10, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile11, arginfo_stub_flow_testwhile11, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhile13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testDoWhile1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhileNextTest, arginfo_stub_flow_testwhilenexttest, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testWhileDoNextTest, arginfo_stub_flow_testwhiledonexttest, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor15, arginfo_stub_flow_testfor15, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor16, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor17, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor18, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor19, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor20, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor21, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor22, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor23, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor24, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor30, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor31, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor32, arginfo_stub_flow_testfor32, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor33, arginfo_stub_flow_testfor33, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor34, arginfo_stub_flow_testfor34, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor35Aux, arginfo_stub_flow_testfor35aux, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor35, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor36Aux, arginfo_stub_flow_testfor36aux, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor36, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor37, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor38, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor39, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testFor40, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testUnrechable1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testUnrechable2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Flow, testUnrechable3, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
