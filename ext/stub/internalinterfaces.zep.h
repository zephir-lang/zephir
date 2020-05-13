
extern zend_class_entry *stub_internalinterfaces_ce;

ZEPHIR_INIT_CLASS(Stub_InternalInterfaces);

PHP_METHOD(Stub_InternalInterfaces, count);

ZEPHIR_INIT_FUNCS(stub_internalinterfaces_method_entry) {
	PHP_ME(Stub_InternalInterfaces, count, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
