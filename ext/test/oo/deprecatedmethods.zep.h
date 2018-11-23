
extern zend_class_entry *test_oo_deprecatedmethods_ce;

ZEPHIR_INIT_CLASS(Test_Oo_DeprecatedMethods);

PHP_METHOD(Test_Oo_DeprecatedMethods, publicDeprecated);
PHP_METHOD(Test_Oo_DeprecatedMethods, normalMethod);
PHP_METHOD(Test_Oo_DeprecatedMethods, privateDeprecated);

ZEPHIR_INIT_FUNCS(test_oo_deprecatedmethods_method_entry) {
	PHP_ME(Test_Oo_DeprecatedMethods, publicDeprecated, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_DEPRECATED)
	PHP_ME(Test_Oo_DeprecatedMethods, normalMethod, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_DeprecatedMethods, privateDeprecated, NULL, ZEND_ACC_PRIVATE|ZEND_ACC_DEPRECATED)
	PHP_FE_END
};
