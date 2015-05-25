
extern zend_class_entry *test_internalinterfaces_ce;

ZEPHIR_INIT_CLASS(Test_InternalInterfaces);

PHP_METHOD(Test_InternalInterfaces, count);

ZEPHIR_INIT_FUNCS(test_internalinterfaces_method_entry) {
	PHP_ME(Test_InternalInterfaces, count, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
