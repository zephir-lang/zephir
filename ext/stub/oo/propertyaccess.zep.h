
extern zend_class_entry *stub_oo_propertyaccess_ce;

ZEPHIR_INIT_CLASS(Stub_Oo_PropertyAccess);

PHP_METHOD(Stub_Oo_PropertyAccess, __construct);
PHP_METHOD(Stub_Oo_PropertyAccess, setPrivatevariable);
PHP_METHOD(Stub_Oo_PropertyAccess, getPrivatevariable);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_oo_propertyaccess_setprivatevariable, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_oo_propertyaccess_method_entry) {
	PHP_ME(Stub_Oo_PropertyAccess, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Stub_Oo_PropertyAccess, setPrivatevariable, arginfo_stub_oo_propertyaccess_setprivatevariable, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Oo_PropertyAccess, getPrivatevariable, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
