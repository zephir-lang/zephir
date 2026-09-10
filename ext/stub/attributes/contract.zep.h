
extern zend_class_entry *stub_attributes_contract_ce;

ZEPHIR_INIT_CLASS(Stub_Attributes_Contract);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_attributes_contract_handle, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, secret, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_attributes_contract_method_entry) {
	PHP_ABSTRACT_ME(Stub_Attributes_Contract, handle, arginfo_stub_attributes_contract_handle)
	PHP_FE_END
};
