
extern zend_class_entry *test_invoke_ce;

ZEPHIR_INIT_CLASS(Test_Invoke);

PHP_METHOD(Test_Invoke, __construct);
PHP_METHOD(Test_Invoke, __invoke);
PHP_METHOD(Test_Invoke, test);

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_invoke___invoke, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_invoke___invoke, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_invoke_test, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test_invoke_test, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_invoke_method_entry) {
	PHP_ME(Test_Invoke, __construct, NULL, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Test_Invoke, __invoke, arginfo_test_invoke___invoke, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Invoke, test, arginfo_test_invoke_test, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};
