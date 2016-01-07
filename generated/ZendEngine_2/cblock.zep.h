
extern zend_class_entry *test_cblock_ce;

ZEPHIR_INIT_CLASS(Test_Cblock);

PHP_METHOD(Test_Cblock, testCblock1);
PHP_METHOD(Test_Cblock, testCblock2);

ZEPHIR_INIT_FUNCS(test_cblock_method_entry) {
	PHP_ME(Test_Cblock, testCblock1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cblock, testCblock2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
