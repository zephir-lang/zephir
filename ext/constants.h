
extern zend_class_entry *test_constants_ce;

ZEPHIR_INIT_CLASS(Test_Constants);

PHP_METHOD(Test_Constants, testReadConstant);
PHP_METHOD(Test_Constants, testReadClassConstant);

ZEPHIR_INIT_FUNCS(test_constants_method_entry) {
	PHP_ME(Test_Constants, testReadConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Constants, testReadClassConstant, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
