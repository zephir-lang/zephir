
extern zend_class_entry *test_oo_extendpdoclass_ce;

ZEPHIR_INIT_CLASS(Test_Oo_ExtendPdoClass);

PHP_METHOD(Test_Oo_ExtendPdoClass, __construct);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_oo_extendpdoclass___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, dsn)
	ZEND_ARG_INFO(0, username)
	ZEND_ARG_INFO(0, password)
	ZEND_ARG_INFO(0, attrs)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_oo_extendpdoclass_method_entry) {
	PHP_ME(Test_Oo_ExtendPdoClass, __construct, arginfo_test_oo_extendpdoclass___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_FE_END
};
