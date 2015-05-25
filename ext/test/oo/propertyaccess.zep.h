
extern zend_class_entry *test_oo_propertyaccess_ce;

ZEPHIR_INIT_CLASS(Test_Oo_PropertyAccess);

PHP_METHOD(Test_Oo_PropertyAccess, __construct);

ZEPHIR_INIT_FUNCS(test_oo_propertyaccess_method_entry) {
	PHP_ME(Test_Oo_PropertyAccess, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
