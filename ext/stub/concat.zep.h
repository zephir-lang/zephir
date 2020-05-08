
extern zend_class_entry *stub_concat_ce;

ZEPHIR_INIT_CLASS(Stub_Concat);

PHP_METHOD(Stub_Concat, getTestProperty);
PHP_METHOD(Stub_Concat, testConcatBySelfProperty);
PHP_METHOD(Stub_Concat, testConcat1);
PHP_METHOD(Stub_Concat, testConcat2);
PHP_METHOD(Stub_Concat, testConcat3);
PHP_METHOD(Stub_Concat, testConcat4);
PHP_METHOD(Stub_Concat, testConcat5);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_gettestproperty, 0, 0, IS_STRING, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_gettestproperty, 0, 0, IS_STRING, NULL, 1)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70100
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatbyselfproperty, 0, 1, IS_VOID, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcatbyselfproperty, 0, 1, IS_VOID, NULL, 0)
#endif
#else
ZEND_BEGIN_ARG_INFO_EX(arginfo_stub_concat_testconcatbyselfproperty, 0, 0, 1)
#define arginfo_stub_concat_testconcatbyselfproperty NULL
#endif

#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, title, IS_STRING, 0)
#else
	ZEND_ARG_INFO(0, title)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat1, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat1, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat2, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat2, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat3, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat3, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat4, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat4, 0, 1, IS_STRING, NULL, 0)
#endif
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat5, 0, 1, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_stub_concat_testconcat5, 0, 1, IS_STRING, NULL, 0)
#endif
#if PHP_VERSION_ID >= 70200
	ZEND_ARG_TYPE_INFO(0, number, IS_DOUBLE, 0)
#else
	ZEND_ARG_INFO(0, number)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(stub_concat_method_entry) {
	PHP_ME(Stub_Concat, getTestProperty, arginfo_stub_concat_gettestproperty, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcatBySelfProperty, arginfo_stub_concat_testconcatbyselfproperty, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat1, arginfo_stub_concat_testconcat1, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat2, arginfo_stub_concat_testconcat2, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat3, arginfo_stub_concat_testconcat3, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat4, arginfo_stub_concat_testconcat4, ZEND_ACC_PUBLIC)
	PHP_ME(Stub_Concat, testConcat5, arginfo_stub_concat_testconcat5, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
