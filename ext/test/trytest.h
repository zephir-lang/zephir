
extern zend_class_entry *test_trytest_ce;

ZEPHIR_INIT_CLASS(Test_TryTest);

PHP_METHOD(Test_TryTest, testTry1);

ZEPHIR_INIT_FUNCS(test_trytest_method_entry) {
	PHP_ME(Test_TryTest, testTry1, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
