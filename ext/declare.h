
extern zend_class_entry *test_declare_ce;

ZEPHIR_INIT_CLASS(Test_Declare);

PHP_METHOD(Test_Declare, testDeclare1);
PHP_METHOD(Test_Declare, testDeclare2);
PHP_METHOD(Test_Declare, testDeclare3);
PHP_METHOD(Test_Declare, testDeclare4);
PHP_METHOD(Test_Declare, testDeclare5);
PHP_METHOD(Test_Declare, testDeclare6);

ZEPHIR_INIT_FUNCS(test_declare_method_entry) {
	PHP_ME(Test_Declare, testDeclare1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Declare, testDeclare6, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
