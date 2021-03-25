
extern zend_class_entry *stub_interfaces_interfaceint_ce;

ZEPHIR_INIT_CLASS(Stub_Interfaces_InterfaceInt);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_interfaces_interfaceint_set, 0, 1, IS_VOID, 0)

	ZEND_ARG_TYPE_INFO(0, val, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_interfaces_interfaceint_get, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_interfaces_interfaceint_method_entry) {
	PHP_ABSTRACT_ME(Stub_Interfaces_InterfaceInt, set, arginfo_stub_interfaces_interfaceint_set)
	PHP_ABSTRACT_ME(Stub_Interfaces_InterfaceInt, get, arginfo_stub_interfaces_interfaceint_get)
	PHP_FE_END
};
