
extern zend_class_entry *test_fetchtest_ce;

ZEPHIR_INIT_CLASS(Test_FetchTest);

PHP_METHOD(Test_FetchTest, testFetchArray1);
PHP_METHOD(Test_FetchTest, testFetchArray2);
PHP_METHOD(Test_FetchTest, testFetchArray3);
PHP_METHOD(Test_FetchTest, testFetchObject1);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_testfetcharray1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_testfetcharray2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_testfetcharray3, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_testfetchobject1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_fetchtest_method_entry) {
	PHP_ME(Test_FetchTest, testFetchArray1, arginfo_test_fetchtest_testfetcharray1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchArray2, arginfo_test_fetchtest_testfetcharray2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchArray3, arginfo_test_fetchtest_testfetcharray3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchObject1, arginfo_test_fetchtest_testfetchobject1, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
