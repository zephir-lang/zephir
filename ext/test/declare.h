
extern zend_class_entry *test_declare_ce;

ZEPHIR_INIT_CLASS(Test_Declare);

PHP_METHOD(Test_Declare, testDeclare1);
PHP_METHOD(Test_Declare, testDeclare2);
PHP_METHOD(Test_Declare, testDeclare3);
PHP_METHOD(Test_Declare, testDeclare4);
PHP_METHOD(Test_Declare, testDeclare5);
PHP_METHOD(Test_Declare, testDeclare6);
PHP_METHOD(Test_Declare, testDeclare7);
PHP_METHOD(Test_Declare, testDeclare8);
PHP_METHOD(Test_Declare, testDeclare9);
PHP_METHOD(Test_Declare, testDeclare10);
PHP_METHOD(Test_Declare, testDeclare11);
PHP_METHOD(Test_Declare, testDeclare12);
PHP_METHOD(Test_Declare, testDeclare13);
PHP_METHOD(Test_Declare, testDeclare14);
PHP_METHOD(Test_Declare, testDeclare15);

ZEPHIR_INIT_FUNCS(test_declare_method_entry) {
	PHP_ME(Test_Declare, testDeclare1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare15, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
