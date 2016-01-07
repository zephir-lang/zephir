
extern zend_class_entry *test_exitdie_ce;

ZEPHIR_INIT_CLASS(Test_ExitDie);

PHP_METHOD(Test_ExitDie, testExit);
PHP_METHOD(Test_ExitDie, testDie);

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exitdie_testexit, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_test_exitdie_testdie, 0, 0, 0)
	ZEND_ARG_INFO(0, param)
ZEND_END_ARG_INFO()

ZEPHIR_INIT_FUNCS(test_exitdie_method_entry) {
	PHP_ME(Test_ExitDie, testExit, arginfo_test_exitdie_testexit, ZEND_ACC_PUBLIC)
	PHP_ME(Test_ExitDie, testDie, arginfo_test_exitdie_testdie, ZEND_ACC_PUBLIC)
	PHP_FE_END
};
