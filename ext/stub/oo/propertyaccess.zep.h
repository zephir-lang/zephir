
extern zend_class_entry *stub_oo_propertyaccess_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_PropertyAccess);

PHP_METHOD(Stub_Oo_PropertyAccess, __construct);
PHP_METHOD(Stub_Oo_PropertyAccess, setPrivatevariable);
PHP_METHOD(Stub_Oo_PropertyAccess, getPrivatevariable);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_stub_oo_propertyaccess_setprivatevariable, 0, 1, Stub\\Oo\\PropertyAccess, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_oo_propertyaccess_setprivatevariable, 0, 1, IS_OBJECT, "Stub\\Oo\\PropertyAccess", 0)
#endif
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_propertyaccess_method_entry) {
	PHP_ME(Stub_Oo_PropertyAccess, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Oo_PropertyAccess, setPrivatevariable, arginfo_stub_oo_propertyaccess_setprivatevariable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_PropertyAccess, getPrivatevariable, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
