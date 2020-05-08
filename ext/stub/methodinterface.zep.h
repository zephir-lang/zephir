
extern zend_class_entry *stub_methodinterface_ce;

ZEPHIR_INIT_CLASS(Stub_MethodInterface);

ZEPHIR_INIT_FUNCS(stub_methodinterface_method_entry) {
	PHP_ABSTRACT_ME(Stub_MethodInterface, testMethod, NULL)
	PHP_FE_END
};
