
extern zend_class_entry *test_internalclasses_ce;

ZEPHIR_INIT_CLASS(Test_InternalClasses);

PHP_METHOD(Test_InternalClasses, testStaticCall);
PHP_METHOD(Test_InternalClasses, testStaticPropertyFetch);

ZEPHIR_INIT_FUNCS(test_internalclasses_method_entry) {
	PHP_ME(Test_InternalClasses, testStaticCall, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_InternalClasses, testStaticPropertyFetch, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
