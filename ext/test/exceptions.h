
extern zend_class_entry *test_exceptions_ce;

ZEPHIR_INIT_CLASS(Test_Exceptions);

PHP_METHOD(Test_Exceptions, testException1);
PHP_METHOD(Test_Exceptions, testException2);
PHP_METHOD(Test_Exceptions, testException3);
PHP_METHOD(Test_Exceptions, getException);
PHP_METHOD(Test_Exceptions, testException4);

ZEPHIR_INIT_FUNCS(test_exceptions_method_entry) {
	PHP_ME(Test_Exceptions, testException1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testException2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, testException3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Exceptions, getException, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Test_Exceptions, testException4, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
