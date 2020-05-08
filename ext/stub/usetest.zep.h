
extern zend_class_entry *stub_usetest_ce;

ZEPHIR_INIT_CLASS(Stub_UseTest);

PHP_METHOD(Stub_UseTest, createInstance);
PHP_METHOD(Stub_UseTest, count);
PHP_METHOD(Stub_UseTest, testUseClass1);
PHP_METHOD(Stub_UseTest, testUseClass2);
PHP_METHOD(Stub_UseTest, testUseNamespaceAlias);

ZEPHIR_INIT_FUNCS(stub_usetest_method_entry) {
	PHP_ME(Stub_UseTest, createInstance, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, count, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, testUseClass1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, testUseClass2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, testUseNamespaceAlias, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
