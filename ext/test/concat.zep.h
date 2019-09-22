
extern zend_class_entry *test_concat_ce;

ZEPHIR_INIT_CLASS(Test_Concat);

PHP_METHOD(Test_Concat, getTestProperty);
PHP_METHOD(Test_Concat, testConcatBySelfProperty);
PHP_METHOD(Test_Concat, testConcat1);
PHP_METHOD(Test_Concat, testConcat2);
PHP_METHOD(Test_Concat, testConcat3);
PHP_METHOD(Test_Concat, testConcat4);
PHP_METHOD(Test_Concat, testConcat5);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_gettestproperty, 0, 0, IS_STRING, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_gettestproperty, 0, 0, IS_STRING, NULL, 1)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcatbyselfproperty, 0, 1, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcatbyselfproperty, 0, 1, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_test_concat_testconcatbyselfproperty, 0, 0, 1)
#define arginfo_test_concat_testconcatbyselfproperty NULL
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, title)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat1, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat1, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat2, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat2, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat3, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat3, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat4, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat4, 0, 1, IS_STRING, NULL, 0)
#endif
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat5, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_concat_testconcat5, 0, 1, IS_STRING, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, number, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, number)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_concat_method_entry) {
	PHP_ME(Test_Concat, getTestProperty, arginfo_test_concat_gettestproperty, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcatBySelfProperty, arginfo_test_concat_testconcatbyselfproperty, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat1, arginfo_test_concat_testconcat1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat2, arginfo_test_concat_testconcat2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat3, arginfo_test_concat_testconcat3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat4, arginfo_test_concat_testconcat4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Concat, testConcat5, arginfo_test_concat_testconcat5, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
