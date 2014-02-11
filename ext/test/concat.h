
extern zend_class_entry *test_concat_ce;

ZEPHIR_INIT_CLASS(Test_Concat);

PHP_METHOD(Test_Concat, testConcat1);
PHP_METHOD(Test_Concat, testConcat10);

ZEPHIR_INIT_FUNCS(test_concat_method_entry) {
	PHP_ME(Test_Concat, testConcat1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat10, NULL, ZEND_ACC_PUBLIC)
  PHP_FE_END
};
