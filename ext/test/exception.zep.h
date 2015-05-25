
extern zend_class_entry *test_exception_ce;

ZEPHIR_INIT_CLASS(Test_Exception);

PHP_METHOD(Test_Exception, testRuntimePropertyFetch);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exception_testruntimepropertyfetch, 0, 0, 1)
	ZEND_ARG_INFO(0, message)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_exception_method_entry) {
	PHP_ME(Test_Exception, testRuntimePropertyFetch, arginfo_test_exception_testruntimepropertyfetch, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
