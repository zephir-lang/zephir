
extern zend_class_entry *test_fetchtest_ce;

ZEPHIR_INIT_CLASS(Test_FetchTest);

PHP_METHOD(Test_FetchTest, setValues);
PHP_METHOD(Test_FetchTest, getValues);
PHP_METHOD(Test_FetchTest, testFetchArray1);
PHP_METHOD(Test_FetchTest, testFetchArray2);
PHP_METHOD(Test_FetchTest, testFetchArray3);
PHP_METHOD(Test_FetchTest, testFetchObject1);
PHP_METHOD(Test_FetchTest, testFetchPost);
PHP_METHOD(Test_FetchTest, hasValue);
PHP_METHOD(Test_FetchTest, getValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_setvalues, 0, 0, 1)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

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

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_testfetchpost, 0, 0, 1)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_hasvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_fetchtest_getvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_fetchtest_method_entry) {
	PHP_ME(Test_FetchTest, setValues, arginfo_test_fetchtest_setvalues, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, getValues, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchArray1, arginfo_test_fetchtest_testfetcharray1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchArray2, arginfo_test_fetchtest_testfetcharray2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchArray3, arginfo_test_fetchtest_testfetcharray3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchObject1, arginfo_test_fetchtest_testfetchobject1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, testFetchPost, arginfo_test_fetchtest_testfetchpost, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, hasValue, arginfo_test_fetchtest_hasvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Test_FetchTest, getValue, arginfo_test_fetchtest_getvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
