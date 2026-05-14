
extern zend_class_entry *stub_interfaces_interfacestaticmethod_ce;

ZEPHIR_INIT_CLASS(Stub_Interfaces_InterfaceStaticMethod);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_interfaces_interfacestaticmethod_reset, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_interfaces_interfacestaticmethod_method_entry) {
ZEND_FENTRY(reset, NULL, arginfo_stub_interfaces_interfacestaticmethod_reset, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_FE_END
};
