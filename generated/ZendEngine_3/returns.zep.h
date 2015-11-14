
extern zend_class_entry *test_returns_ce;

ZEPHIR_INIT_CLASS(Test_Returns);

PHP_METHOD(Test_Returns, testReturnCast1);
PHP_METHOD(Test_Returns, testReturnCast2);
PHP_METHOD(Test_Returns, testReturnCast3);
PHP_METHOD(Test_Returns, testReturnCast4);

ZEPHIR_INIT_FUNCS(test_returns_method_entry) {
	PHP_ME(Test_Returns, testReturnCast1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Returns, testReturnCast2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Returns, testReturnCast3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Returns, testReturnCast4, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
