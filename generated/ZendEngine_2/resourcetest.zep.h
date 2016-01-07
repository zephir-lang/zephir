
extern zend_class_entry *test_resourcetest_ce;

ZEPHIR_INIT_CLASS(Test_ResourceTest);

PHP_METHOD(Test_ResourceTest, testLetStatementSTDIN);
PHP_METHOD(Test_ResourceTest, testLetStatementSTDOUT);
PHP_METHOD(Test_ResourceTest, testLetStatementSTDERR);
PHP_METHOD(Test_ResourceTest, testTypeOffResource);
PHP_METHOD(Test_ResourceTest, testIsResource);
PHP_METHOD(Test_ResourceTest, testFunctionsForSTDIN);

ZEPHIR_INIT_FUNCS(test_resourcetest_method_entry) {
	PHP_ME(Test_ResourceTest, testLetStatementSTDIN, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ResourceTest, testLetStatementSTDOUT, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ResourceTest, testLetStatementSTDERR, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ResourceTest, testTypeOffResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ResourceTest, testIsResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ResourceTest, testFunctionsForSTDIN, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
