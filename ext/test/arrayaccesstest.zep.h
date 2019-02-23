
extern zend_class_entry *test_arrayaccesstest_ce;

ZEPHIR_INIT_CLASS(Test_ArrayAccessTest);

PHP_METHOD(Test_ArrayAccessTest, exits);
PHP_METHOD(Test_ArrayAccessTest, get);

ZEPHIR_INIT_FUNCS(test_arrayaccesstest_method_entry) {
	PHP_ME(Test_ArrayAccessTest, exits, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_ArrayAccessTest, get, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
