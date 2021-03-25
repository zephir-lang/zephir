
extern zend_class_entry *stub_interfaces_interfaceintsignature_ce;

ZEPHIR_INIT_CLASS(Stub_Interfaces_InterfaceIntSignature);

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_interfaces_interfaceintsignature_get, 0, 1, IS_LONG, 0)
	ZEND_ARG_OBJ_INFO(0, obj, Stub\\Interfaces\\InterfaceInt, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_interfaces_interfaceintsignature_method_entry) {
	PHP_ABSTRACT_ME(Stub_Interfaces_InterfaceIntSignature, get, arginfo_stub_interfaces_interfaceintsignature_get)
	PHP_FE_END
};
