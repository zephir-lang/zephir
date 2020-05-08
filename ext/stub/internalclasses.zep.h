
extern zend_class_entry *stub_internalclasses_ce;

ZEPHIR_INIT_CLASS(Stub_InternalClasses);

PHP_METHOD(Stub_InternalClasses, testStaticCall);
PHP_METHOD(Stub_InternalClasses, testStaticPropertyFetch);

ZEPHIR_INIT_FUNCS(stub_internalclasses_method_entry) {
	PHP_ME(Stub_InternalClasses, testStaticCall, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_InternalClasses, testStaticPropertyFetch, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
