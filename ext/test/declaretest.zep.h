
extern zend_class_entry *test_declaretest_ce;

ZEPHIR_INIT_CLASS(Test_DeclareTest);

PHP_METHOD(Test_DeclareTest, testStringDeclare1);
PHP_METHOD(Test_DeclareTest, testStringDeclare2);
PHP_METHOD(Test_DeclareTest, testDeclare1);
PHP_METHOD(Test_DeclareTest, testDeclare2);
PHP_METHOD(Test_DeclareTest, testDeclare3);
PHP_METHOD(Test_DeclareTest, testDeclare4);
PHP_METHOD(Test_DeclareTest, testDeclare5);
PHP_METHOD(Test_DeclareTest, testDeclare6);
PHP_METHOD(Test_DeclareTest, testDeclare7);
PHP_METHOD(Test_DeclareTest, testDeclare8);
PHP_METHOD(Test_DeclareTest, testDeclare9);
PHP_METHOD(Test_DeclareTest, testDeclare10);
PHP_METHOD(Test_DeclareTest, testDeclare11);
PHP_METHOD(Test_DeclareTest, testDeclare12);
PHP_METHOD(Test_DeclareTest, testDeclare13);
PHP_METHOD(Test_DeclareTest, testDeclare14);
PHP_METHOD(Test_DeclareTest, testDeclare15);

ZEPHIR_INIT_FUNCS(test_declaretest_method_entry) {
	PHP_ME(Test_DeclareTest, testStringDeclare1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testStringDeclare2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare6, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare10, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare11, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare12, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare14, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_DeclareTest, testDeclare15, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
