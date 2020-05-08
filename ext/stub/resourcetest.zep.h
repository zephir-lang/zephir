
extern zend_class_entry *stub_resourcetest_ce;

ZEPHIR_INIT_CLASS(Stub_ResourceTest);

PHP_METHOD(Stub_ResourceTest, testLetStatementSTDIN);
PHP_METHOD(Stub_ResourceTest, testLetStatementSTDOUT);
PHP_METHOD(Stub_ResourceTest, testLetStatementSTDERR);
PHP_METHOD(Stub_ResourceTest, testTypeOffResource);
PHP_METHOD(Stub_ResourceTest, testIsResource);
PHP_METHOD(Stub_ResourceTest, testFunctionsForSTDIN);

ZEPHIR_INIT_FUNCS(stub_resourcetest_method_entry) {
	PHP_ME(Stub_ResourceTest, testLetStatementSTDIN, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ResourceTest, testLetStatementSTDOUT, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ResourceTest, testLetStatementSTDERR, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ResourceTest, testTypeOffResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ResourceTest, testIsResource, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_ResourceTest, testFunctionsForSTDIN, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
