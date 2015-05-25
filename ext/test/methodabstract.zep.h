
extern zend_class_entry *test_methodabstract_ce;

ZEPHIR_INIT_CLASS(Test_MethodAbstract);

PHP_METHOD(Test_MethodAbstract, testInterfaceMetho);

ZEPHIR_INIT_FUNCS(test_methodabstract_method_entry) {
	PHP_ME(Test_MethodAbstract, testInterfaceMetho, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
