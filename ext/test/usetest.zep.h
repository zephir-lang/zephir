
extern zend_class_entry *test_usetest_ce;

ZEPHIR_INIT_CLASS(Test_UseTest);

PHP_METHOD(Test_UseTest, createInstance);
PHP_METHOD(Test_UseTest, count);
PHP_METHOD(Test_UseTest, testUseClass1);
PHP_METHOD(Test_UseTest, testUseClass2);
PHP_METHOD(Test_UseTest, testUseNamespaceAlias);

ZEPHIR_INIT_FUNCS(test_usetest_method_entry) {
	PHP_ME(Test_UseTest, createInstance, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_UseTest, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_UseTest, testUseClass1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_UseTest, testUseClass2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_UseTest, testUseNamespaceAlias, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
