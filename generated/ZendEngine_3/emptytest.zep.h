
extern zend_class_entry *test_emptytest_ce;

ZEPHIR_INIT_CLASS(Test_EmptyTest);

PHP_METHOD(Test_EmptyTest, testDynamicVarArrayEmpty);
PHP_METHOD(Test_EmptyTest, testDynamicVarArrayNotEmpty);
PHP_METHOD(Test_EmptyTest, testEmptyString);
PHP_METHOD(Test_EmptyTest, testNotEmptyString);
PHP_METHOD(Test_EmptyTest, testString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_emptytest_teststring, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_emptytest_method_entry) {
	PHP_ME(Test_EmptyTest, testDynamicVarArrayEmpty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_EmptyTest, testDynamicVarArrayNotEmpty, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_EmptyTest, testEmptyString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_EmptyTest, testNotEmptyString, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_EmptyTest, testString, arginfo_test_emptytest_teststring, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
