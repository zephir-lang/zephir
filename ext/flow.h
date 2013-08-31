
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
PHP_METHOD(Test_Flow, testDoWhile1);
PHP_METHOD(Test_Flow, testFor1);
PHP_METHOD(Test_Flow, testFor2);
PHP_METHOD(Test_Flow, testFor3);
PHP_METHOD(Test_Flow, testFor4);
PHP_METHOD(Test_Flow, testFor10);
PHP_METHOD(Test_Flow, testFor11);

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
	PHP_ME(Test_Flow, testDoWhile1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Flow, testFor11, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
