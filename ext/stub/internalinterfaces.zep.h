
extern zend_class_entry *stub_internalinterfaces_ce;

ZEPHIR_INIT_CLASS(Stub_InternalInterfaces);

PHP_METHOD(Stub_InternalInterfaces, count);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_internalinterfaces_count, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_internalinterfaces_method_entry) {
	PHP_ME(Stub_InternalInterfaces, count, arginfo_stub_internalinterfaces_count, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
