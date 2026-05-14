
extern zend_class_entry *stub_oo_deprecatedmethods_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_DeprecatedMethods);

PHP_METHOD(Stub_Oo_DeprecatedMethods, publicDeprecated);
PHP_METHOD(Stub_Oo_DeprecatedMethods, normalMethod);
PHP_METHOD(Stub_Oo_DeprecatedMethods, privateDeprecated);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_deprecatedmethods_publicdeprecated, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_deprecatedmethods_normalmethod, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_deprecatedmethods_privatedeprecated, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_deprecatedmethods_method_entry) {
PHP_ME(Stub_Oo_DeprecatedMethods, publicDeprecated, arginfo_stub_oo_deprecatedmethods_publicdeprecated, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED)
PHP_ME(Stub_Oo_DeprecatedMethods, normalMethod, arginfo_stub_oo_deprecatedmethods_normalmethod, ZEND_ACC_PUBLIC)
PHP_ME(Stub_Oo_DeprecatedMethods, privateDeprecated, arginfo_stub_oo_deprecatedmethods_privatedeprecated, ZEND_ACC_PRIVATE|ZEND_ACC_DEPRECATED)
	PHP_FE_END
};
