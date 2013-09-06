
extern zend_class_entry *test_mcall_ce;

ZEPHIR_INIT_CLASS(Test_Mcall);

PHP_METHOD(Test_Mcall, testMethod1);
PHP_METHOD(Test_Mcall, testMethod2);
PHP_METHOD(Test_Mcall, testMethod3);
PHP_METHOD(Test_Mcall, testMethod4);
PHP_METHOD(Test_Mcall, testMethod5);
PHP_METHOD(Test_Mcall, testMethod6);
PHP_METHOD(Test_Mcall, testCall1);
PHP_METHOD(Test_Mcall, testCall2);
PHP_METHOD(Test_Mcall, testCall3);
PHP_METHOD(Test_Mcall, testCall4);
PHP_METHOD(Test_Mcall, testCall5);
PHP_METHOD(Test_Mcall, testCall6);
PHP_METHOD(Test_Mcall, testCall7);
PHP_METHOD(Test_Mcall, testCall8);
PHP_METHOD(Test_Mcall, testCall9);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod4, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod5, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testmethod6, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall4, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall5, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall6, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall7, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall8, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_mcall_testcall9, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_mcall_method_entry) {
	PHP_ME(Test_Mcall, testMethod1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testMethod2, NULL, ZEND_ACC_PROTECTED)
	PHP_ME(Test_Mcall, testMethod3, NULL, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Mcall, testMethod4, arginfo_test_mcall_testMethod4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testMethod5, arginfo_test_mcall_testMethod5, ZEND_ACC_PROTECTED)
	PHP_ME(Test_Mcall, testMethod6, arginfo_test_mcall_testMethod6, ZEND_ACC_PRIVATE)
	PHP_ME(Test_Mcall, testCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall4, arginfo_test_mcall_testCall4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall5, arginfo_test_mcall_testCall5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall6, arginfo_test_mcall_testCall6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall7, arginfo_test_mcall_testCall7, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall8, arginfo_test_mcall_testCall8, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Mcall, testCall9, arginfo_test_mcall_testCall9, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
