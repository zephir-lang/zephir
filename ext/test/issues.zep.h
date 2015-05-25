
extern zend_class_entry *test_issues_ce;

ZEPHIR_INIT_CLASS(Test_Issues);

PHP_METHOD(Test_Issues, setAdapter);
PHP_METHOD(Test_Issues, someMethod);
PHP_METHOD(Test_Issues, test);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issues_setadapter, 0, 0, 1)
	ZEND_ARG_INFO(0, adapter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_issues_somemethod, 0, 0, 1)
	ZEND_ARG_INFO(0, methodName)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_issues_method_entry) {
	PHP_ME(Test_Issues, setAdapter, arginfo_test_issues_setadapter, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Issues, someMethod, arginfo_test_issues_somemethod, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Issues, test, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
