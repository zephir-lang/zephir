
extern zend_class_entry *test_scall_ce;

ZEPHIR_INIT_CLASS(Test_Scall);

PHP_METHOD(Test_Scall, testMethod1);
PHP_METHOD(Test_Scall, testCall1);

ZEPHIR_INIT_FUNCS(test_scall_method_entry) {
	PHP_ME(Test_Scall, testMethod1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
