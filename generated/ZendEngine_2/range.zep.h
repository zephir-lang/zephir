
extern zend_class_entry *test_range_ce;

ZEPHIR_INIT_CLASS(Test_Range);

PHP_METHOD(Test_Range, inclusive1);
PHP_METHOD(Test_Range, exclusive1);

ZEPHIR_INIT_FUNCS(test_range_method_entry) {
	PHP_ME(Test_Range, inclusive1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Range, exclusive1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
