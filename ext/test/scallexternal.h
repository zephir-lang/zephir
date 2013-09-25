
extern zend_class_entry *test_scallexternal_ce;

ZEPHIR_INIT_CLASS(Test_ScallExternal);

PHP_METHOD(Test_ScallExternal, testCall1);
PHP_METHOD(Test_ScallExternal, testCall2);
PHP_METHOD(Test_ScallExternal, testCall3);
PHP_METHOD(Test_ScallExternal, testCall4);
PHP_METHOD(Test_ScallExternal, testCall5);
PHP_METHOD(Test_ScallExternal, testCall6);
PHP_METHOD(Test_ScallExternal, testCall7);
PHP_METHOD(Test_ScallExternal, testCall8);
PHP_METHOD(Test_ScallExternal, testCall9);
PHP_METHOD(Test_ScallExternal, testCall10);
PHP_METHOD(Test_ScallExternal, testCall11);
PHP_METHOD(Test_ScallExternal, testCall12);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall4, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall5, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall6, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall10, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall11, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall12, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_scallexternal_method_entry) {
	PHP_ME(Test_ScallExternal, testCall1, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall2, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall3, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall4, arginfo_test_scallexternal_testcall4, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall5, arginfo_test_scallexternal_testcall5, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall6, arginfo_test_scallexternal_testcall6, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall7, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall8, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall9, NULL, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall10, arginfo_test_scallexternal_testcall10, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall11, arginfo_test_scallexternal_testcall11, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall12, arginfo_test_scallexternal_testcall12, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
