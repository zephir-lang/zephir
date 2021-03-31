
extern zend_class_entry *stub_interfaces_interfacestaticmethod_ce;

ZEPHIR_INIT_CLASS(Stub_Interfaces_InterfaceStaticMethod);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_interfaces_interfacestaticmethod_reset, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_interfaces_interfacestaticmethod_method_entry) {
#if PHP_VERSION_ID >= 80000
	ZEND_FENTRY(reset, NULL, arginfo_stub_interfaces_interfacestaticmethod_reset, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
#else
	ZEND_FENTRY(reset, NULL, NULL, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
