
extern zend_class_entry *test_returns_ce;

ZEPHIR_INIT_CLASS(Test_Returns);

PHP_METHOD(Test_Returns, testReturnCast1);
PHP_METHOD(Test_Returns, testReturnCast2);
PHP_METHOD(Test_Returns, testReturnCast3);
PHP_METHOD(Test_Returns, testReturnCast4);
PHP_METHOD(Test_Returns, returnWithParameter);
PHP_METHOD(Test_Returns, returnWithoutParameter);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast1, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast1, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast2, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast2, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast3, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast3, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast4, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_returns_testreturncast4, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_returns_returnwithparameter, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, parameter, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_returns_method_entry) {
	PHP_ME(Test_Returns, testReturnCast1, arginfo_test_returns_testreturncast1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Returns, testReturnCast2, arginfo_test_returns_testreturncast2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Returns, testReturnCast3, arginfo_test_returns_testreturncast3, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Returns, testReturnCast4, arginfo_test_returns_testreturncast4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Returns, returnWithParameter, arginfo_test_returns_returnwithparameter, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_ME(Test_Returns, returnWithoutParameter, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
