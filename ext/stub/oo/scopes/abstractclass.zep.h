
extern zend_class_entry *stub_oo_scopes_abstractclass_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_Scopes_AbstractClass);

PHP_METHOD(Stub_Oo_Scopes_AbstractClass, setProperty);
PHP_METHOD(Stub_Oo_Scopes_AbstractClass, getPrivateProperty2);
PHP_METHOD(Stub_Oo_Scopes_AbstractClass, getProtectedProperty2);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_scopes_abstractclass_setproperty, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_scopes_abstractclass_getprivateproperty2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_scopes_abstractclass_getprotectedproperty2, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_scopes_abstractclass_method_entry) {
	PHP_ME(Stub_Oo_Scopes_AbstractClass, setProperty, arginfo_stub_oo_scopes_abstractclass_setproperty, ZEND_ACC_PUBLIC)
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_Scopes_AbstractClass, getPrivateProperty2, arginfo_stub_oo_scopes_abstractclass_getprivateproperty2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_Scopes_AbstractClass, getPrivateProperty2, NULL, ZEND_ACC_PUBLIC)
#endif
#if PHP_VERSION_ID >= 80000
	PHP_ME(Stub_Oo_Scopes_AbstractClass, getProtectedProperty2, arginfo_stub_oo_scopes_abstractclass_getprotectedproperty2, ZEND_ACC_PUBLIC)
#else
	PHP_ME(Stub_Oo_Scopes_AbstractClass, getProtectedProperty2, NULL, ZEND_ACC_PUBLIC)
#endif
	PHP_FE_END
};
