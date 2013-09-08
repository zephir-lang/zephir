
extern zend_class_entry *test_cast_ce;

ZEPHIR_INIT_CLASS(Test_Cast);

PHP_METHOD(Test_Cast, testCast1);
PHP_METHOD(Test_Cast, testCast2);

ZEPHIR_INIT_FUNCS(test_cast_method_entry) {
	PHP_ME(Test_Cast, testCast1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Cast, testCast2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
