
extern zend_class_entry *test_issue1521_ce;

ZEPHIR_INIT_CLASS(Test_Issue1521);

PHP_METHOD(Test_Issue1521, test);

ZEPHIR_INIT_FUNCS(test_issue1521_method_entry) {
	PHP_ME(Test_Issue1521, test, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
