
extern zend_class_entry *test_trytest_ce;

ZEPHIR_INIT_CLASS(Test_TryTest);

PHP_METHOD(Test_TryTest, testTry1);
PHP_METHOD(Test_TryTest, testTry2);
PHP_METHOD(Test_TryTest, testTry3);
PHP_METHOD(Test_TryTest, testTry4);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_trytest_testtry4, 0, 0, 1)
	ZEND_ARG_INFO(0, a)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_trytest_method_entry) {
	PHP_ME(Test_TryTest, testTry1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_TryTest, testTry2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_TryTest, testTry3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_TryTest, testTry4, arginfo_test_trytest_testtry4, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
