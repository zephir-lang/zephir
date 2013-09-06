
extern zend_class_entry *test_scall_ce;

ZEPHIR_INIT_CLASS(Test_Scall);

PHP_METHOD(Test_Scall, testMethod1);
PHP_METHOD(Test_Scall, testMethod2);
PHP_METHOD(Test_Scall, testMethod3);
PHP_METHOD(Test_Scall, testMethod4);
PHP_METHOD(Test_Scall, testMethod5);
PHP_METHOD(Test_Scall, testMethod6);
PHP_METHOD(Test_Scall, testCall1);
PHP_METHOD(Test_Scall, testCall2);
PHP_METHOD(Test_Scall, testCall3);
PHP_METHOD(Test_Scall, testCall4);
PHP_METHOD(Test_Scall, testCall5);
PHP_METHOD(Test_Scall, testCall6);
PHP_METHOD(Test_Scall, testCall7);
PHP_METHOD(Test_Scall, testCall8);
PHP_METHOD(Test_Scall, testCall9);
PHP_METHOD(Test_Scall, testCall10);
PHP_METHOD(Test_Scall, testCall11);
PHP_METHOD(Test_Scall, testCall12);
PHP_METHOD(Test_Scall, testCall13);
PHP_METHOD(Test_Scall, testCall14);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testmethod4, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testmethod5, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testmethod6, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testcall4, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testcall5, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testcall6, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testcall10, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testcall11, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scall_testcall12, 0, 0, 0)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_scall_method_entry) {
	PHP_ME(Test_Scall, testMethod1, NULL, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testMethod2, NULL, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Test_Scall, testMethod3, NULL, ZEND_ACC_STATIC|ZEND_ACC_PRIVATE)
	PHP_ME(Test_Scall, testMethod4, arginfo_test_scall_testMethod4, ZEND_ACC_STATIC|ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testMethod5, arginfo_test_scall_testMethod5, ZEND_ACC_STATIC|ZEND_ACC_PROTECTED)
	PHP_ME(Test_Scall, testMethod6, arginfo_test_scall_testMethod6, ZEND_ACC_STATIC|ZEND_ACC_PRIVATE)
	PHP_ME(Test_Scall, testCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall4, arginfo_test_scall_testCall4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall5, arginfo_test_scall_testCall5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall6, arginfo_test_scall_testCall6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall10, arginfo_test_scall_testCall10, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall11, arginfo_test_scall_testCall11, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall12, arginfo_test_scall_testCall12, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall13, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_Scall, testCall14, NULL, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
