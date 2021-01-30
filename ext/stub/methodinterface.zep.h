
extern zend_class_entry *stub_methodinterface_ce;

ZEPHIR_INIT_CLASS(Stub_MethodInterface);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodinterface_testmethod, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_methodinterface_method_entry) {
	PHP_ABSTRACT_ME(Stub_MethodInterface, testMethod, arginfo_stub_methodinterface_testmethod)
	PHP_FE_END
};
