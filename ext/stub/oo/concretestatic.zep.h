
extern zend_class_entry *stub_oo_concretestatic_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_ConcreteStatic);

PHP_METHOD(Stub_Oo_ConcreteStatic, parentFunction);
PHP_METHOD(Stub_Oo_ConcreteStatic, childFunction);

ZEPHIR_INIT_FUNCS(stub_oo_concretestatic_method_entry) {
	PHP_ME(Stub_Oo_ConcreteStatic, parentFunction, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_ConcreteStatic, childFunction, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
