
extern zend_class_entry *stub_oo_deprecatedmethods_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_DeprecatedMethods);

PHP_METHOD(Stub_Oo_DeprecatedMethods, publicDeprecated);
PHP_METHOD(Stub_Oo_DeprecatedMethods, normalMethod);
PHP_METHOD(Stub_Oo_DeprecatedMethods, privateDeprecated);

ZEPHIR_INIT_FUNCS(stub_oo_deprecatedmethods_method_entry) {
	PHP_ME(Stub_Oo_DeprecatedMethods, publicDeprecated, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED)
	PHP_ME(Stub_Oo_DeprecatedMethods, normalMethod, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_DeprecatedMethods, privateDeprecated, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DEPRECATED)
	PHP_FE_END
};
