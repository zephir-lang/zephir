
extern zend_class_entry *stub_fetchtest_ce;

ZEPHIR_INIT_CLASS(Stub_FetchTest);

PHP_METHOD(Stub_FetchTest, setValues);
PHP_METHOD(Stub_FetchTest, getValues);
PHP_METHOD(Stub_FetchTest, testFetchArray1);
PHP_METHOD(Stub_FetchTest, testFetchArray2);
PHP_METHOD(Stub_FetchTest, testFetchArray3);
PHP_METHOD(Stub_FetchTest, testFetchArray4);
PHP_METHOD(Stub_FetchTest, testFetchArray5);
PHP_METHOD(Stub_FetchTest, testFetchArray6);
PHP_METHOD(Stub_FetchTest, testFetchObject1);
PHP_METHOD(Stub_FetchTest, testFetchObject2);
PHP_METHOD(Stub_FetchTest, testFetchPost);
PHP_METHOD(Stub_FetchTest, hasValue);
PHP_METHOD(Stub_FetchTest, getValue);

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_setvalues, 0, 0, 1)
	ZEND_ARG_INFO(0, values)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetcharray1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetcharray2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetcharray3, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetcharray4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_LONG, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetcharray5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetcharray6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, b, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, b)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetchobject1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetchobject2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_testfetchpost, 0, 0, 1)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fetchtest_hasvalue, 0, 1, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_fetchtest_hasvalue, 0, 1, _IS_BOOL, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, name)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_fetchtest_getvalue, 0, 0, 1)
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, name)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_fetchtest_method_entry) {
	PHP_ME(Stub_FetchTest, setValues, arginfo_stub_fetchtest_setvalues, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, getValues, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchArray1, arginfo_stub_fetchtest_testfetcharray1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchArray2, arginfo_stub_fetchtest_testfetcharray2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchArray3, arginfo_stub_fetchtest_testfetcharray3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchArray4, arginfo_stub_fetchtest_testfetcharray4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchArray5, arginfo_stub_fetchtest_testfetcharray5, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchArray6, arginfo_stub_fetchtest_testfetcharray6, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchObject1, arginfo_stub_fetchtest_testfetchobject1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchObject2, arginfo_stub_fetchtest_testfetchobject2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, testFetchPost, arginfo_stub_fetchtest_testfetchpost, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, hasValue, arginfo_stub_fetchtest_hasvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_FetchTest, getValue, arginfo_stub_fetchtest_getvalue, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
