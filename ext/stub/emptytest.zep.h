
extern zend_class_entry *stub_emptytest_ce;

ZEPHIR_INIT_CLASS(Stub_EmptyTest);

PHP_METHOD(Stub_EmptyTest, testDynamicVarArrayEmpty);
PHP_METHOD(Stub_EmptyTest, testDynamicVarArrayNotEmpty);
PHP_METHOD(Stub_EmptyTest, testEmptyString);
PHP_METHOD(Stub_EmptyTest, testNotEmptyString);
PHP_METHOD(Stub_EmptyTest, testString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_emptytest_teststring, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, a, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, a)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_emptytest_method_entry) {
	PHP_ME(Stub_EmptyTest, testDynamicVarArrayEmpty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_EmptyTest, testDynamicVarArrayNotEmpty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_EmptyTest, testEmptyString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_EmptyTest, testNotEmptyString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_EmptyTest, testString, arginfo_stub_emptytest_teststring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
