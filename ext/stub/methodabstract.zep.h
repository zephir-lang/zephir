
extern zend_class_entry *stub_methodabstract_ce;

ZEPHIR_INIT_CLASS(Stub_MethodAbstract);

PHP_METHOD(Stub_MethodAbstract, testInterfaceMetho);

ZEPHIR_INIT_FUNCS(stub_methodabstract_method_entry) {
	PHP_ME(Stub_MethodAbstract, testInterfaceMetho, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
