
extern zend_class_entry *test_12__closure_ce;

ZEPHIR_INIT_CLASS(test_12__closure);

PHP_METHOD(test_12__closure, __invoke);

ZEPHIR_INIT_FUNCS(test_12__closure_method_entry) {
	PHP_ME(test_12__closure, __invoke, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};
