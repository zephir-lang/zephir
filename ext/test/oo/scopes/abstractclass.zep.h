
extern zend_class_entry *test_oo_scopes_abstractclass_ce;

ZEPHIR_INIT_CLASS(Test_Oo_Scopes_AbstractClass);

PHP_METHOD(Test_Oo_Scopes_AbstractClass, setProperty);
PHP_METHOD(Test_Oo_Scopes_AbstractClass, getPrivateProperty2);
PHP_METHOD(Test_Oo_Scopes_AbstractClass, getProtectedProperty2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_scopes_abstractclass_setproperty, 0, 0, 2)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, name)
#endif
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_scopes_abstractclass_method_entry) {
	PHP_ME(Test_Oo_Scopes_AbstractClass, setProperty, arginfo_test_oo_scopes_abstractclass_setproperty, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_Scopes_AbstractClass, getPrivateProperty2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_Scopes_AbstractClass, getProtectedProperty2, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
