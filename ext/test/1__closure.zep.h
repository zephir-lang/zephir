
extern zend_class_entry *test_1__closure_ce;

ZEPHIR_INIT_CLASS(test_1__closure);

PHP_METHOD(test_1__closure, __invoke);

ZEPHIR_INIT_FUNCS(test_1__closure_method_entry) {
	PHP_ME(test_1__closure, __invoke, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_FINAL)
	PHP_FE_END
};
