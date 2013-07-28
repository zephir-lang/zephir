
extern zend_class_entry *test_oo_ce;

ZEPHIR_INIT_CLASS(Test_Oo);

PHP_METHOD(Test_Oo, testInstance1);
PHP_METHOD(Test_Oo, testInstance2);

ZEPHIR_INIT_FUNCS(test_oo_method_entry) {
	PHP_ME(Test_Oo, testInstance1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo, testInstance2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
