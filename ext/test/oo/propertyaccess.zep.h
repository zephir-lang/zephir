
extern zend_class_entry *test_oo_propertyaccess_ce;

ZEPHIR_INIT_CLASS(Test_Oo_PropertyAccess);

PHP_METHOD(Test_Oo_PropertyAccess, __construct);
PHP_METHOD(Test_Oo_PropertyAccess, setPrivatevariable);
PHP_METHOD(Test_Oo_PropertyAccess, getPrivatevariable);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_propertyaccess_setprivatevariable, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_propertyaccess_method_entry) {
	PHP_ME(Test_Oo_PropertyAccess, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_Oo_PropertyAccess, setPrivatevariable, arginfo_test_oo_propertyaccess_setprivatevariable, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Oo_PropertyAccess, getPrivatevariable, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
