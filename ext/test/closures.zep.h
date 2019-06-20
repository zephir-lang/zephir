
extern zend_class_entry *test_closures_ce;

ZEPHIR_INIT_CLASS(Test_Closures);

PHP_METHOD(Test_Closures, simple1);
PHP_METHOD(Test_Closures, simple2);
PHP_METHOD(Test_Closures, simple3);
PHP_METHOD(Test_Closures, simple4);
PHP_METHOD(Test_Closures, simple5);
PHP_METHOD(Test_Closures, arrow1);
PHP_METHOD(Test_Closures, arrow2);
PHP_METHOD(Test_Closures, testUseCommand);
PHP_METHOD(Test_Closures, issue1860);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_closures_issue1860, 0, 0, 1)
	ZEND_ARG_ARRAY_INFO(0, abc, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_closures_method_entry) {
	PHP_ME(Test_Closures, simple1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple4, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, simple5, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, arrow1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, arrow2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, testUseCommand, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Closures, issue1860, arginfo_test_closures_issue1860, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
