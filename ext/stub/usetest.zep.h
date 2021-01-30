
extern zend_class_entry *stub_usetest_ce;

ZEPHIR_INIT_CLASS(Stub_UseTest);

PHP_METHOD(Stub_UseTest, createInstance);
PHP_METHOD(Stub_UseTest, count);
PHP_METHOD(Stub_UseTest, testUseClass1);
PHP_METHOD(Stub_UseTest, testUseClass2);
PHP_METHOD(Stub_UseTest, testUseNamespaceAlias);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_usetest_createinstance, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_usetest_count, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_usetest_testuseclass1, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_usetest_testuseclass2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_usetest_testusenamespacealias, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_usetest_method_entry) {
	PHP_ME(Stub_UseTest, createInstance, arginfo_stub_usetest_createinstance, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, count, arginfo_stub_usetest_count, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, testUseClass1, arginfo_stub_usetest_testuseclass1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, testUseClass2, arginfo_stub_usetest_testuseclass2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_UseTest, testUseNamespaceAlias, arginfo_stub_usetest_testusenamespacealias, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
