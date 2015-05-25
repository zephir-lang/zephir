
extern zend_class_entry *test_references_ce;

ZEPHIR_INIT_CLASS(Test_References);

PHP_METHOD(Test_References, assignByRef);

ZEPHIR_INIT_FUNCS(test_references_method_entry) {
	PHP_ME(Test_References, assignByRef, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
