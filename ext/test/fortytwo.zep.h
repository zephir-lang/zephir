
extern zend_class_entry *test_fortytwo_ce;

ZEPHIR_INIT_CLASS(Test_FortyTwo);

PHP_METHOD(Test_FortyTwo, proof);

ZEPHIR_INIT_FUNCS(test_fortytwo_method_entry) {
	PHP_ME(Test_FortyTwo, proof, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
