
extern zend_class_entry *test_typeinstances_ce;

ZEPHIR_INIT_CLASS(Test_TypeInstances);

PHP_METHOD(Test_TypeInstances, testInstanceOfString1);
PHP_METHOD(Test_TypeInstances, testInstanceOfString2);
PHP_METHOD(Test_TypeInstances, testInstanceOfString3);

ZEPHIR_INIT_FUNCS(test_typeinstances_method_entry) {
	PHP_ME(Test_TypeInstances, testInstanceOfString1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_TypeInstances, testInstanceOfString2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_TypeInstances, testInstanceOfString3, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
