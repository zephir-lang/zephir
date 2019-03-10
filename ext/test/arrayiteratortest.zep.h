
extern zend_class_entry *test_arrayiteratortest_ce;

ZEPHIR_INIT_CLASS(Test_ArrayIteratorTest);

PHP_METHOD(Test_ArrayIteratorTest, test);

ZEPHIR_INIT_FUNCS(test_arrayiteratortest_method_entry) {
	PHP_ME(Test_ArrayIteratorTest, test, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
