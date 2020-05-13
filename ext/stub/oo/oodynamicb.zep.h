
extern zend_class_entry *stub_oo_oodynamicb_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoDynamicB);

PHP_METHOD(Stub_Oo_OoDynamicB, execute);
PHP_METHOD(Stub_Oo_OoDynamicB, execute2);

ZEPHIR_INIT_FUNCS(stub_oo_oodynamicb_method_entry) {
	PHP_ME(Stub_Oo_OoDynamicB, execute, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDynamicB, execute2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
