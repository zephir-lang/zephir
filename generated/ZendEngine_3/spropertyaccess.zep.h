
extern zend_class_entry *test_spropertyaccess_ce;

ZEPHIR_INIT_CLASS(Test_SPropertyAccess);

PHP_METHOD(Test_SPropertyAccess, __construct);
PHP_METHOD(Test_SPropertyAccess, testArgumentWithUnderscore);
PHP_METHOD(Test_SPropertyAccess, testArgument);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spropertyaccess_testargumentwithunderscore, 0, 0, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_spropertyaccess_testargument, 0, 0, 0)
	ZEND_ARG_INFO(0, delimiter)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_spropertyaccess_method_entry) {
	PHP_ME(Test_SPropertyAccess, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_SPropertyAccess, testArgumentWithUnderscore, arginfo_test_spropertyaccess_testargumentwithunderscore, ZEND_ACC_PUBLIC)
	PHP_ME(Test_SPropertyAccess, testArgument, arginfo_test_spropertyaccess_testargument, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
