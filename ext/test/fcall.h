
extern zend_class_entry *test_fcall_ce;

ZEPHIR_INIT_CLASS(Test_Fcall);

PHP_METHOD(Test_Fcall, testCall1);
PHP_METHOD(Test_Fcall, testCall1FromVar);

ZEPHIR_INIT_FUNCS(test_fcall_method_entry) {
	PHP_ME(Test_Fcall, testCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Fcall, testCall1FromVar, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
