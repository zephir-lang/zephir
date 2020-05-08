
extern zend_class_entry *stub_oo_oodynamica_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_OoDynamicA);

PHP_METHOD(Stub_Oo_OoDynamicA, getNew);
PHP_METHOD(Stub_Oo_OoDynamicA, call2);
PHP_METHOD(Stub_Oo_OoDynamicA, call1);
PHP_METHOD(Stub_Oo_OoDynamicA, execute);
PHP_METHOD(Stub_Oo_OoDynamicA, execute2);

ZEPHIR_INIT_FUNCS(stub_oo_oodynamica_method_entry) {
	PHP_ME(Stub_Oo_OoDynamicA, getNew, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_OoDynamicA, call2, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_OoDynamicA, call1, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Stub_Oo_OoDynamicA, execute, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_OoDynamicA, execute2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
