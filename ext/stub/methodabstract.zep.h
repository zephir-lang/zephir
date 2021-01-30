
extern zend_class_entry *stub_methodabstract_ce;

ZEPHIR_INIT_CLASS(Stub_MethodAbstract);

PHP_METHOD(Stub_MethodAbstract, testInterfaceMetho);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_methodabstract_testinterfacemetho, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_methodabstract_method_entry) {
	PHP_ME(Stub_MethodAbstract, testInterfaceMetho, arginfo_stub_methodabstract_testinterfacemetho, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
