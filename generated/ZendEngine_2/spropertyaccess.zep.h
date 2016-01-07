
extern zend_class_entry *test_spropertyaccess_ce;

ZEPHIR_INIT_CLASS(Test_SPropertyAccess);

PHP_METHOD(Test_SPropertyAccess, __construct);

ZEPHIR_INIT_FUNCS(test_spropertyaccess_method_entry) {
	PHP_ME(Test_SPropertyAccess, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
