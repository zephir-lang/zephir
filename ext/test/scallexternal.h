
extern zend_class_entry *test_scallexternal_ce;

ZEPHIR_INIT_CLASS(Test_ScallExternal);

PHP_METHOD(Test_ScallExternal, testCall1);
PHP_METHOD(Test_ScallExternal, testCall2);
PHP_METHOD(Test_ScallExternal, testCall3);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall1, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall2, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_scallexternal_testcall3, 0, 0, 2)
	ZEND_ARG_INFO(0, a)
	ZEND_ARG_INFO(0, b)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_scallexternal_method_entry) {
	PHP_ME(Test_ScallExternal, testCall1, arginfo_test_scallexternal_testcall1, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall2, arginfo_test_scallexternal_testcall2, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ScallExternal, testCall3, arginfo_test_scallexternal_testcall3, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
