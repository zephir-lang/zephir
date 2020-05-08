
extern zend_class_entry *stub_typeinstances_ce;

ZEPHIR_INIT_CLASS(Stub_TypeInstances);

PHP_METHOD(Stub_TypeInstances, testInstanceOfString1);
PHP_METHOD(Stub_TypeInstances, testInstanceOfString2);
PHP_METHOD(Stub_TypeInstances, testInstanceOfString3);

ZEPHIR_INIT_FUNCS(stub_typeinstances_method_entry) {
	PHP_ME(Stub_TypeInstances, testInstanceOfString1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeInstances, testInstanceOfString2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_TypeInstances, testInstanceOfString3, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
